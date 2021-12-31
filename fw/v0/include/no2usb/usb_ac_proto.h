/*
 * usb_ac_proto.h
 *
 * See USB Device Class Definition for Audio Devices, release 1.0
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
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

enum usb_ac_ac_intf_desc_subtype {
	USB_AC_AC_IDST_UNDEFINED	= 0x00,
	USB_AC_AC_IDST_HEADER		= 0x01,
	USB_AC_AC_IDST_INPUT_TERMINAL	= 0x02,
	USB_AC_AC_IDST_OUTPUT_TERMINAL	= 0x03,
	USB_AC_AC_IDST_MIXER_UNIT	= 0x04,
	USB_AC_AC_IDST_SELECTOR_UNIT	= 0x05,
	USB_AC_AC_IDST_FEATURE_UNIT	= 0x06,
	USB_AC_AC_IDST_PROCESSING_UNIT	= 0x07,
	USB_AC_AC_IDST_EXTENSION_UNIT	= 0x08,
};

#define usb_ac_ac_hdr_desc_def(n) \
	struct usb_ac_ac_hdr_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptorType; \
		uint8_t  bDescriptorSubtype; \
		uint16_t bcdADC; \
		uint16_t wTotalLength; \
		uint8_t  bInCollection; \
		uint8_t  baInterfaceNr[n]; \
	} __attribute__ ((packed))

usb_ac_ac_hdr_desc_def(0);
#define usb_ac_ac_hdr_desc usb_ac_ac_hdr_desc_def__0

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

#define usb_ac_ac_mixer_desc_def(n, m) \
	struct usb_ac_ac_mixer_desc__ ## n ## _ ## m { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bUnitID; \
		uint8_t  bNrInPins; \
		uint8_t  baSourceID[n]; \
		uint8_t  bNrChannels; \
		uint16_t wChannelConfig; \
		uint8_t  iChannelNames; \
		uint8_t  bmControls[m]; \
		uint8_t  iMixer \
	} __attribute__ ((packed))

#define usb_ac_ac_selector_desc_def(n) \
	struct usb_ac_ac_selector_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bUnitID; \
		uint8_t  bNrInPins; \
		uint8_t  baSourceID[n]; \
		uint8_t  iSelector; \
	} __attribute__ ((packed))

#define usb_ac_ac_feature_desc_def(n) \
	struct usb_ac_ac_feature_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bUnitID; \
		uint8_t  bSourceID; \
		uint8_t  bControlSize; \
		uint8_t  bmaControls[n]; \
		uint8_t  iFeature; \
	} __attribute__ ((packed))

#define usb_ac_ac_processing_common_desc_def(n, m) \
	struct usb_ac_ac_processing_common_desc__ ## n ## _ ## m { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bUnitID; \
		uint16_t wProcessType; \
		uint8_t  bNrInPins; \
		uint8_t  baSourceID[n]; \
		uint8_t  bNrChannels; \
		uint16_t wChannelConfig; \
		uint8_t  iChannelNames; \
		uint8_t  bControlSize; \
		uint8_t  bmControls[m]; \
		uint8_t  iProcessing; \
	} __attribute__ ((packed))

#define usb_ac_ac_processing_updown_desc_def(n) \
	struct usb_ac_ac_processing_updown_desc__ ## n { \
		uint8_t  bNrModes; \
		uint16_t waModes; \
	} __attribute__ ((packed))

#define usb_ac_ac_processing_dolby_desc_def(n) \
	struct usb_ac_ac_processing_dolby_desc__ ## n { \
		uint8_t  bNrModes; \
		uint16_t waModes; \
	} __attribute__ ((packed))

#define usb_ac_ac_extension_desc_def(n, m) \
	struct usb_ac_ac_extension_desc__ ## n ## _ ## m { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bUnitID; \
		uint16_t wExtensionCode; \
		uint8_t  bNrInPins; \
		uint8_t  baSourceID[n]; \
		uint8_t  bNrChannels; \
		uint16_t wChannelConfig; \
		uint8_t  iChannelNames; \
		uint8_t  bControlSize; \
		uint8_t  bmControls[m]; \
		uint8_t  iExtension; \
	} __attribute__ ((packed))


/* Audio Streaming Interfaces */

