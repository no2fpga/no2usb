/*
 * usb_defs.vh
 *
 * vim: ts=4 sw=4 syntax=verilog
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

localparam SYM_SE0		= 2'b00;
localparam SYM_SE1		= 2'b11;
localparam SYM_J		= 2'b10;
localparam SYM_K		= 2'b01;


localparam PID_OUT		= 4'b0001;
localparam PID_IN		= 4'b1001;
localparam PID_SOF		= 4'b0101;
localparam PID_SETUP	= 4'b1101;

localparam PID_DATA0	= 4'b0011;
localparam PID_DATA1	= 4'b1011;

localparam PID_ACK		= 4'b0010;
localparam PID_NAK		= 4'b1010;
localparam PID_STALL	= 4'b1110;

localparam PID_INVAL	= 4'b0000;
