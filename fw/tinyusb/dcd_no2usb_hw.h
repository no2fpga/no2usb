/*
 * dcd_no2usb_hw.h
 *
 * Hardware register description for the no2usb core
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#include "dcd_no2usb_config.h"


struct no2usb_core {
	uint32_t csr;
	uint32_t ar;
	uint32_t evt;
	uint32_t ir;
} __attribute__((packed,aligned(4)));

#define NO2USB_CSR_PU_ENA		(1 << 15)
#define NO2USB_CSR_EVT_PENDING		(1 << 14)
#define NO2USB_CSR_CEL_ACTIVE		(1 << 13)
#define NO2USB_CSR_CEL_ENA		(1 << 12)
#define NO2USB_CSR_BUS_SUSPEND		(1 << 11)
#define NO2USB_CSR_BUS_RST		(1 << 10)
#define NO2USB_CSR_BUS_RST_PENDING	(1 <<  9)
#define NO2USB_CSR_SOF_PENDING		(1 <<  8)
#define NO2USB_CSR_ADDR_MATCH		(1 <<  7)
#define NO2USB_CSR_ADDR(x)		((x) & 0x7f)

#define NO2USB_AR_CEL_RELEASE		(1 << 13)
#define NO2USB_AR_BUS_RST_CLEAR		(1 <<  9)
#define NO2USB_AR_SOF_CLEAR		(1 <<  8)

#define NO2USB_EVT_VALID		(1 << 15)		/* FIFO mode only */
#define NO2USB_EVT_OVERFLOW		(1 << 14)		/* FIFO mode only */
#define NO2USB_EVT_GET_CNT(x)		(((x) >> 12) & 0xf)	/* Count mode only */
#define NO2USB_EVT_GET_CODE(x)		(((x) >>  8) & 0xf)
#define NO2USB_EVT_GET_EP(x)		(((x) >>  4) & 0xf)
#define NO2USB_EVT_DIR_IN		(1 <<  3)
#define NO2USB_EVT_IS_SETUP		(1 <<  2)
#define NO2USB_EVT_BD_IDX		(1 <<  1)

#define NO2USB_IR_SOF_PENDING		(1 <<  5)
#define NO2USB_IR_EVT_PENDING		(1 <<  4)
#define NO2USB_IR_BUS_SUSPEND		(1 <<  3)
#define NO2USB_IR_BUS_RST_RELEASE	(1 <<  2)
#define NO2USB_IR_BUS_RST		(1 <<  1)
#define NO2USB_IR_BUS_RST_PENDING	(1 <<  0)


struct no2usb_ep {
	uint32_t status;
	uint32_t _rsvd[3];
	struct {
		uint32_t csr;
		uint32_t ptr;
	} bd[2];
} __attribute__((packed,aligned(4)));

struct no2usb_ep_pair {
	struct no2usb_ep out;
	struct no2usb_ep in;
} __attribute__((packed,aligned(4)));

#define NO2USB_EP_TYPE_NONE		0x0000
#define NO2USB_EP_TYPE_ISOC		0x0001
#define NO2USB_EP_TYPE_INT		0x0002
#define NO2USB_EP_TYPE_BULK		0x0004
#define NO2USB_EP_TYPE_CTRL		0x0006
#define NO2USB_EP_TYPE_HALTED		0x0001
#define NO2USB_EP_TYPE_IS_BCI(x)	(((x) & 6) != 0)
#define NO2USB_EP_TYPE(x)		((x) & 6)

#define NO2USB_EP_DT_BIT		0x0080
#define NO2USB_EP_BD_IDX		0x0040
#define NO2USB_EP_BD_CTRL		0x0020
#define NO2USB_EP_BD_DUAL		0x0010

#define NO2USB_BD_STATE_MSK		0xe000
#define NO2USB_BD_STATE_NONE		0x0000
#define NO2USB_BD_STATE_RDY_DATA	0x4000
#define NO2USB_BD_STATE_RDY_STALL	0x6000
#define NO2USB_BD_STATE_DONE_OK		0x8000
#define NO2USB_BD_STATE_DONE_ERR	0xa000
#define NO2USB_BD_IS_SETUP		0x1000

#define NO2USB_BD_LEN(l)		((l) & 0x3ff)
#define NO2USB_BD_LEN_MSK		0x03ff


static volatile struct no2usb_core *    const no2usb_regs    = (void*) (NO2USB_CORE_BASE);
static volatile struct no2usb_ep_pair * const no2usb_ep_regs = (void*)((NO2USB_CORE_BASE) + (1 << 13));
