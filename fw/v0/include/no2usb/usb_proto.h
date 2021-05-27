/*
 * usb_proto.h
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include <stdint.h>

// Helpers
// -------

#ifndef NULL
#define NULL  ((void*)0)
#endif

#define num_elem(a) (sizeof(a) / sizeof(a[0]))

#define U16_TO_U8_LE(x) ((x) & 0xff), (((x) >> 8) & 0xff)
#define U24_TO_U8_LE(x) ((x) & 0xff), (((x) >> 8) & 0xff), (((x) >> 16) & 0xff)
#define U32_TO_U8_LE(x) ((x) & 0xff), (((x) >> 8) & 0xff), (((x) >> 16) & 0xff), (((x) >> 24) & 0xff)


// Descriptors
// -----------

enum usb_desc_type {
	USB_DT_DEV		= 1,
	USB_DT_CONF		= 2,
	USB_DT_STR		= 3,
	USB_DT_INTF		= 4,
	USB_DT_EP		= 5,
	USB_DT_DEV_QUAL		= 6,
	USB_DT_OTHER_SPEED_CONF	= 7,
	USB_DT_INTF_PWR		= 8,
	USB_DT_OTG		= 9,
	USB_DT_DEBUG		= 10,
	USB_DT_INTF_ASSOC	= 11,
	USB_DT_BOS		= 15,
	USB_DT_DEV_CAP		= 16,
};

enum usb_cs_desc_type {
	USB_CS_DT_UNDEFINED	= 32,
	USB_CS_DT_DEV		= 33,
	USB_CS_DT_CONF		= 34,
	USB_CS_DT_STR		= 35,
	USB_CS_DT_INTF		= 36,
	USB_CS_DT_EP		= 37,
};

enum usb_class {
	USB_CLS_AUDIO		= 0x01,
	USB_CLS_COMMUNICATIONS	= 0x02,
	USB_CLS_CDC_CONTROL	= 0x02,
	USB_CLS_HID		= 0x03,
	USB_CLS_PHYSICAL	= 0x05,
	USB_CLS_IMAGE		= 0x06,
	USB_CLS_PRINTER		= 0x07,
	USB_CLS_MASS_STORAGE	= 0x08,
	USB_CLS_HUB		= 0x09,
	USB_CLS_CDC_DATA	= 0x0a,
	USB_CLS_SMARTCARD	= 0x0b,
	USB_CLS_VIDEO		= 0x0e,
	USB_CLS_HEALTHCARE	= 0x0f,
	USB_CLS_AUDIOVIDEO	= 0x10,
	USB_CLS_BILLBOARD	= 0x11,
	USB_CLS_USB_C_BRIDGE	= 0x12,
	USB_CLS_DIAGNOSTIC	= 0xdc,
	USB_CLS_WIRELESS	= 0xe0,
	USB_CLS_MISC		= 0xef,
	USB_CLS_APP_SPECIFIC	= 0xfe,
	USB_CLS_VENDOR_SPECIFIC	= 0xff,
};

struct usb_desc_hdr {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
} __attribute__((packed));

struct usb_desc_hdr2 {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bDescriptorSubtype;
} __attribute__((packed));

struct usb_dev_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t bcdUSB;
	uint8_t  bDeviceClass;
	uint8_t  bDeviceSubClass;
	uint8_t  bDeviceProtocol;
	uint8_t  bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t  iManufacturer;
	uint8_t  iProduct;
	uint8_t  iSerialNumber;
	uint8_t  bNumConfigurations;
} __attribute__((packed));

struct usb_conf_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t wTotalLength;
	uint8_t  bNumInterfaces;
	uint8_t  bConfigurationValue;
	uint8_t  iConfiguration;
	uint8_t  bmAttributes;
	uint8_t  bMaxPower;
} __attribute__((packed));

struct usb_str_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t wString[];
} __attribute__((packed));

struct usb_intf_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bInterfaceNumber;
	uint8_t  bAlternateSetting;
	uint8_t  bNumEndpoints;
	uint8_t  bInterfaceClass;
	uint8_t  bInterfaceSubClass;
	uint8_t  bInterfaceProtocol;
	uint8_t  iInterface;
} __attribute__((packed));

struct usb_ep_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bEndpointAddress;
	uint8_t  bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t  bInterval;
} __attribute__((packed));

struct usb_cc_ep_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bEndpointAddress;
	uint8_t  bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t  bInterval;
	uint8_t  bRefresh;
	uint8_t  bSynchAddress;
} __attribute__((packed));

struct usb_intf_assoc_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bFirstInterface;
	uint8_t  bInterfaceCount;
	uint8_t  bFunctionClass;
	uint8_t  bFunctionSubClass;
	uint8_t  bFunctionProtocol;
	uint8_t  iFunction;
} __attribute__((packed));

struct usb_bos_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t wTotalLength;
	uint8_t  bNumDeviceCaps;
} __attribute__((packed));

struct usb_bos_plat_cap_hdr {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDevCapabilityType;
	uint8_t bReserved;
	uint8_t PlatformCapabilityUUID[16];
} __attribute__((packed));


// Control requests
// ----------------

struct usb_ctrl_req {
	union {
		struct {
			uint8_t  bmRequestType;
			uint8_t  bRequest;
		};
		uint16_t wRequestAndType;
	};
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} __attribute__((packed,aligned(4)));

#define USB_REQ_RCPT_MSK	(0x1f)
#define USB_REQ_RCPT(req)	((req)->bmRequestType & USB_REQ_RCPT_MSK)
#define USB_REQ_RCPT_DEV	(0 << 0)
#define USB_REQ_RCPT_INTF	(1 << 0)
#define USB_REQ_RCPT_EP		(2 << 0)
#define USB_REQ_RCPT_OTHER	(3 << 0)

#define USB_REQ_TYPE_MSK	(0x60)
#define USB_REQ_TYPE(req)	((req)->bmRequestType & USB_REQ_TYPE_MSK)
#define USB_REQ_TYPE_STD	(0 << 5)
#define USB_REQ_TYPE_CLASS	(1 << 5)
#define USB_REQ_TYPE_VENDOR	(2 << 5)
#define USB_REQ_TYPE_RSVD	(3 << 5)

#define USB_REQ_TYPE_RCPT(req)	((req)->bmRequestType & (USB_REQ_RCPT_MSK | USB_REQ_TYPE_MSK))

#define USB_REQ_READ		(1 << 7)
#define USB_REQ_IS_READ(req)	(  (req)->bmRequestType & USB_REQ_READ )
#define USB_REQ_IS_WRITE(req)	(!((req)->bmRequestType & USB_REQ_READ))

	/* wRequestAndType */