enum usb_ac_as_intf_desc_subtype {
	USB_AC_AS_IDST_UNDEFINED	= 0x00,
	USB_AC_AS_IDST_GENERAL		= 0x01,
	USB_AC_AS_IDST_FORMAT_TYPE	= 0x02,
	USB_AC_AS_IDST_FORMAT_SPECIFIC	= 0x03,
};

struct usb_ac_as_general_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bTerminalLink;
	uint8_t  bDelay;
	uint16_t wFormatTag;
} __attribute__ ((packed));

#define usb_ac_as_fmt_type1_desc_def(n) \
	struct usb_ac_as_fmt_type1_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bFormatType; \
		uint8_t  bNrChannels; \
		uint8_t  bSubframeSize; \
		uint8_t  bBitResolution; \
		uint8_t  bSamFreqType; \
		uint8_t tSamFreq[n];	/* 3 byte per frequency, uint24_t */ \
	} __attribute__ ((packed))

usb_ac_as_fmt_type1_desc_def(0);
#define usb_ac_as_fmt_type1_desc usb_ac_as_fmt_type1_desc__0


/* MIDI Streaming Interfaces */

enum usb_ac_ms_intf_desc_subtype {
	USB_AC_MS_IDST_UNDEFINED	= 0x00,
	USB_AC_MS_IDST_HEADER		= 0x01,
	USB_AC_MS_IDST_MIDI_IN_JACK	= 0x02,
	USB_AC_MS_IDST_MIDI_OUT_JACK	= 0x03,
	USB_AC_MS_IDST_ELEMENT		= 0x04,
};

struct usb_ac_ms_hdr_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bDescriptorSubtype;
	uint16_t bcdADC;
	uint16_t wTotalLength;
} __attribute__ ((packed));

struct usb_ac_ms_in_jack_desc {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bDescriptorSubtype;
	uint8_t  bJackType;
	uint8_t  bJackID;
	uint8_t  iJack;
} __attribute__ ((packed));

#define usb_ac_ms_out_jack_desc_def(n) \
	struct usb_ac_ms_out_jack_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptorType; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bJackType; \
		uint8_t  bJackID; \
		uint8_t  bNrInputPins; \
		struct { \
			uint8_t  baSourceID; \
			uint8_t  baSourcePin; \
		} __attribute__ ((packed)) sources[n]; \
		uint8_t  iJack; \
	} __attribute__ ((packed))

#define usb_ac_ms_elem_desc_def(n) \
	struct usb_ac_ms_elem_desc { \
		uint8_t  bLength; \
		uint8_t  bDescriptorType; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bElementID; \
		uint8_t  bNrInputPins; \
		struct { \
			uint8_t  baSourceID; \
			uint8_t  baSourcePin; \
		} __attribute__ ((packed)) sources[n]; \
		uint8_t  bNrOutputPins; \
		uint8_t  bInTerminalLink; \
		uint8_t  bOutTerminalLink; \
		uint8_t  bElCapsSize;	/* Must be =2 since we fix bmElementCaps to uint16_t */ \
		uint16_t bmElementCaps;	/* fixed to 16 bits */ \
		uint8_t  iElement; \
	} __attribute__ ((packed))

enum usb_ac_ms_jack_types {
	USB_AC_MS_JACK_TYPE_UNDEFINED	= 0x00,
	USB_AC_MS_JACK_TYPE_EMBEDDED	= 0x01,
	USB_AC_MS_JACK_TYPE_EXTERNAL	= 0x02,
};


/* Endpoints */

enum usb_ac_ep_desc_subtype {
	USB_AC_EDST_UNDEFINED	= 0x00,
	USB_AC_EDST_GENERAL	= 0x01,
};

struct usb_ac_as_ep_general_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bmAttributes;
	uint8_t  bLockDelayUnits;
	uint16_t wLockDelay;
} __attribute__ ((packed));

#define usb_ac_ms_ep_general_desc_def(n) \
	struct usb_ac_ms_ep_general_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bNumEmbMIDIJack; \
		uint8_t  baAssocJackID[n]; \
	} __attribute__ ((packed))

