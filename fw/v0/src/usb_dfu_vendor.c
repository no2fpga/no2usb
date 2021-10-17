/*
 * usb_dfu_vendor.c
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <no2usb/usb.h>
#include <no2usb/usb_dfu.h>


#define USB_RT_DFU_VENDOR_VERSION	((0 << 8) | 0xc1)
#define USB_RT_DFU_VENDOR_SPI_EXEC	((1 << 8) | 0x41)
#define USB_RT_DFU_VENDOR_SPI_RESULT	((2 << 8) | 0xc1)


static bool
_dfu_vendor_spi_exec_cb(struct usb_xfer *xfer)
{
	usb_dfu_cb_flash_raw(xfer->data, xfer->len);
	return true;
}

enum usb_fnd_resp
dfu_vendor_ctrl_req(struct usb_ctrl_req *req, struct usb_xfer *xfer)
{
	switch (req->wRequestAndType)
	{
	case USB_RT_DFU_VENDOR_VERSION:
		xfer->len  = 2;
		xfer->data[0] = 0x01;
		xfer->data[1] = 0x00;
		break;

	case USB_RT_DFU_VENDOR_SPI_EXEC:
		xfer->cb_done = _dfu_vendor_spi_exec_cb;
		break;

	case USB_RT_DFU_VENDOR_SPI_RESULT:
		/* Really nothing to do, data is already in the buffer, and we serve
		 * whatever the host requested ... */
		break;

	default:
		return USB_FND_ERROR;
	}

	return USB_FND_SUCCESS;
}
