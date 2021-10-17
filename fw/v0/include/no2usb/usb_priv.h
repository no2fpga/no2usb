/*
 * usb_priv.h
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include <stdint.h>

#include "usb.h"
#include "usb_proto.h"


/* Debug logging */
/* ------------- */

//#define USB_LOG_LEVEL 1

#ifdef USB_LOG_LEVEL
# define USB_LOG(lvl,...)	if ((lvl) <= USB_LOG_LEVEL) printf(__VA_ARGS__)
#else
# define USB_LOG(lvl,...)	do {} while(0)
#endif

#define USB_LOG_ERR(...)	USB_LOG(1, __VA_ARGS__)
#define USB_LOG_INFO(...)	USB_LOG(2, __VA_ARGS__)


/* Internal functions */
/* ------------------ */

/* Stack */
struct usb_stack {
	/* Driver config */
	const struct usb_stack_descriptors *stack_desc;

	/* Device state */
	enum usb_dev_state state;

	const struct usb_conf_desc *conf;
	uint32_t intf_alt;

	/* Timebase */
	uint32_t tick;

	/* EP configuration */
	struct {
		unsigned int mem[2];
	} ep_cfg;

	/* EP0 control state */
	struct {
		enum {
			IDLE,
			DATA_IN,		/* Data stage via 'IN'        */
			DATA_OUT,		/* Data stage via 'OUT'       */
			STATUS_DONE_OUT,	/* Status sent via 'OUT' EP   */
			STATUS_DONE_IN,		/* Status sent via 'IN' EP    */
			STALL,			/* Stalled until next `SETUP` */
		} state;

		uint8_t buf[64];

		struct usb_xfer xfer;
		struct usb_ctrl_req req;
	} ctrl;

	/* Function drivers */
	struct usb_fn_drv *fnd;
};

extern struct usb_stack g_usb;

/* Helpers for data access */
void usb_data_write(unsigned int dst_ofs, const void *src, int len);
void usb_data_read(void *dst, unsigned int src_ofs, int len);


void usb_dispatch_sof(void);
void usb_dipatch_bus_reset(void);
void usb_dispatch_state_chg(enum usb_dev_state state);
enum usb_fnd_resp usb_dispatch_ctrl_req(struct usb_ctrl_req *req, struct usb_xfer *xfer);
enum usb_fnd_resp usb_dispatch_set_conf(const struct usb_conf_desc *desc);
enum usb_fnd_resp usb_dispatch_set_intf(const struct usb_intf_desc *base, const struct usb_intf_desc *sel);
enum usb_fnd_resp usb_dispatch_get_intf(const struct usb_intf_desc *base, uint8_t *sel);

/* Control */
void usb_ep0_reset(void);
void usb_ep0_poll(void);

extern struct usb_fn_drv usb_ctrl_std_drv;
