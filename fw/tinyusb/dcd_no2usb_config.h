/*
 * dcd_no2usb_config.h
 *
 * Example config for the no2usb core driver
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

/* Specify the core and buffer base addresses (required!) */
#define NO2USB_CORE_BASE        0x????????
#define NO2USB_DATA_RX_BASE     0x????????
#define NO2USB_DATA_TX_BASE     0x????????

/* Enable/Disable processinf Start-of-Frame events */
	/* By default SOF isn't used by most tinyusb driver and generating
	 * 1000 interrupts costs a lot of CPU cycle so disabled by default */
/* #define NO2USB_WITH_SOF 1 */

/* Enable/Disable usage of core event FIFO to optimize processing */
	/* Only enable this if the core was configured with event FIFO
	 * enabled with at least a depth of 4 */
/* #define NO2USB_WITH_EVENT_FIFO 1 */
