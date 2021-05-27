/*
 * usb_msos20.c
 *
 * Microsoft OS 2.0 Descriptors support
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <no2usb/usb.h>
#include <no2usb/usb_msos20.h>
#include <no2usb/usb_msos20_proto.h>


// Generic descriptors
// --------------------

// User is free to use those by default, they will just setup the device to
// be usable with WinUSB. If some more fancy config is needed, then user
// can provive his own descriptors.
//
// The msos20_winusb_bos has to be used as 'BOS' descriptor in the device
// descriptor set passed during usb init.

static const struct {
	struct msos20_desc_set_hdr hdr;
	struct msos20_feat_compat_id_desc feat;
} __attribute__ ((packed)) _msos20_winusb_desc = {
	.hdr = {
		.wLength          = sizeof(struct msos20_desc_set_hdr),
		.wDescriptorType  = MSOS20_SET_HEADER_DESCRIPTOR,
		.dwWindowsVersion = MSOS20_WIN_VER_8_1,
		.wTotalLength     = sizeof(_msos20_winusb_desc),
	},
	.feat = {
		.wLength          = sizeof(struct msos20_feat_compat_id_desc),
		.wDescriptorType  = MSOS20_FEATURE_COMPATBLE_ID,
		.CompatibleID     = { 'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00 },
		.SubCompatibleID  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	},
};

static const struct {
	struct usb_bos_desc bos;
	struct usb_bos_plat_cap_hdr cap_hdr;
	struct usb_bos_msos20_desc_set cap_data;
} __attribute__ ((packed)) _msos20_winusb_bos = {
	.bos = {
		.bLength                = sizeof(struct usb_bos_desc),
		.bDescriptorType        = USB_DT_BOS,
		.wTotalLength           = sizeof(_msos20_winusb_bos),
		.bNumDeviceCaps         = 1,
	},
	.cap_hdr = {
		.bLength                = sizeof(struct usb_bos_plat_cap_hdr) + 8,
		.bDescriptorType        = USB_DT_DEV_CAP,
		.bDevCapabilityType     = 5, /* PLATFORM */
		.bReserved              = 0,
		.PlatformCapabilityUUID = {
			0xDF, 0x60, 0xDD, 0xD8, 0x89, 0x45, 0xC7, 0x4C,
			0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F,
		},
	},
	.cap_data = {
		.dwWindowsVersion              = MSOS20_WIN_VER_8_1,
		.wMSOSDescriptorSetTotalLength = sizeof(_msos20_winusb_desc),
		.bMS_VendorCode                = MSOS20_MS_VENDOR_CODE,
		.bAltEnumCode                  = 0x00,
	},
};

extern __attribute__ ((alias ("_msos20_winusb_desc"))) const struct msos20_desc_set_hdr msos20_winusb_desc;
extern __attribute__ ((alias ("_msos20_winusb_bos")))  const struct usb_bos_desc        msos20_winusb_bos;


// MS OS 2.0 driver
// ----------------

static struct {
	const struct msos20_desc_set_hdr *desc;
} g_msos20;

static enum usb_fnd_resp
_msos20_ctrl_req(struct usb_ctrl_req *req, struct usb_xfer *xfer)
{
	switch (req->wRequestAndType)
	{
	case ((MSOS20_MS_VENDOR_CODE << 8) | 0xc0):
		if (req->wIndex == MSOS20_DESCRIPTOR_INDEX) {
			xfer->data = (void*)g_msos20.desc;
			xfer->len  = g_msos20.desc->wTotalLength;
			return USB_FND_SUCCESS;
		} else {
			return USB_FND_ERROR;
		}
	}

	return USB_FND_CONTINUE;
}

static struct usb_fn_drv _msos20_drv = {
	.ctrl_req = _msos20_ctrl_req,
};

void
usb_msos20_init(const struct msos20_desc_set_hdr *desc)
{
	g_msos20.desc = desc ? desc : &_msos20_winusb_desc.hdr;
	usb_register_function_driver(&_msos20_drv);
}
