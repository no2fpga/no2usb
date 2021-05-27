/*
 * usb_hid_proto.h
 *
 * See USB Class Definitions for Human Interface Devices (HID), Revision 1.11
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once


/* Interfaces Subclasses */

enum usb_hid_subclass {
	USB_HID_SCLS_NONE	= 0x00,
	USB_HID_SCLS_BOOT	= 0x01,
};


/* Interfaces Protocols */

enum usb_hid_protocol {
	USB_HID_PROTO_NONE	= 0x00,
	USB_HID_PROTO_KEYBOARD	= 0x01,
	USB_HID_PROTO_MOUSE	= 0x02,
};


/* Descriptors */

enum usb_hid_desc_type {
	USB_HID_DT_HID		= 0x21,
	USB_HID_DT_REPORT	= 0x22,
	USB_HID_DT_PHYSICAL	= 0x23,
};

#define usb_hid_hid_desc_def(n) \
	struct usb_hid_hid_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptorType; \
		uint16_t bcdHID; \
		uint8_t  bCountryCode; \
		uint8_t  bNumDescriptors; \
		struct { \
			uint8_t  bDescriptorType; \
			uint16_t wDescriptorLength; \
		} __attribute__ ((packed)) desc[n]; \
	} __attribute__ ((packed))

usb_hid_hid_desc_def(1);
#define usb_hid_hid_desc usb_hid_hid_desc__1


/* Control requests */

#define USB_REQ_HID_GET_REPORT		(0x01)
#define USB_REQ_HID_SET_REPORT		(0x09)
#define USB_REQ_HID_GET_IDLE		(0x02)
#define USB_REQ_HID_SET_IDLE		(0x0a)
#define USB_REQ_HID_GET_PROTOCOL	(0x03)
#define USB_REQ_HID_SET_PROTOCOL	(0x0b)

#define USB_RT_HID_GET_REPORT		((0x01 << 8) | 0xa1)
#define USB_RT_HID_SET_REPORT		((0x09 << 8) | 0x21)
#define USB_RT_HID_GET_IDLE		((0x02 << 8) | 0xa1)
#define USB_RT_HID_SET_IDLE		((0x0a << 8) | 0x21)
#define USB_RT_HID_GET_PROTOCOL		((0x03 << 8) | 0xa1)
#define USB_RT_HID_SET_PROTOCOL		((0x0b << 8) | 0x21)

#define USB_RT_HID_GET_DESCRIPTOR	(( 6 << 8) | 0x81)
#define USB_RT_HID_SET_DESCRIPTOR	(( 7 << 8) | 0x01)
