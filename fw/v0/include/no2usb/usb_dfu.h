/*
 * usb_dfu.h
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

struct usb_dfu_zone {
	uint32_t start;
	uint32_t end;
	uint32_t flags;
};

void usb_dfu_cb_reboot(void);
bool usb_dfu_cb_flash_busy(void);
void usb_dfu_cb_flash_erase(uint32_t addr, unsigned size);			/* 4k, 32k, 64k */
void usb_dfu_cb_flash_program(const void *data, uint32_t addr, unsigned size);	/* up to 256b, page aligned */
void usb_dfu_cb_flash_read(void *data, uint32_t addr, unsigned size);		/* any addr, any length */
void usb_dfu_cb_flash_raw(void *data, unsigned len);

void usb_dfu_init(const struct usb_dfu_zone *zones, int n_zones);
