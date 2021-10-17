/*
 * usb_vc_proto.h
 *
 * See USB Device Class Definition for Video Devices, release 1.1
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once


/* Interfaces Subclasses */

enum usb_vc_subclass {
	USB_VC_SCLS_UNDEFINED		= 0x00,
	USB_VC_SCLS_VIDEOCONTROL	= 0x01,
	USB_VC_SCLS_VIDEOSTREAMING	= 0x02,
	USB_VC_SCLS_COLLECTION		= 0x03,
};


/* Video Control Interfaces */

enum usb_vc_vc_intf_desc_subtype {
	USB_VC_VC_IDST_UNDEFINED	= 0x00,
	USB_VC_VC_IDST_HEADER		= 0x01,
	USB_VC_VC_IDST_INPUT_TERMINAL	= 0x02,
	USB_VC_VC_IDST_OUTPUT_TERMINAL	= 0x03,
	USB_VC_VC_IDST_SELECTOR_UNIT	= 0x04,
	USB_VC_VC_IDST_PROCESSING_UNIT	= 0x05,
	USB_VC_VC_IDST_EXTENSION_UNIT	= 0x06,
};

#define usb_vc_vc_hdr_desc_def(n) \
	struct usb_vc_vc_hdr_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptorType; \
		uint8_t  bDescriptorSubtype; \
		uint16_t bcdUVC; \
		uint16_t wTotalLength; \
		uint32_t dwClockFrequency; \
		uint8_t  bInCollection; \
		uint8_t  baInterfaceNr[n]; \
	} __attribute__ ((packed))

usb_vc_vc_hdr_desc_def(0);
#define usb_vc_vc_hdr_desc usb_vc_vc_hdr_desc__0

struct usb_vc_vc_input_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bTerminalID;
	uint16_t wTerminalType;
	uint8_t  bAssocTerminal;
	uint8_t  iTerminal;
} __attribute__ ((packed));

struct usb_vc_vc_output_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bTerminalID;
	uint16_t wTerminalType;
	uint8_t  bAssocTerminal;
	uint8_t  bSourceID;
	uint8_t  iTerminal;
} __attribute__ ((packed));

#define usb_vc_vc_camera_desc_def(n) \
	struct usb_vc_vc_camera_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bTerminalID; \
		uint16_t wTerminalType; \
		uint8_t  bAssocTerminal; \
		uint8_t  iTerminal; \
		uint16_t wObjectiveFocalLengthMin; \
		uint16_t wObjectiveFocalLengthMax; \
		uint16_t wObjectiveFocalLength; \
		uint8_t  bControlSize; \
		uint8_t  bmControls[n]; \
	} __attribute__ ((packed))

usb_vc_vc_camera_desc_def(0);
#define usb_vc_vc_camera_desc usb_vc_vc_camera_desc__0

#define usb_vc_vc_selector_desc_def(n) \
	struct usb_vc_vc_selector_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bUnitID; \
		uint8_t  bNrInPins; \
		uint8_t  baSourceID[n]; \
		uint8_t  iSelector; \
	} __attribute__ ((packed))

#define usb_vc_vc_processing_desc_def(n) \
	struct usb_vc_vc_processing_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bUnitID; \
		uint8_t  bSourceID; \
		uint16_t wMaxMultiplier; \
		uint8_t  bControlSize; \
		uint8_t  bmControls[n]; \
		uint8_t  iProcessing; \
		uint8_t  bmVideoStandards; \
	} __attribute__ ((packed))

#define usb_vc_vc_extension_desc_def(n,m) \
	struct usb_vc_vc_extension_desc__ ## n ## _ ## m { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bUnitID; \
		uint8_t  guidExtensionCode[16]; \
		uint8_t  bNumControls; \
		uint8_t  bNrInPins; \
		uint8_t  baSourceID[n]; \
		uint8_t  bControlSize; \
		uint8_t  bmControls[m]; \
		uint8_t  iExtension; \
	} __attribute__ ((packed))


/* Video Streaming Interfaces */

enum usb_vc_vs_intf_desc_subtype {
	USB_VC_VS_IDST_UNDEFINED		= 0x00,
	USB_VC_VS_IDST_INPUT_HEADER		= 0x01,
	USB_VC_VS_IDST_OUTPUT_HEADER		= 0x02,
	USB_VC_VS_IDST_STILL_IMAGE_FRAME	= 0x03,
	USB_VC_VS_IDST_FORMAT_UNCOMPRESSED	= 0x04,
	USB_VC_VS_IDST_FRAME_UNCOMPRESSED	= 0x05,
	USB_VC_VS_IDST_FORMAT_MJPEG		= 0x06,
	USB_VC_VS_IDST_FRAME_MJPEG		= 0x07,
	USB_VC_VS_IDST_FORMAT_MPEG2TS		= 0x0A,
	USB_VC_VS_IDST_FORMAT_DV		= 0x0C,
	USB_VC_VS_IDST_COLORFORMAT		= 0x0D,
	USB_VC_VS_IDST_FORMAT_FRAME_BASED	= 0x10,
	USB_VC_VS_IDST_FRAME_FRAME_BASED	= 0x11,
	USB_VC_VS_IDST_FORMAT_STREAM_BASED	= 0x12,
};