#define USB_RT_GET_STATUS_DEV		(( 0 << 8) | 0x80)
#define USB_RT_GET_STATUS_INTF		(( 0 << 8) | 0x81)
#define USB_RT_GET_STATUS_EP		(( 0 << 8) | 0x82)
#define USB_RT_CLEAR_FEATURE_DEV	(( 1 << 8) | 0x00)
#define USB_RT_CLEAR_FEATURE_INTF	(( 1 << 8) | 0x01)
#define USB_RT_CLEAR_FEATURE_EP		(( 1 << 8) | 0x02)
#define USB_RT_SET_FEATURE_DEV		(( 3 << 8) | 0x00)
#define USB_RT_SET_FEATURE_INTF		(( 3 << 8) | 0x01)
#define USB_RT_SET_FEATURE_EP		(( 3 << 8) | 0x02)
#define USB_RT_SET_ADDRESS		(( 5 << 8) | 0x00)
#define USB_RT_GET_DESCRIPTOR		(( 6 << 8) | 0x80)
#define USB_RT_SET_DESCRIPTOR		(( 7 << 8) | 0x00)
#define USB_RT_GET_CONFIGURATION	(( 8 << 8) | 0x80)
#define USB_RT_SET_CONFIGURATION	(( 9 << 8) | 0x00)
#define USB_RT_GET_INTERFACE		((10 << 8) | 0x81)
#define USB_RT_SET_INTERFACE		((11 << 8) | 0x01)
#define USB_RT_SYNCHFRAME		((12 << 8) | 0x82)
