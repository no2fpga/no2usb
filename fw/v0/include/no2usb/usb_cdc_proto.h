/*
 * usb_cdc_proto.h
 *
 * See USB Class Definitions for Communication Devices, Revision 1.1
 *
 * Copyright (C) 2019-2020  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

enum usb_cdc_desc_subtype {
	USB_CDC_DST_HEADER	= 0x00,
	USB_CDC_DST_CALL_MGMT	= 0x01,
	USB_CDC_DST_ACM		= 0x02,
	USB_CDC_DST_DLM		= 0x03,
	USB_CDC_DST_TEL_R	= 0x04,
	USB_CDC_DST_TEL_CAP	= 0x05,
	USB_CDC_UNION		= 0x06,
	USB_CDC_COUNTRY_SEL	= 0x07,
	USB_CDC_TEL_OM		= 0x08,
	USB_CDC_USB_TERM	= 0x09,
	USB_CDC_NCT		= 0x0A,
	USB_CDC_PU		= 0x0B,
	USB_CDC_EU		= 0x0C,
	USB_CDC_MCM		= 0x0D,
	USB_CDC_CAPI		= 0x0E,
	USB_CDC_ETH		= 0x0F,
	USB_CDC_ATM		= 0x10,
	USB_CDC_WHCM		= 0x11,
	USB_CDC_MDLM		= 0x12,
	USB_CDC_MDLM_DET	= 0x13,
	USB_CDC_DMM		= 0x14,
	USB_CDC_OBEX		= 0x15,
	USB_CDC_CS		= 0x16,
	USB_CDC_CS_DET		= 0x17,
	USB_CDC_TEL_CM		= 0x18,
	USB_CDC_OBEX_SI		= 0x19,
	USB_CDC_NCM		= 0x1A
};

struct usb_cdc_hdr_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bDescriptorsubtype;
	uint16_t bcdCDC;
} __attribute__((packed));

struct usb_cdc_call_mgmt_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bDescriptorsubtype;
	uint8_t  bmCapabilities;
	uint8_t  bDataInterface;
} __attribute__((packed));

struct usb_cdc_acm_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bDescriptorsubtype;
	uint8_t  bmCapabilities;
} __attribute__((packed));

struct usb_cdc_union_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bDescriptorsubtype;
	uint8_t  bMasterInterface;
	/* uint8_t  bSlaveInterface[]; */
} __attribute__((packed));