#define usb_vc_vs_input_hdr_desc_def(n) \
	struct usb_vc_vs_input_hdr_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bNumFormats; \
		uint16_t wTotalLength; \
		uint8_t  bEndpointAddress; \
		uint8_t  bmInfo; \
		uint8_t  bTerminalLink; \
		uint8_t  bStillCaptureMethod; \
		uint8_t  bTriggerSupport; \
		uint8_t  bTriggerUsage; \
		uint8_t  bControlSize; \
		uint8_t  bmaControls[n]; \
	} __attribute__ ((packed))

usb_vc_vs_input_hdr_desc_def(0);
#define usb_vc_vs_input_hdr_desc usb_vc_vs_input_hdr_desc__0

#define usb_vc_vs_output_hdr_desc_def(n) \
	struct usb_vc_vs_output_hdr_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bNumFormats; \
		uint16_t wTotalLength; \
		uint8_t  bEndpointAddress; \
		uint8_t  bTerminalLink; \
		uint8_t  bControlSize; \
		uint8_t  bmaControls[n]; \
	} __attribute__ ((packed))

usb_vc_vs_output_hdr_desc_def(0);
#define usb_vc_vs_output_hdr_desc usb_vc_vs_output_hdr_desc_def__0

struct usb_vc_vs_fmt_uncompressed_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bFormatIndex;
	uint8_t  bNumFrameDescriptors;
	uint8_t  guidFormat[16];
	uint8_t  bBitsPerPixel;
	uint8_t  bDefaultFrameIndex;
	uint8_t  bAspectRatioX;
	uint8_t  bAspectRatioY;
	uint8_t  bmInterlaceFlags;
	uint8_t  bCopyProtect;
} __attribute__ ((packed));

struct usb_vc_vs_frame_uncompressed_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint8_t  bFrameIndex;
	uint8_t  bmCapabilities;
	uint16_t wWidth;
	uint16_t wHeight;
	uint32_t dwMinBitRate;
	uint32_t dwMaxBitRate;
	uint32_t dwMaxVideoFrameBufferSize;
	uint32_t dwDefaultFrameInterval;
	uint8_t  bFrameIntervalType; /* Set to 0 for continuous */
	uint32_t dwMinFrameInterval;
	uint32_t dwMaxFrameInterval;
	uint32_t dwFrameIntervalStep;
} __attribute__ ((packed));

#define usb_vc_vs_frame_uncompressed_desc_def(n) \
	struct usb_vc_vs_frame_uncompressed_desc__ ## n { \
		uint8_t  bLength; \
		uint8_t  bDescriptortype; \
		uint8_t  bDescriptorSubtype; \
		uint8_t  bFrameIndex; \
		uint8_t  bmCapabilities; \
		uint16_t wWidth; \
		uint16_t wHeight; \
		uint32_t dwMinBitRate; \
		uint32_t dwMaxBitRate; \
		uint32_t dwMaxVideoFrameBufferSize; \
		uint32_t dwDefaultFrameInterval; \
		uint8_t  bFrameIntervalType; /* Set to 'n', the # of intervals */ \
		uint32_t dwFrameInterval[n]; \
	} __attribute__ ((packed))

usb_vc_vs_frame_uncompressed_desc_def(0);


/* Endpoints */

enum usb_vc_ep_desc_subtype {
	USB_VC_EDST_UNDEFINED	= 0x00,
	USB_VC_EDST_GENERAL	= 0x01,
	USB_VC_EDST_ENDPOINT	= 0x02,
	USB_VC_EDST_INTERRUPT	= 0x03,
};

struct usb_vc_ep_interrupt_desc {
	uint8_t  bLength;
	uint8_t  bDescriptortype;
	uint8_t  bDescriptorSubtype;
	uint16_t wMaxtransferSize;
};


/* Control requests */

#define USB_REQ_VC_SET_CUR	(0x01)
#define USB_REQ_VC_GET_CUR	(0x81)
#define USB_REQ_VC_GET_MIN	(0x82)
#define USB_REQ_VC_GET_MAX	(0x83)
#define USB_REQ_VC_GET_RES	(0x84)
#define USB_REQ_VC_GET_LEN	(0x85)
#define USB_REQ_VC_GET_INFO	(0x86)
#define USB_REQ_VC_GET_DEF	(0x87)

