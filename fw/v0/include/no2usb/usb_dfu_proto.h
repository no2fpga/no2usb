/*
 * usb_dfu_proto.h
 *
 * See USB Device Firmware Upgrade Specification, Revision 1.1
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

/* Descriptors (Section 4) */

enum usb_dfu_desc_type {
	USB_DFU_DT_FUNC		= 33,
};

struct usb_dfu_func_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bmAttributes;
	uint16_t wDetachTimeOut;
	uint16_t wTransferSize;
	uint16_t bcdDFUVersion;
} __attribute__((packed));


/* Control requests (Section 3, Table 3.1/3.2) */

#define USB_REQ_DFU_DETACH	(0)
#define USB_REQ_DFU_DNLOAD	(1)
#define USB_REQ_DFU_UPLOAD	(2)
#define USB_REQ_DFU_GETSTATUS	(3)
#define USB_REQ_DFU_CLRSTATUS	(4)
#define USB_REQ_DFU_GETSTATE	(5)
#define USB_REQ_DFU_ABORT	(6)

#define USB_RT_DFU_DETACH	((0 << 8) | 0x21)
#define USB_RT_DFU_DNLOAD	((1 << 8) | 0x21)
#define USB_RT_DFU_UPLOAD	((2 << 8) | 0xa1)
#define USB_RT_DFU_GETSTATUS	((3 << 8) | 0xa1)
#define USB_RT_DFU_CLRSTATUS	((4 << 8) | 0x21)
#define USB_RT_DFU_GETSTATE	((5 << 8) | 0xa1)
#define USB_RT_DFU_ABORT	((6 << 8) | 0x21)


/* State / Status (Section 6.1.2) */

enum dfu_state {
	appIDLE = 0,
	appDETACH,
	dfuIDLE,
	dfuDNLOAD_SYNC,
	dfuDNBUSY,
	dfuDNLOAD_IDLE,
	dfuMANIFEST_SYNC,
	dfuMANIFEST,
	dfuMANIFEST_WAIT_RESET,
	dfuUPLOAD_IDLE,
	dfuERROR,
	_DFU_MAX_STATE
};

enum dfu_status {
	OK = 0,
	errTARGET,
	errFILE,
	errWRITE,
	errERASE,
	errCHECK_ERASED,
	errPROG,
	errVERIFY,
	errADDRESS,
	errNOTDONE,
	errFIRMWARE,
	errVENDOR,
	errUSBR,
	errPOR,
	errUNKNOWN,
	errSTALLEDPKT,
	_DFU_MAX_STATUS
};
