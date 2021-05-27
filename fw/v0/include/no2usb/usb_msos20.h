/*
 * usb_msos20.h
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

struct msos20_desc_set_hdr;
struct usb_bos_desc;


/* "Generic" descriptor set for simple WINUSB support */
extern const struct msos20_desc_set_hdr msos20_winusb_desc;
extern const struct usb_bos_desc        msos20_winusb_bos;

/* Register driver */
void usb_msos20_init(const struct msos20_desc_set_hdr *desc);
