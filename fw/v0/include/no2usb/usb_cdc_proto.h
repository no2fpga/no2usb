/*
 * usb_cdc_proto.h
 *
 * See USB Class Definitions for Communication Devices, Revision 1.1
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once


enum usb_cdc_subclass {
	USB_CDC_SCLS_DLCM	= 0x01,		/* Direct Line Control Model */
	USB_CDC_SCLS_ACM	= 0x02,		/* Abstract Control Model */
	USB_CDC_SCLS_TCM	= 0x03,		/* Telephone Control Model */
	USB_CDC_SCLS_MCCM	= 0x04,		/* Multi-Channel Control Model */
	USB_CDC_SCLS_CCM	= 0x05,		/* CAPI Control Model */
	USB_CDC_SCLS_ENCM	= 0x06,		/* Ethernet Networking Control Model */
	USB_CDC_SCLS_ANCM	= 0x07,		/* ATM Networking Control Model */
};

enum usb_cdc_desc_subtype {
	USB_CDC_DST_HEADER	= 0x00,
	USB_CDC_DST_CALL_MGMT	= 0x01,
	USB_CDC_DST_ACM		= 0x02,
	USB_CDC_DST_DLM		= 0x03,
	USB_CDC_DST_TEL_R	= 0x04,
	USB_CDC_DST_TEL_CAP	= 0x05,
	USB_CDC_DST_UNION	= 0x06,
	USB_CDC_DST_COUNTRY_SEL	= 0x07,
	USB_CDC_DST_TEL_OM	= 0x08,
	USB_CDC_DST_USB_TERM	= 0x09,
	USB_CDC_DST_NCT		= 0x0A,
	USB_CDC_DST_PU		= 0x0B,
	USB_CDC_DST_EU		= 0x0C,
	USB_CDC_DST_MCM		= 0x0D,
	USB_CDC_DST_CAPI	= 0x0E,
	USB_CDC_DST_ETH		= 0x0F,
	USB_CDC_DST_ATM		= 0x10,
	USB_CDC_DST_WHCM	= 0x11,
	USB_CDC_DST_MDLM	= 0x12,
	USB_CDC_DST_MDLM_DET	= 0x13,
	USB_CDC_DST_DMM		= 0x14,
	USB_CDC_DST_OBEX	= 0x15,
	USB_CDC_DST_CS		= 0x16,
	USB_CDC_DST_CS_DET	= 0x17,
	USB_CDC_DST_TEL_CM	= 0x18,
	USB_CDC_DST_OBEX_SI	= 0x19,
	USB_CDC_DST_NCM		= 0x1A
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

#define usb_cdc_union_desc_def(n) \
	struct usb_cdc_union_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptorType; \
		uint8_t  bDescriptorsubtype; \
		uint8_t  bMasterInterface; \
		uint8_t  bSlaveInterface[n]; \
	} __attribute__((packed))

usb_cdc_union_desc_def(0);
#define usb_cdc_union_desc usb_cdc_union_desc__0