#define USB_RT_VC_SET_CUR_INTF	((0x01 << 8) | 0x21)
#define USB_RT_VC_GET_CUR_INTF	((0x81 << 8) | 0xa1)
#define USB_RT_VC_GET_MIN_INTF	((0x82 << 8) | 0xa1)
#define USB_RT_VC_GET_MAX_INTF	((0x83 << 8) | 0xa1)
#define USB_RT_VC_GET_RES_INTF	((0x84 << 8) | 0xa1)
#define USB_RT_VC_GET_LEN_INTF	((0x85 << 8) | 0xa1)
#define USB_RT_VC_GET_INFO_INTF	((0x86 << 8) | 0xa1)
#define USB_RT_VC_GET_DEF_INTF	((0x87 << 8) | 0xa1)

#define USB_RT_VC_SET_CUR_EP	((0x01 << 8) | 0x22)
#define USB_RT_VC_GET_CUR_EP	((0x81 << 8) | 0xa2)
#define USB_RT_VC_GET_MIN_EP	((0x82 << 8) | 0xa2)
#define USB_RT_VC_GET_MAX_EP	((0x83 << 8) | 0xa2)
#define USB_RT_VC_GET_RES_EP	((0x84 << 8) | 0xa2)
#define USB_RT_VC_GET_LEN_EP	((0x85 << 8) | 0xa2)
#define USB_RT_VC_GET_INFO_EP	((0x86 << 8) | 0xa2)
#define USB_RT_VC_GET_DEF_EP	((0x87 << 8) | 0xa2)


/* Controls */

enum usb_vc_vc_control {
	USB_VC_VC_CONTROL_UNDEFINED		= 0x00,
	USB_VC_VC_CONTROL_VIDEO_POWER_MODE	= 0x01,
	USB_VC_VC_CONTROL_REQUEST_ERROR_CODE	= 0x02,
};

enum usb_vc_terminal_control {
	USB_VC_TE_CONTROL_UNDEFINED		= 0x00,
};

enum usb_vc_selector_control {
	USB_VC_SU_CONTROL_UNDEFINED		= 0x00,
	USB_VC_SU_CONTROL_INPUT_SELECT		= 0x01,
};

enum usb_vc_camera_control {
	USB_VC_CT_CONTROL_UNDEFINED			= 0x00,
	USB_VC_CT_CONTROL_SCANNING_MODE			= 0x01,
	USB_VC_CT_CONTROL_AE_MODE			= 0x02,
	USB_VC_CT_CONTROL_AE_PRIORITY			= 0x03,
	USB_VC_CT_CONTROL_EXPOSURE_TIME_ABSOLUTE	= 0x04,
	USB_VC_CT_CONTROL_EXPOSURE_TIME_RELATIVE	= 0x05,
	USB_VC_CT_CONTROL_FOCUS_ABSOLUTE		= 0x06,
	USB_VC_CT_CONTROL_FOCUS_RELATIVE		= 0x07,
	USB_VC_CT_CONTROL_FOCUS_AUTO			= 0x08,
	USB_VC_CT_CONTROL_IRIS_ABSOLUTE			= 0x09,
	USB_VC_CT_CONTROL_IRIS_RELATIVE			= 0x0A,
	USB_VC_CT_CONTROL_ZOOM_ABSOLUTE			= 0x0B,
	USB_VC_CT_CONTROL_ZOOM_RELATIVE			= 0x0C,
	USB_VC_CT_CONTROL_PANTILT_ABSOLUTE		= 0x0D,
	USB_VC_CT_CONTROL_PANTILT_RELATIVE		= 0x0E,
	USB_VC_CT_CONTROL_ROLL_ABSOLUTE			= 0x0F,
	USB_VC_CT_CONTROL_ROLL_RELATIVE			= 0x10,
	USB_VC_CT_CONTROL_PRIVACY			= 0x11,
	USB_VC_CT_CONTROL_FOCUS_SIMPLE			= 0x12,
	USB_VC_CT_CONTROL_WINDOW			= 0x13,
	USB_VC_CT_CONTROL_REGION_OF_INTEREST		= 0x14,
};

