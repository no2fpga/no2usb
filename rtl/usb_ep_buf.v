/*
 * usb_ep_buf.v
 *
 * vim: ts=4 sw=4
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

`default_nettype none

module usb_ep_buf #(
	parameter TARGET = "ICE40",
	parameter integer RWIDTH = 8,	// 8/16/32/64
	parameter integer WWIDTH = 8,	// 8/16/32/64
	parameter integer AWIDTH = 11,	// Assuming 'byte' access

	// auto-set
	parameter integer MWIDTH = WWIDTH / 8,
	parameter integer ARW = AWIDTH - $clog2(RWIDTH / 8),
	parameter integer AWW = AWIDTH - $clog2(WWIDTH / 8)
)(
	// Read port
	input  wire [ARW-1:0] rd_addr_0,
	output wire [RWIDTH-1:0] rd_data_1,
	input  wire rd_en_0,
	input  wire rd_clk,

	// Write port
	input  wire [AWW-1:0] wr_addr_0,
	input  wire [WWIDTH-1:0] wr_data_0,
	input  wire [MWIDTH-1:0] wr_mask_0,
	input  wire wr_en_0,
	input  wire wr_clk
);

	wire [WWIDTH-1:0] wr_mask_i_0;

	genvar i;
	generate
		for (i=0; i<WWIDTH; i=i+1)
			assign wr_mask_i_0[i] = wr_mask_0[i>>3];
	endgenerate

	ice40_ebr #(
		.READ_MODE  (4 - $clog2(RWIDTH)),
		.WRITE_MODE (4 - $clog2(WWIDTH))
	) ram_I (
		.wr_addr (wr_addr_0),
		.wr_data (wr_data_0),
		.wr_mask (wr_mask_i_0),
		.wr_ena  (wr_en_0),
		.wr_clk  (wr_clk),
		.rd_addr (rd_addr_0),
		.rd_data (rd_data_1),
		.rd_ena  (rd_en_0),
		.rd_clk  (rd_clk)
	);

endmodule // usb_ep_buf
