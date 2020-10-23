/*
 * usb_ac_proto.h
 *
 * See USB Device Class Definition for Audio Devices, release 1.0
 *
 * Copyright (C) 2019-2020  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once


/* Interfaces Subclasses */

enum usb_ac_subclass {
	USB_AC_SCLS_UNDEFINED		= 0x00,
	USB_AC_SCLS_AUDIOCONTROL	= 0x01,
	USB_AC_SCLS_AUDIOSTREAMING	= 0x02,
	USB_AC_SCLS_MIDISTREAMING	= 0x03,
};


/* Audio Control Interfaces */

enum usb_ac_ac_desc_subtype {
	USB_AC_DST_AC_UNDEFINED		= 0x00,
	USB_AC_DST_AC_HEADER		= 0x01,
	USB_AC_DST_AC_INPUT_TERMINAL	= 0x02,
	USB_AC_DST_AC_OUTPUT_TERMINAL	= 0x03,
	USB_AC_DST_AC_MIXER_UNIT	= 0x04,
	USB_AC_DST_AC_SELECTOR_UNIT	= 0x05,
	USB_AC_DST_AC_FEATURE_UNIT	= 0x06,
	USB_AC_DST_AC_PROCESSING_UNIT	= 0x07,
	USB_AC_DST_AC_EXTENSION_UNIT	= 0x08,
};

struct usb_ac_ac_hdr_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bDescriptorSubtype;
	uint16_t bcdADC;
	uint16_t wTotalLength;
	uint8_t  bInCollection;
	/* uint8_t  baInterfaceNr[]; */
} __attribute__ ((packed));

struct usb_ac_ac_input_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bTerminalID;
	uint16_t wTerminalType;
	uint8_t  bAssocTerminal;
	uint8_t  bNrChannels;
	uint16_t wChannelConfig;
	uint8_t  iChannelNames;
	uint8_t  iTerminal;
} __attribute__ ((packed));

struct usb_ac_ac_output_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bTerminalID;
	uint16_t wTerminalType;
	uint8_t  bAssocTerminal;
	uint8_t  bSourceID;
	uint8_t  iTerminal;
} __attribute__ ((packed));

struct usb_ac_ac_feat_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bUnitID;
	uint8_t  bSourceID;
	uint8_t  bControlSize;
	/* uint8_t  bmaControls[]; */
	/* uint8_t  iTerminal; */
} __attribute__ ((packed));


/* Audio Streaming Interfaces */

enum usb_ac_as_desc_subtype {
	USB_AC_DST_AS_UNDEFINED		= 0x00,
	USB_AC_DST_AS_GENERAL		= 0x01,
	USB_AC_DST_AS_FORMAT_TYPE	= 0x02,
	USB_AC_DST_AS_FORMAT_SPECIFIC	= 0x03,
};

struct usb_ac_as_general_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bTerminalLink;
	uint8_t  bDelay;
	uint16_t wFormatTag;
} __attribute__ ((packed));

struct usb_ac_as_fmt_type1_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bFormatType;
	uint8_t  bNrChannels;
	uint8_t  bSubframeSize;
	uint8_t  bBitResolution;
	uint8_t  bSamFreqType;
	/* uint8_t tSamFreq[]; */
} __attribute__ ((packed));


/* Endpoints */

enum usb_ac_ep_desc_subtype {
	USB_AC_DST_EP_UNDEFINED		= 0x00,
	USB_AC_DST_EP_GENERAL		= 0x01,
};

struct usb_ac_ep_general_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bmAttributes;
	uint8_t  bLockDelayUnits;
	uint16_t wLockDelay;
} __attribute__ ((packed));


/* Control requests */

#define USB_REQ_AC_SET_CUR	(0x01)
#define USB_REQ_AC_GET_CUR	(0x81)
#define USB_REQ_AC_SET_MIN	(0x02)
#define USB_REQ_AC_GET_MIN	(0x82)
#define USB_REQ_AC_SET_MAX	(0x03)
#define USB_REQ_AC_GET_MAX	(0x83)
#define USB_REQ_AC_SET_RES	(0x04)
#define USB_REQ_AC_GET_RES	(0x84)
#define USB_REQ_AC_SET_MEM	(0x05)
#define USB_REQ_AC_GET_MEM	(0x85)
#define USB_REQ_AC_GET_STAT	(0xFF)

#define USB_RT_AC_SET_CUR_INTF	((0x01 << 8) | 0x21)
#define USB_RT_AC_GET_CUR_INTF	((0x81 << 8) | 0xa1)
#define USB_RT_AC_SET_MIN_INTF	((0x02 << 8) | 0x21)
#define USB_RT_AC_GET_MIN_INTF	((0x82 << 8) | 0xa1)
#define USB_RT_AC_SET_MAX_INTF	((0x03 << 8) | 0x21)
#define USB_RT_AC_GET_MAX_INTF	((0x83 << 8) | 0xa1)
#define USB_RT_AC_SET_RES_INTF	((0x04 << 8) | 0x21)
#define USB_RT_AC_GET_RES_INTF	((0x84 << 8) | 0xa1)
#define USB_RT_AC_SET_MEM_INTF	((0x05 << 8) | 0x21)
#define USB_RT_AC_GET_MEM_INTF	((0x85 << 8) | 0xa1)
#define USB_RT_AC_GET_STAT_INTF	((0xFF << 8) | 0xa1)

#define USB_RT_AC_SET_CUR_EP	((0x01 << 8) | 0x22)
#define USB_RT_AC_GET_CUR_EP	((0x81 << 8) | 0xa2)
#define USB_RT_AC_SET_MIN_EP	((0x02 << 8) | 0x22)
#define USB_RT_AC_GET_MIN_EP	((0x82 << 8) | 0xa2)
#define USB_RT_AC_SET_MAX_EP	((0x03 << 8) | 0x22)
#define USB_RT_AC_GET_MAX_EP	((0x83 << 8) | 0xa2)
#define USB_RT_AC_SET_RES_EP	((0x04 << 8) | 0x22)
#define USB_RT_AC_GET_RES_EP	((0x84 << 8) | 0xa2)
#define USB_RT_AC_GET_STAT_EP	((0xFF << 8) | 0xa2)
