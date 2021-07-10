/*
 * dcd_no2usb.c
 *
 * DCD driver for the no2usb core
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: MIT
 */

#include "tusb_option.h"
#include "device/dcd.h"

#include "dcd_no2usb_hw.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>



/* ------------------------------------------------------------------------ */
/* Debug helpers                                                            */
/* ------------------------------------------------------------------------ */

#define L_NONE  0
#define L_ERROR 1
#define L_INFO  2
#define L_TRACE 3
#define L_EXTRA 4

#define DEBUG_LVL L_NONE

#if DEBUG_LVL > 0

# define USB_CHECK(x) do { \
	if (!(x)) \
		printf("USB: check fail: " #x " (%s:%d)\n", __FILE__, __LINE__); \
	} while (0)

# define USB_DEBUG(lvl, fmt, ...) do { \
		if ((DEBUG_LVL) >= (lvl)) { \
			printf("USB: " fmt "\n", ##__VA_ARGS__); \
		} \
	} while (0)

# define USB_DEBUG_BUF(lvl, buf, len, fmt, ...) do { \
		if ((DEBUG_LVL) >= (lvl)) { \
			if (len <= 8) { \
				printf("USB: " fmt ":", ##__VA_ARGS__); \
				for (int i=0; i<(len); i++) \
					printf(" %02x", ((uint8_t*)(buf))[i]); \
				printf("\n"); \
			} else { \
				printf("USB: " fmt "\n", ##__VA_ARGS__); \
				for (int i=0; i<(len); i++) { \
					if ((i&15) == 0) \
						printf("\t"); \
					printf("%02x", ((uint8_t*)(buf))[i]); \
					if (((i&15) == 15) || (i == ((len)-1))) \
						printf("\n"); \
					else \
						printf(" "); \
				} \
			} \
		} \
	} while (0)

#else

# define USB_CHECK(x)
# define USB_DEBUG(lvl, ...)
# define USB_DEBUG_BUF(lvl, buf, len, fmt, ...)

#endif


/* ------------------------------------------------------------------------ */
/* Globals                                                                  */
/* ------------------------------------------------------------------------ */

/* EP state */
struct dcd_ep {
	/* EP state / config */
	uint16_t mps;		/* Max Packet Size */

	bool busy;		/* xfer in progress */
	bool dual;		/* dual bufferred */
	uint8_t bdi_fill;	/* Next buffer to fill */
	uint8_t bdi_retire;	/* Next buffer to retire */

	/* Current transfer */
	struct {
		uint8_t *buf;	/* Buffer (NULL for ZLP) */
		uint16_t len;	/* Total transfer length */
		uint16_t ofs;	/* Offset in the total transfer */
		uint16_t plen;	/* Length of last queued packet */
	} xfer;
};

/* Global state */
static struct {
	/* Memory allocator */
	uint16_t mem[2];	/* [0]=OUT, [1]=IN */

	/* EP0 special */
	bool ep0_stall;

	/* EP statuses */
	volatile struct dcd_ep ep[16][2];
} g_usb;


/* ------------------------------------------------------------------------ */
/* Internal functions                                                       */
/* ------------------------------------------------------------------------ */

static inline volatile struct no2usb_ep *
_ep_regs(uint8_t epnum, uint8_t dir)
{
	return (dir == TUSB_DIR_OUT) ?
		&no2usb_ep_regs[epnum].out :
		&no2usb_ep_regs[epnum].in;
}

static inline volatile struct dcd_ep *
_ep_state(uint8_t epnum, uint8_t dir)
{
	return &g_usb.ep[epnum][dir];
}


static void
_usb_data_write(unsigned int dst_ofs, const void *src, int len)
{
	/* Only handles aligned offsets ! */
	/* Our allocator ensures that buffers are aligned */
	const uint32_t *src_u32 = src;
	volatile uint32_t *dst_u32 = (volatile uint32_t *)((NO2USB_DATA_TX_BASE) + dst_ofs);

	len = (len + 3) >> 2;
	while (len--)
		*dst_u32++ = *src_u32++;
}

static void
_usb_data_read (void *dst, unsigned int src_ofs, int len)
{
	/* Only handles aligned offsets ! */
	/* Our allocator ensures that buffers are aligned */
	volatile uint32_t *src_u32 = (volatile uint32_t *)((NO2USB_DATA_RX_BASE) + src_ofs);
	uint32_t *dst_u32 = dst;

	int i = len >> 2;

	while (i--)
		*dst_u32++ = *src_u32++;

	if ((len &= 3) != 0) {
		uint32_t x = *src_u32;
		uint8_t  *dst_u8 = (uint8_t *)dst_u32;
		while (len--) {
			*dst_u8++ = x & 0xff;
			x >>= 8;
		}
	}
}


static void
_usb_hw_reset_ep(volatile struct no2usb_ep *epr)
{
	epr->status = 0;
	epr->bd[0].csr = 0;
	epr->bd[0].ptr = 0;
	epr->bd[1].csr = 0;
	epr->bd[1].ptr = 0;
}

static void
_usb_hw_reset(bool pu)
{
	/* Reset all EPs */
	for (int i=0; i<16; i++) {
		_usb_hw_reset_ep(&no2usb_ep_regs[i].out);
		_usb_hw_reset_ep(&no2usb_ep_regs[i].in);
	}

	/* Main control */
	no2usb_regs->csr =
		(pu ? NO2USB_CSR_PU_ENA : 0) |
		NO2USB_CSR_CEL_ENA |
		NO2USB_CSR_ADDR_MATCH |
		NO2USB_CSR_ADDR(0);

	no2usb_regs->ar =
		NO2USB_AR_BUS_RST_CLEAR |
		NO2USB_AR_SOF_CLEAR |
		NO2USB_AR_CEL_RELEASE;
}

static inline uint16_t
_usb_hw_buf_alloc(uint8_t dir, int mps)
{
	uint16_t rv = g_usb.mem[dir];
	g_usb.mem[dir] += mps;
	return rv;
}


static void
_usb_ep_advance_xfer_in(const uint8_t epnum)
{
	volatile struct no2usb_ep *epr = _ep_regs(epnum, TUSB_DIR_IN);
	volatile struct dcd_ep *eps = _ep_state(epnum, TUSB_DIR_IN);

	uint32_t bds;
	int len;

	/* Retire done descriptors */
	while (1)
	{
		/* Get BD status */
		bds = epr->bd[eps->bdi_retire].csr;

		/* Packet done ? */
		if ((bds & NO2USB_BD_STATE_MSK) == NO2USB_BD_STATE_DONE_OK)
		{
			/* Reset the descriptor */
			epr->bd[eps->bdi_retire].csr = 0;
		}

		/* Errors are not valid for TX. The HW will auto retry and never report this */
		else if ((bds & NO2USB_BD_STATE_MSK) == NO2USB_BD_STATE_DONE_ERR)
		{
			/* So if it happens ... reset things and hope for the best ? */
			epr->bd[eps->bdi_retire].csr = 0;

			USB_DEBUG(L_ERROR, "NO2USB_BD_STATE_DONE_ERR on EP%d IN !", epnum);
		}

		/* Ok, current BD was neither done or error, we stop here */
		else
		{
			break;
		}

		/* Next ! */
		eps->bdi_retire ^= eps->dual;
	}

	/* If we don't have a transfer ... we have nothing to do ! */
	if (!eps->busy)
		return;

	/* Fill as many descriptors as possible */
	while (eps->busy)
	{
		/* Get BD status */
		bds = epr->bd[eps->bdi_fill].csr;

		/* If BD is not used, then re-fill */
		if ((bds & NO2USB_BD_STATE_MSK) == NO2USB_BD_STATE_NONE)
		{
			/* Load packet in buffer */
			if (eps->xfer.buf) {
				/* Select packet size */
				eps->xfer.plen = eps->xfer.len - eps->xfer.ofs;
				if (eps->xfer.plen > eps->mps)
					eps->xfer.plen = eps->mps;

				/* Fill data buffer */
				_usb_data_write(epr->bd[eps->bdi_fill].ptr, &eps->xfer.buf[eps->xfer.ofs], eps->xfer.plen);
			}

			/* Submit packet */
			epr->bd[eps->bdi_fill].csr = NO2USB_BD_STATE_RDY_DATA | NO2USB_BD_LEN(eps->xfer.plen);

			/* Advance the transfer, and maybe finish it */
			eps->xfer.ofs += eps->xfer.plen;

			if (eps->xfer.ofs == eps->xfer.len)
			{
				len = eps->xfer.len;

				eps->busy      = false;
				eps->xfer.buf  = NULL;
				eps->xfer.len  = 0;
				eps->xfer.ofs  = 0;
				eps->xfer.plen = 0;

				USB_DEBUG(L_TRACE, "dcd_edpt_xfer_complete() EP%d IN  len %d", epnum, len);

				dcd_event_xfer_complete(0, epnum | 0x80, len, XFER_RESULT_SUCCESS, true);
			}
		}
		else
		{
			/* No buffer ready to fill, we're done for now */
			break;
		}

		/* Next ! */
		eps->bdi_fill ^= eps->dual;
	}
}

static void
_usb_ep_advance_xfer_out(const uint8_t epnum)
{
	volatile struct no2usb_ep *epr = _ep_regs(epnum, TUSB_DIR_OUT);
	volatile struct dcd_ep *eps = _ep_state(epnum, TUSB_DIR_OUT);

	uint32_t bds;
	int len;

	/* Retire done descriptors */
	while (1)
	{
		/* Get BD status */
		bds = epr->bd[eps->bdi_retire].csr;

		/* Packet done ? */
		if ((bds & NO2USB_BD_STATE_MSK) == NO2USB_BD_STATE_DONE_OK)
		{
			/* We have data ! And nowhere to put it ... abort */
			if (!eps->busy)
				break;

			/* Get packet length */
			eps->xfer.plen = (bds & NO2USB_BD_LEN_MSK) - 2;
			if (eps->xfer.plen > (eps->xfer.len - eps->xfer.ofs))
				eps->xfer.plen = eps->xfer.len - eps->xfer.ofs;

			/* Reset the descriptor */
			epr->bd[eps->bdi_retire].csr = 0;

			/* Grab data from buffer (if any) */
			if (eps->xfer.plen) {
				_usb_data_read(&eps->xfer.buf[eps->xfer.ofs], epr->bd[eps->bdi_retire].ptr, eps->xfer.plen);
				eps->xfer.ofs += eps->xfer.plen;
			}

			/* End of transfer when requested length is reached, or a short transfer from host */
			if ((eps->xfer.plen < eps->mps) || (eps->xfer.len == eps->xfer.ofs))
			{
				len = eps->xfer.ofs;

				eps->busy      = false;
				eps->xfer.buf  = NULL;
				eps->xfer.len  = 0;
				eps->xfer.ofs  = 0;
				eps->xfer.plen = 0;

				USB_DEBUG(L_TRACE, "dcd_edpt_xfer_complete() EP%d OUT  len %d", epnum, len);

				dcd_event_xfer_complete(0, epnum, len, XFER_RESULT_SUCCESS, true);
			}
		}

		/* Or maybe an error ? */
		else if ((bds & NO2USB_BD_STATE_MSK) == NO2USB_BD_STATE_DONE_ERR)
		{
			/* Just retry */
			epr->bd[0].csr = NO2USB_BD_STATE_RDY_DATA | NO2USB_BD_LEN(eps->mps);
		}

		/* Ok, current BD was neither done or error, we stop here */
		else
		{
			break;
		}

		/* Next ! */
		eps->bdi_retire ^= eps->dual;
	}

	/* Prepare any descriptors that's not ready */
	while (1)
	{
		/* Get BD status */
		bds = epr->bd[eps->bdi_fill].csr;

		/* Refill only if not initialized. If there is something, we're done for now */
		/* For EP0, we don't prefill, so only init if we have a transfer pending */
		if (((bds & NO2USB_BD_STATE_MSK) == NO2USB_BD_STATE_NONE) &&
		    (eps->busy || (epnum != 0)))
		{
			epr->bd[eps->bdi_fill].csr = NO2USB_BD_STATE_RDY_DATA | NO2USB_BD_LEN(eps->mps);
		}
		else
		{
			/* No buffer ready to fill, we're done for now */
			break;
		}

		/* Next ! */
		eps->bdi_fill ^= eps->dual;
	}
}

static void
_usb_ep0_configure(void)
{
	volatile struct no2usb_ep_pair *ep0r = &no2usb_ep_regs[0];
	const int mps = CFG_TUD_ENDPOINT0_SIZE;

	USB_DEBUG(L_INFO, "Setting up control endpoint EP0");

	/* Setup the EP config */
	ep0r->out.status = NO2USB_EP_TYPE_CTRL | NO2USB_EP_BD_CTRL; /* Type=Control, control mode buffered */
	ep0r->in.status  = NO2USB_EP_TYPE_CTRL | NO2USB_EP_DT_BIT;  /* Type=Control, single buffered, DT=1 */

	/* Setup the BDs */
	ep0r->in.bd[0].ptr  = _usb_hw_buf_alloc(TUSB_DIR_IN,  mps);
	ep0r->out.bd[0].ptr = _usb_hw_buf_alloc(TUSB_DIR_OUT, mps);
	ep0r->out.bd[1].ptr = _usb_hw_buf_alloc(TUSB_DIR_OUT, 8);

	ep0r->in.bd[0].csr  = 0;
	ep0r->out.bd[0].csr = 0;
	ep0r->out.bd[1].csr = NO2USB_BD_STATE_RDY_DATA | NO2USB_BD_LEN(8);

	/* EP DCD config */
	g_usb.ep[0][TUSB_DIR_IN].mps  = mps;
	g_usb.ep[0][TUSB_DIR_OUT].mps = mps;
}

static void
_usb_ep0_handle_setup(void)
{
	uint32_t bds_setup = no2usb_ep_regs[0].out.bd[1].csr;

	if ((bds_setup & NO2USB_BD_STATE_MSK) == NO2USB_BD_STATE_DONE_OK)
	{
		uint8_t setup_pkt[8];

		/* If there is still IN/OUT pending from before SETUP, make
		 * sure to finish them */
		if (g_usb.ep[0][TUSB_DIR_IN].busy)
			_usb_ep_advance_xfer_in(0);
		if (g_usb.ep[0][TUSB_DIR_OUT].busy)
			_usb_ep_advance_xfer_out(0);

		/* Read data from USB buffer */
		_usb_data_read(setup_pkt, no2usb_ep_regs[0].out.bd[1].ptr, 8);

		USB_DEBUG_BUF(L_INFO, setup_pkt, 8, "SETUP pkt rx");

		/* Clear IN/OUT/STALL */
		no2usb_ep_regs[0].in.bd[0].csr  = 0;
		no2usb_ep_regs[0].out.bd[0].csr = 0;
		g_usb.ep0_stall = false;

		/* Make sure DT=1 for IN endpoint after a SETUP */
		no2usb_ep_regs[0].in.status = NO2USB_EP_TYPE_CTRL | NO2USB_EP_DT_BIT;

		/* Setup next SETUP buffer */
		no2usb_ep_regs[0].out.bd[1].csr = NO2USB_BD_STATE_RDY_DATA | NO2USB_BD_LEN(8);

		/* Release lockout */
		no2usb_regs->ar = NO2USB_AR_CEL_RELEASE;

		/* Notify tinyusb stack */
		dcd_event_setup_received(0, setup_pkt, true);
	}
	else if ((bds_setup & NO2USB_BD_STATE_MSK) == NO2USB_BD_STATE_DONE_ERR)
	{
		/* Just setup a new one ... */
		no2usb_ep_regs[0].out.bd[1].csr = NO2USB_BD_STATE_RDY_DATA | NO2USB_BD_LEN(8);
	}
}


static void
_usb_bus_reset(void)
{
	USB_DEBUG(L_TRACE, "_usb_bus_reset()");

	/* Reset hardware */
	_usb_hw_reset(true);

	/* Main state reset */
	memset(&g_usb, 0x00, sizeof(g_usb));

	/* Configure EP0 */
	_usb_ep0_configure();

	/* Signal bus reset */
	dcd_event_bus_signal(0, DCD_EVENT_BUS_RESET, true);
}


/* ------------------------------------------------------------------------ */
/* Controller API                                                           */
/* ------------------------------------------------------------------------ */

void
dcd_init(uint8_t rhport)
{
	(void) rhport;

	USB_DEBUG(L_TRACE, "dcd_init()");

	/* Main state reset */
	memset(&g_usb, 0x00, sizeof(g_usb));

	/* Reset and enable the core */
	_usb_hw_reset(true);
}

void
dcd_int_enable(uint8_t rhport)
{
	(void) rhport;

	USB_DEBUG(L_EXTRA, "dcd_int_enable()");

	no2usb_regs->ir =
#ifdef NO2USB_WITH_SOF
		NO2USB_IR_SOF_PENDING |
#endif
		NO2USB_IR_EVT_PENDING |
		NO2USB_IR_BUS_RST_RELEASE;
}

void
dcd_int_disable(uint8_t rhport)
{
	(void) rhport;

	USB_DEBUG(L_EXTRA, "dcd_int_disable()");

	no2usb_regs->ir = 0;
}

void
dcd_int_handler(uint8_t rhport)
{
	uint32_t csr = no2usb_regs->csr;
	bool do_ep_poll = false;

	/* Handle resets */
	if (csr & NO2USB_CSR_BUS_RST_PENDING) {
		if (csr & NO2USB_CSR_BUS_RST) {
			return;
		}
		_usb_bus_reset();
	}

	/* Handle SoF */
#ifdef NO2USB_WITH_SOF
	if (csr & NO2USB_CSR_SOF_PENDING) {
		no2usb_regs->ar = NO2USB_AR_SOF_CLEAR;
		dcd_event_bus_signal(0, DCD_EVENT_SOF, true);
	}
#endif

	/* If there is nothing else, abort early */
	if (!(csr & NO2USB_CSR_EVT_PENDING))
		return;

	/* Unqueue / Ack events */
	while (no2usb_regs->csr & NO2USB_CSR_EVT_PENDING)
	{
		uint16_t evt = no2usb_regs->evt;

		USB_DEBUG(L_TRACE, "Event %04x", (unsigned int)evt);

#ifdef NO2USB_WITH_EVENT_FIFO
		/* Should not happen */
		USB_CHECK(evt & NO2USB_EVT_VALID);

		/* Just dequeue if we're already committed to a full poll */
		if (do_ep_poll)
			continue;

		/* If an overflow occured, we need to do a full poll */
		if (evt & NO2USB_EVT_OVERFLOW) {
			USB_DEBUG(L_ERROR, "Event FIFO overflow");
			do_ep_poll = true;
			continue;
		}

		/* Advance the transfer for the target EP */
		int epnum = NO2USB_EVT_GET_EP(evt);

		if (evt & NO2USB_EVT_IS_SETUP) {
			/* SETUP are special */
			_usb_ep0_handle_setup();
		} else if ((epnum == 0) && g_usb.ep0_stall) {
			/* Event was for EP0 IN/OUT during STALL, just force refresh */
			no2usb_ep_regs[epnum].out.bd[0].csr = NO2USB_BD_STATE_RDY_STALL;
			no2usb_ep_regs[epnum].in.bd[0].csr  = NO2USB_BD_STATE_RDY_STALL;
		} else if (evt & NO2USB_EVT_DIR_IN) {
			/* Normal IN xfer */
			_usb_ep_advance_xfer_in(epnum);
		} else {
			/* Normal OUT xfer */
			_usb_ep_advance_xfer_out(epnum);
		}
#else
		/* No event fifo */
		/* -> Always poll all busy EP if something happenned */
		(void)evt;
		do_ep_poll = true;
#endif
	}

	/* Poll of all busy EP */
	if (do_ep_poll)
	{
		/* EP0 stalls */
		if (g_usb.ep0_stall) {
			no2usb_ep_regs[0].out.bd[0].csr = NO2USB_BD_STATE_RDY_STALL;
			no2usb_ep_regs[0].in.bd[0].csr  = NO2USB_BD_STATE_RDY_STALL;
		}

		/* Normal xfers */
		for (int ep=0; ep<16; ep++) {
			if (g_usb.ep[ep][TUSB_DIR_IN].busy)
				_usb_ep_advance_xfer_in(ep);
			if (g_usb.ep[ep][TUSB_DIR_OUT].busy)
				_usb_ep_advance_xfer_out(ep);
		}

		/* SETUP */
		_usb_ep0_handle_setup();
	}
}

void
dcd_set_address(uint8_t rhport, uint8_t dev_addr)
{
	(void) rhport;

	volatile struct no2usb_ep *ep0r_in = &no2usb_ep_regs[0].in;

	USB_DEBUG(L_TRACE, "dcd_set_address() dev_addr=%d", dev_addr);

	/* Manually queue a IN ZLP and wait for it to be done */
	dcd_int_disable(rhport);

	ep0r_in->bd[0].csr = NO2USB_BD_STATE_RDY_DATA | NO2USB_BD_LEN(0);
	while ((ep0r_in->bd[0].csr & NO2USB_BD_STATE_MSK) != NO2USB_BD_STATE_DONE_OK);
	ep0r_in->bd[0].csr = 0;

	dcd_int_enable(rhport);

	/* Setup address filtering */
	no2usb_regs->csr =
		NO2USB_CSR_PU_ENA |
		NO2USB_CSR_CEL_ENA |
		NO2USB_CSR_ADDR_MATCH |
		NO2USB_CSR_ADDR(dev_addr);

	USB_DEBUG(L_TRACE, "dcd_set_address() done !");
}

void
dcd_remote_wakeup(uint8_t rhport)
{
	(void) rhport;

	/* not supported */
}


/* ------------------------------------------------------------------------ */
/* Endpoint API                                                             */
/* ------------------------------------------------------------------------ */

bool
dcd_edpt_open(uint8_t rhport, tusb_desc_endpoint_t const * desc_edpt)
{
	(void) rhport;

	uint8_t const epnum = tu_edpt_number(desc_edpt->bEndpointAddress);
	uint8_t const dir   = tu_edpt_dir   (desc_edpt->bEndpointAddress);

	USB_CHECK(epnum != 0);
	USB_DEBUG(L_TRACE, "dcd_edpt_open() EP%d %s",
		epnum, (dir == TUSB_DIR_OUT) ? "OUT" : "IN "
	);

	volatile struct no2usb_ep *epr = _ep_regs(epnum, dir);
	volatile struct dcd_ep *eps = _ep_state(epnum, dir);
	int type = 0;
	bool dual = false;

	/* Type */
	switch (desc_edpt->bmAttributes.xfer) {
	case TUSB_XFER_ISOCHRONOUS:
		type = NO2USB_EP_TYPE_ISOC;
		break;

	case TUSB_XFER_BULK:
		type = NO2USB_EP_TYPE_BULK;
		break;

	case TUSB_XFER_INTERRUPT:
		type = NO2USB_EP_TYPE_INT;
		break;

	default:
		USB_DEBUG(L_ERROR, "Unsupported type for EP: %d", desc_edpt->bmAttributes.xfer);
		return false;
	}

	USB_DEBUG(L_INFO, "Setting up %s endpoint EP%d. type=%d dual=%d",
		(dir == TUSB_DIR_OUT) ? "OUT" : "IN ",
		epnum, type, dual
	);

	dual = (type == NO2USB_EP_TYPE_BULK);

	/* Setup EP DCD state */
	memset((void*)eps, 0x00, sizeof(*eps));
	eps->mps = desc_edpt->wMaxPacketSize.size;
	eps->dual = dual;

	/* Setup the BDs */
	epr->bd[0].ptr =        _usb_hw_buf_alloc(dir, eps->mps);
	epr->bd[1].ptr = dual ? _usb_hw_buf_alloc(dir, eps->mps) : 0;
	epr->bd[0].csr = 0;
	epr->bd[1].csr = 0;

	if (dir == TUSB_DIR_OUT) {
		epr->bd[0].csr = NO2USB_BD_STATE_RDY_DATA | NO2USB_BD_LEN(eps->mps);
		epr->bd[1].csr = NO2USB_BD_STATE_RDY_DATA | NO2USB_BD_LEN(eps->mps);
	}

	epr->status = type | (dual ? NO2USB_EP_BD_DUAL : 0);

	return true;
}

bool
dcd_edpt_xfer(uint8_t rhport, uint8_t ep_addr, uint8_t *buffer, uint16_t total_bytes)
{
	(void) rhport;

	uint8_t const epnum = tu_edpt_number(ep_addr);
	uint8_t const dir   = tu_edpt_dir(ep_addr);

	USB_DEBUG(L_TRACE, "dcd_edpt_xfer() EP%d %s  len %d, buf %08x",
		epnum, (dir == TUSB_DIR_OUT) ? "OUT" : "IN ",
		total_bytes, (unsigned int)buffer
	);

	volatile struct dcd_ep *eps = _ep_state(epnum, dir);

	/* Disable interrupts */
	dcd_int_disable(rhport);

	/* Setup transfer state */
	eps->busy      = true;
	eps->xfer.buf  = buffer;
	eps->xfer.len  = total_bytes;
	eps->xfer.ofs  = 0;
	eps->xfer.plen = 0;

	/* Bootstrap transfer */
	if (dir == TUSB_DIR_OUT)
		_usb_ep_advance_xfer_out(epnum);
	else
		_usb_ep_advance_xfer_in(epnum);

	/* Re-enable interrupts */
	dcd_int_enable(rhport);

	return true;
}

void
dcd_edpt_stall (uint8_t rhport, uint8_t ep_addr)
{
	(void) rhport;

	uint8_t const epnum = tu_edpt_number(ep_addr);
	uint8_t const dir   = tu_edpt_dir(ep_addr);

	USB_DEBUG(L_TRACE, "dcd_edpt_stall() EP%d %s",
		epnum, (dir == TUSB_DIR_OUT) ? "OUT" : "IN "
	);

	if (epnum == 0) {
		/* Can't halt EP0 since we still need to receive SETUP.
		 * So we need to manually queue STALL.
		 * We also only do it on OUT since tinyUSB will stall both one after
		 * the other and  we want to prevent any interrupt processing between
		 * setting them.
		 */
		if (dir == TUSB_DIR_OUT) {
			dcd_int_disable(rhport);
			g_usb.ep0_stall = true;
			no2usb_ep_regs[0].out.bd[0].csr = NO2USB_BD_STATE_RDY_STALL;
			no2usb_ep_regs[0].in.bd[0].csr  = NO2USB_BD_STATE_RDY_STALL;
			dcd_int_enable(rhport);
		}
	} else {
		/* Simply halt the end point */
		volatile struct no2usb_ep *epr = _ep_regs(epnum, dir);
		epr->status |= NO2USB_EP_TYPE_HALTED;
	}
}

void
dcd_edpt_clear_stall(uint8_t rhport, uint8_t ep_addr)
{
	(void) rhport;

	uint8_t const epnum = tu_edpt_number(ep_addr);
	uint8_t const dir   = tu_edpt_dir(ep_addr);

	USB_DEBUG(L_TRACE, "dcd_edpt_clear_stall() EP%d %s",
		epnum, (dir == TUSB_DIR_OUT) ? "OUT" : "IN "
	);

	if (epnum == 0) {
		/* This is handled when we get a SETUP packet */
	} else {
		/* Resume the end point */
		volatile struct no2usb_ep *epr = _ep_regs(epnum, dir);
		epr->status &= ~NO2USB_EP_TYPE_HALTED;
	}
}


void
dcd_edpt_debug(uint8_t rhport, uint8_t ep_addr)
{
	uint8_t const epnum = tu_edpt_number(ep_addr);
	uint8_t const dir   = tu_edpt_dir(ep_addr);

	volatile struct no2usb_ep *epr = _ep_regs(epnum, dir);
	volatile struct dcd_ep *eps = _ep_state(epnum, dir);

	printf("EP%d %s:\n", epnum, (dir == TUSB_DIR_OUT) ? "OUT" : "IN ");
	printf(" epr: status=%04x,  bd[0].csr=%04x bd[1].csr=%04x\n",
		(unsigned int)epr->status,
		(unsigned int)epr->bd[0].csr,
		(unsigned int)epr->bd[1].csr);
	printf(" eps: busy=%d, dual=%d, bdi_fill=%d, bdi_retire=%d, buf=%08x, len=%d, ofs=%d, plen=%d\n",
		eps->busy,
		eps->dual,
		eps->bdi_fill,
		eps->bdi_retire,
		(unsigned int)eps->xfer.buf,
		eps->xfer.len,
		eps->xfer.ofs,
		eps->xfer.plen);
}
