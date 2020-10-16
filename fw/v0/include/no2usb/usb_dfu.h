/*
 * usb_dfu.h
 *
 * Copyright (C) 2019-2020  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

void usb_dfu_cb_reboot(void);
bool usb_dfu_cb_flash_busy(void);
void usb_dfu_cb_flash_erase(uint32_t addr, unsigned size);			/* 4k, 32k, 64k */
void usb_dfu_cb_flash_program(const void *data, uint32_t addr, unsigned size);	/* up to 256b   */

void usb_dfu_init(void);
