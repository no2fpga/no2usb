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

struct usb_cdc_dlm_desc {
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


/* Control requests */

#define USB_REQ_CDC_SEND_ENCAPSULATED_COMMAND	0x00
#define USB_REQ_CDC_GET_ENCAPSULATED_RESPONSE	0x01
#define USB_REQ_CDC_SET_COMM_FEATURE		0x02
#define USB_REQ_CDC_GET_COMM_FEATURE		0x03
#define USB_REQ_CDC_CLEAR_COMM_FEATURE		0x04
#define USB_REQ_CDC_SET_AUX_LINE_STATE		0x10
#define USB_REQ_CDC_SET_HOOK_STATE		0x11
#define USB_REQ_CDC_PULSE_SETUP			0x12
#define USB_REQ_CDC_SEND_PULSE			0x13
#define USB_REQ_CDC_SET_PULSE_TIME		0x14
#define USB_REQ_CDC_RING_AUX_JACK		0x15
#define USB_REQ_CDC_SET_LINE_CODING		0x20
#define USB_REQ_CDC_GET_LINE_CODING		0x21
#define USB_REQ_CDC_SET_CONTROL_LINE_STATE	0x22
#define USB_REQ_CDC_SEND_BREAK			0x23
#define USB_REQ_CDC_SET_RINGER_PARMS		0x30
#define USB_REQ_CDC_GET_RINGER_PARMS		0x31
#define USB_REQ_CDC_SET_OPERATION_PARMS		0x32
#define USB_REQ_CDC_GET_OPERATION_PARMS		0x33
#define USB_REQ_CDC_SET_LINE_PARMS		0x34
#define USB_REQ_CDC_GET_LINE_PARMS		0x35
#define USB_REQ_CDC_DIAL_DIGITS			0x36
#define USB_REQ_CDC_SET_UNIT_PARAMETER		0x37
#define USB_REQ_CDC_GET_UNIT_PARAMETER		0x38
#define USB_REQ_CDC_CLEAR_UNIT_PARAMETER	0x39
#define USB_REQ_CDC_GET_PROFILE			0x3A
#define USB_REQ_CDC_SET_ETH_MULTICAST_FILTERS	0x40
#define USB_REQ_CDC_SET_ETH_PWR_MGMT_PTRN_FLT	0x41
#define USB_REQ_CDC_GET_ETH_PWR_MGMT_PTRN_FLT	0x42
#define USB_REQ_CDC_SET_ETH_PACKET_FILTER	0x43
#define USB_REQ_CDC_GET_ETH_STATISTIC		0x44
#define USB_REQ_CDC_SET_ATM_DATA_FORMAT		0x50
#define USB_REQ_CDC_GET_ATM_DEVICE_STATISTICS	0x51
#define USB_REQ_CDC_SET_ATM_DEFAULT_VC		0x52
#define USB_REQ_CDC_GET_ATM_VC_STATISTICS	0x53

#define USB_RT_CDC_SEND_ENCAPSULATED_COMMAND	((0x00 << 8) | 0x21)
#define USB_RT_CDC_GET_ENCAPSULATED_RESPONSE	((0x01 << 8) | 0xa1)
#define USB_RT_CDC_SET_COMM_FEATURE		((0x02 << 8) | 0x21)
#define USB_RT_CDC_GET_COMM_FEATURE		((0x03 << 8) | 0xa1)
#define USB_RT_CDC_CLEAR_COMM_FEATURE		((0x04 << 8) | 0x21)
#define USB_RT_CDC_SET_AUX_LINE_STATE		((0x10 << 8) | 0x21)
#define USB_RT_CDC_SET_HOOK_STATE		((0x11 << 8) | 0x21)
#define USB_RT_CDC_PULSE_SETUP			((0x12 << 8) | 0x21)
#define USB_RT_CDC_SEND_PULSE			((0x13 << 8) | 0x21)
#define USB_RT_CDC_SET_PULSE_TIME		((0x14 << 8) | 0x21)
#define USB_RT_CDC_RING_AUX_JACK		((0x15 << 8) | 0x21)
#define USB_RT_CDC_SET_LINE_CODING		((0x20 << 8) | 0x21)
#define USB_RT_CDC_GET_LINE_CODING		((0x21 << 8) | 0xa1)
#define USB_RT_CDC_SET_CONTROL_LINE_STATE	((0x22 << 8) | 0x21)
#define USB_RT_CDC_SEND_BREAK			((0x23 << 8) | 0x21)
#define USB_RT_CDC_SET_RINGER_PARMS		((0x30 << 8) | 0x21)
#define USB_RT_CDC_GET_RINGER_PARMS		((0x31 << 8) | 0xa1)
#define USB_RT_CDC_SET_OPERATION_PARMS		((0x32 << 8) | 0x21)
#define USB_RT_CDC_GET_OPERATION_PARMS		((0x33 << 8) | 0xa1)
#define USB_RT_CDC_SET_LINE_PARMS		((0x34 << 8) | 0x21)
#define USB_RT_CDC_GET_LINE_PARMS		((0x35 << 8) | 0xa1)
#define USB_RT_CDC_DIAL_DIGITS			((0x36 << 8) | 0x21)
#define USB_RT_CDC_SET_UNIT_PARAMETER		((0x37 << 8) | 0x21)
#define USB_RT_CDC_GET_UNIT_PARAMETER		((0x38 << 8) | 0xa1)
#define USB_RT_CDC_CLEAR_UNIT_PARAMETER		((0x39 << 8) | 0x21)
#define USB_RT_CDC_GET_PROFILE			((0x3A << 8) | 0xa1)
#define USB_RT_CDC_SET_ETH_MULTICAST_FILTERS	((0x40 << 8) | 0x21)
#define USB_RT_CDC_SET_ETH_PWR_MGMT_PTRN_FLT	((0x41 << 8) | 0x21)
#define USB_RT_CDC_GET_ETH_PWR_MGMT_PTRN_FLT	((0x42 << 8) | 0xa1)
#define USB_RT_CDC_SET_ETH_PACKET_FILTER	((0x43 << 8) | 0x21)
#define USB_RT_CDC_GET_ETH_STATISTIC		((0x44 << 8) | 0xa1)
#define USB_RT_CDC_SET_ATM_DATA_FORMAT		((0x50 << 8) | 0x21)
#define USB_RT_CDC_GET_ATM_DEVICE_STATISTICS	((0x51 << 8) | 0xa1)
#define USB_RT_CDC_SET_ATM_DEFAULT_VC		((0x52 << 8) | 0x21)
#define USB_RT_CDC_GET_ATM_VC_STATISTICS	((0x53 << 8) | 0xa1)


/* Notifications */

#define USB_NOTIF_CDC_NETWORK_CONNECTION	0x00
#define USB_NOTIF_CDC_RESPONSE_AVAILABLE	0x01
#define USB_NOTIF_CDC_AUX_JACK_HOOK_STATE	0x08
#define USB_NOTIF_CDC_RING_DETECT		0x09
#define USB_NOTIF_CDC_SERIAL_STATE		0x20
#define USB_NOTIF_CDC_CALL_STATE_CHANGE		0x28
#define USB_NOTIF_CDC_LINE_STATE_CHANGE		0x29
#define USB_NOTIF_CDC_CONNECTION_SPEED_CHANGE	0x2A


/* Argument Structures */

struct usb_cdc_line_coding {
	uint32_t dwDTERate;
	uint8_t  bCharFormat;
	uint8_t  bParityType;
	uint8_t  bDataBits;
} __attribute__((packed));