usb_ac_ms_ep_general_desc_def(0);
#define usb_ac_ms_ep_general_desc usb_ac_ms_ep_general_desc__0


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


/* Controls */

enum usb_ac_terminal_control {
	USB_AC_TE_CONTROL_UNDEFINED		= 0x00,
	USB_AC_TE_CONTROL_COPY_PROTECT		= 0x01,
};

enum usb_ac_feature_control {
        USB_AC_FU_CONTROL_UNDEFINED		= 0x00,
        USB_AC_FU_CONTROL_MUTE			= 0x01,
        USB_AC_FU_CONTROL_VOLUME		= 0x02,
        USB_AC_FU_CONTROL_BASS			= 0x03,
        USB_AC_FU_CONTROL_MID			= 0x04,
        USB_AC_FU_CONTROL_TREBLE		= 0x05,
        USB_AC_FU_CONTROL_GRAPHIC_EQUALIZER	= 0x06,
        USB_AC_FU_CONTROL_AUTOMATIC_GAIN	= 0x07,
        USB_AC_FU_CONTROL_DELAY			= 0x08,
        USB_AC_FU_CONTROL_BASS_BOOST		= 0x09,
        USB_AC_FU_CONTROL_LOUDNESS		= 0x0A,
};

enum usb_ac_updown_control {
	USB_AC_UD_CONTROL_UNDEFINED		= 0x00,
	USB_AC_UD_CONTROL_ENABLE		= 0x01,
	USB_AC_UD_CONTROL_MODE_SELECT		= 0x02,
};

enum usb_ac_dolby_control {
	USB_AC_DP_CONTROL_UNDEFINED		= 0x00,
	USB_AC_DP_CONTROL_ENABLE		= 0x01,
	USB_AC_DP_CONTROL_MODE_SELECT		= 0x02,
};

enum usb_ac_3d_control {
	USB_AC_3D_CONTROL_UNDEFINED		= 0x00,
	USB_AC_3D_CONTROL_ENABLE		= 0x01,
	USB_AC_3D_CONTROL_SPACIOUSNESS		= 0x03,
};

enum usb_ac_reverb_control {
	USB_AC_RV_CONTROL_UNDEFINED		= 0x00,
	USB_AC_RV_CONTROL_ENABLE		= 0x01,
	USB_AC_RV_CONTROL_LEVEL			= 0x02,
	USB_AC_RV_CONTROL_TIME			= 0x03,
	USB_AC_RV_CONTROL_FEEDBACK		= 0x04,
};

enum usb_ac_chorus_control {
	USB_AC_CH_CONTROL_UNDEFINED		= 0x00,
	USB_AC_CH_CONTROL_ENABLE		= 0x01,
	USB_AC_CH_CONTROL_LEVEL			= 0x02,
	USB_AC_CH_CONTROL_RATE			= 0x03,
	USB_AC_CH_CONTROL_DEPTH			= 0x04,
};

enum usb_ac_dynrange_control {
	USB_AC_DR_CONTROL_UNDEFINED		= 0x00,
	USB_AC_DR_CONTROL_ENABLE		= 0x01,
	USB_AC_DR_CONTROL_COMPRESSION_RATE	= 0x02,
	USB_AC_DR_CONTROL_MAXAMPL		= 0x03,
	USB_AC_DR_CONTROL_THRESHOLD		= 0x04,
	USB_AC_DR_CONTROL_ATTACK_TIME		= 0x05,
	USB_AC_DR_CONTROL_RELEASE_TIME		= 0x06,
};

enum usb_ac_extension_control {
	USB_AC_XU_CONTROL_UNDEFINED		= 0x00,
	USB_AC_XU_CONTROL_ENABLE		= 0x01,
};

enum usb_ac_vs_endpoint_control {
	USB_AC_VS_EP_CONTROL_UNDEFINED		= 0x00,
	USB_AC_VS_EP_CONTROL_SAMPLING_FREQ	= 0x01,
	USB_AC_VS_EP_CONTROL_PITCH		= 0x02,
};

enum usb_ac_ms_endpoint_control {
	USB_AC_MS_EP_CONTROL_UNDEFINED		= 0x00,
	USB_AC_MS_EP_CONTROL_ASSOCIATION	= 0x01,
};
