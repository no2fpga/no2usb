/*
 * usb_msos20_proto.h
 *
 * Structures defined in Microsoft OS 2.0 Descriptors specs
 * See https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-os-2-0-descriptors-specification
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

#include "usb_proto.h"


#define MSOS20_PLAT_CAP_UUID { \
	0xDF, 0x60, 0xDD, 0xD8, 0x89, 0x45, 0xC7, 0x4C, \
	0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F, \
}

#define MSOS20_WIN_VER_8_1 0x06030000

#ifndef MSOS20_MS_VENDOR_CODE
#define MSOS20_MS_VENDOR_CODE 0x4d	/* Randomly picked ... */
#endif

#define MSOS20_DESCRIPTOR_INDEX    0x07
#define MSOS20_SET_ALT_ENUMERATION 0x08


struct usb_bos_msos20_desc_set {
	uint32_t dwWindowsVersion;
	uint16_t wMSOSDescriptorSetTotalLength;
	uint8_t  bMS_VendorCode;
	uint8_t  bAltEnumCode;
}  __attribute__((packed));


enum msos20_desc_type
{
	MSOS20_SET_HEADER_DESCRIPTOR          = 0x00,
	MSOS20_SUBSET_HEADER_CONFIGURATION    = 0x01,
	MSOS20_SUBSET_HEADER_FUNCTION         = 0x02,
	MSOS20_FEATURE_COMPATBLE_ID           = 0x03,
	MSOS20_FEATURE_REG_PROPERTY           = 0x04,
	MSOS20_FEATURE_MIN_RESUME_TIME        = 0x05,
	MSOS20_FEATURE_MODEL_ID               = 0x06,
	MSOS20_FEATURE_CCGP_DEVICE            = 0x07,
	MSOS20_FEATURE_VENDOR_REVISION        = 0x08,
};

struct msos20_desc_set_hdr {		/* MS OS 2.0 descriptor set header */
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint32_t dwWindowsVersion;
	uint16_t wTotalLength;
} __attribute__((packed));

struct msos20_conf_subset_hdr {		/* MS OS 2.0 configuration subset header */
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint8_t  bConfigurationValue;
	uint8_t  bReserved;
	uint16_t wTotalLength;
} __attribute__((packed));

struct msos20_func_subset_hdr {		/* MS OS 2.0 function subset header */
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint8_t  bFirstInterface;
	uint8_t  bReserved;
	uint16_t wSubsetLength;
} __attribute__((packed));

struct msos20_feat_compat_id_desc {	/* MS OS 2.0 Compatible ID feature descriptor */
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint8_t  CompatibleID[8];
	uint8_t  SubCompatibleID[8];
} __attribute__((packed));

/* TODO: List is incomplete, add other descriptors as needed */