enum usb_vc_processing_control {
	USB_VC_PU_CONTROL_UNDEFINED			= 0x00,
	USB_VC_PU_CONTROL_BACKLIGHT_COMPENSATION	= 0x01,
	USB_VC_PU_CONTROL_BRIGHTNESS			= 0x02,
	USB_VC_PU_CONTROL_CONTRAST			= 0x03,
	USB_VC_PU_CONTROL_GAIN				= 0x04,
	USB_VC_PU_CONTROL_POWER_LINE_FREQUENCY		= 0x05,
	USB_VC_PU_CONTROL_HUE				= 0x06,
	USB_VC_PU_CONTROL_SATURATION			= 0x07,
	USB_VC_PU_CONTROL_SHARPNESS			= 0x08,
	USB_VC_PU_CONTROL_GAMMA				= 0x09,
	USB_VC_PU_CONTROL_WHITE_BALANCE_TEMPERATURE	= 0x0A,
	USB_VC_PU_CONTROL_WHITE_BALANCE_TEMPERATURE_AUTO= 0x0B,
	USB_VC_PU_CONTROL_WHITE_BALANCE_COMPONENT	= 0x0C,
	USB_VC_PU_CONTROL_WHITE_BALANCE_COMPONENT_AUTO	= 0x0D,
	USB_VC_PU_CONTROL_DIGITAL_MULTIPLIER		= 0x0E,
	USB_VC_PU_CONTROL_DIGITAL_MULTIPLIER_LIMIT	= 0x0F,
	USB_VC_PU_CONTROL_HUE_AUTO			= 0x10,
	USB_VC_PU_CONTROL_ANALOG_VIDEO_STANDARD		= 0x11,
	USB_VC_PU_CONTROL_ANALOG_LOCK_STATUS		= 0x12,
	USB_VC_PU_CONTROL_CONTRAST_AUTO			= 0x13,
};

enum usb_vc_encoding_control {
	USB_VC_EU_CONTROL_UNDEFINED		= 0x00,
	USB_VC_EU_CONTROL_SELECT_LAYER		= 0x01,
	USB_VC_EU_CONTROL_PROFILE_TOOLSET	= 0x02,
	USB_VC_EU_CONTROL_VIDEO_RESOLUTION	= 0x03,
	USB_VC_EU_CONTROL_MIN_FRAME_INTERVAL	= 0x04,
	USB_VC_EU_CONTROL_SLICE_MODE		= 0x05,
	USB_VC_EU_CONTROL_RATE_CONTROL_MODE	= 0x06,
	USB_VC_EU_CONTROL_AVERAGE_BITRATE	= 0x07,
	USB_VC_EU_CONTROL_CPB_SIZE		= 0x08,
	USB_VC_EU_CONTROL_PEAK_BIT_RATE		= 0x09,
	USB_VC_EU_CONTROL_QUANTIZATION_PARAMS	= 0x0A,
	USB_VC_EU_CONTROL_SYNC_REF_FRAME	= 0x0B,
	USB_VC_EU_CONTROL_LTR_BUFFER		= 0x0C,
	USB_VC_EU_CONTROL_LTR_PICTURE		= 0x0D,
	USB_VC_EU_CONTROL_LTR_VALIDATION	= 0x0E,
	USB_VC_EU_CONTROL_LEVEL_IDC_LIMIT	= 0x0F,
	USB_VC_EU_CONTROL_SEI_PAYLOADTYPE	= 0x10,
	USB_VC_EU_CONTROL_QP_RANGE		= 0x11,
	USB_VC_EU_CONTROL_PRIORITY		= 0x12,
	USB_VC_EU_CONTROL_START_OR_STOP_LAYER	= 0x13,
	USB_VC_EU_CONTROL_ERROR_RESILIENCY	= 0x14,
};

enum usb_vc_extension_control {
	USB_VC_XU_CONTROL_UNDEFINED		= 0x00,
};

enum usb_vc_vs_control {
	USB_VC_VS_CONTROL_UNDEFINED		= 0x00,
	USB_VC_VS_CONTROL_PROBE			= 0x01,
	USB_VC_VS_CONTROL_COMMIT		= 0x02,
	USB_VC_VS_CONTROL_STILL_PROBE		= 0x03,
	USB_VC_VS_CONTROL_STILL_COMMI		= 0x04,
	USB_VC_VS_CONTROL_STILL_IMAGE_TRIGGER	= 0x05,
	USB_VC_VS_CONTROL_STREAM_ERROR_CODE	= 0x06,
	USB_VC_VS_CONTROL_GENERATE_KEY_FRAME	= 0x07,
	USB_VC_VS_CONTROL_UPDATE_FRAME_SEGMENT	= 0x08,
	USB_VC_VS_CONTROL_SYNCH_DELAY		= 0x09,
};

struct usb_vc_probe_commit {
	uint16_t bmHint;
	uint8_t  bFormatIndex;
	uint8_t  bFrameIndex;
	uint32_t dwFrameInterval;
	uint16_t wKeyFrameRate;
	uint16_t wPFrameRate;
	uint16_t wCompQuality;
	uint16_t wCompWindowSize;
	uint16_t wDelay;
	uint32_t dwMaxVideoFrameSize;
	uint32_t dwMaxPayloadTransferSize;
	uint32_t dwClockFrequency;
	uint8_t  bmFramingInfo;
	uint8_t  bPreferedVersion;
	uint8_t  bMinVersion;
	uint8_t  bMaxVersion;
} __attribute__((packed));
