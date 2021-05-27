/*
 * usb_ep_buf_tb.v
 *
 * vim: ts=4 sw=4
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

`default_nettype none
`timescale 1ns/100ps

module usb_ep_buf_tb;

	localparam integer RWIDTH = 16;	// 8/16/32
	localparam integer WWIDTH = 64;	// 8/16/32

	localparam integer ARW = 11 - $clog2(RWIDTH / 8);
	localparam integer AWW = 11 - $clog2(WWIDTH / 8);

	// Signals
	reg rst = 1;
	reg clk  = 0;

	wire [ARW-1:0] rd_addr_0;
	wire [RWIDTH-1:0] rd_data_1;
	wire rd_en_0;
	wire [AWW-1:0] wr_addr_0;
	wire [WWIDTH-1:0] wr_data_0;
	wire wr_en_0;

	// Setup recording
	initial begin
		$dumpfile("usb_ep_buf_tb.vcd");
		$dumpvars(0,usb_ep_buf_tb);
	end

	// Reset pulse
	initial begin
		# 200 rst = 0;
		# 1000000 $finish;
	end

	// Clocks
	always #10 clk  = !clk;

	// DUT
	usb_ep_buf #(
		.RWIDTH(RWIDTH),
		.WWIDTH(WWIDTH)
	) dut_I (
		.rd_addr_0(rd_addr_0),
		.rd_data_1(rd_data_1),
		.rd_en_0(rd_en_0),
		.rd_clk(clk),
		.wr_addr_0(wr_addr_0),
		.wr_data_0(wr_data_0),
		.wr_en_0(wr_en_0),
		.wr_clk(clk)
	);

	assign rd_en_0 = 1'b1;
	assign wr_en_0 = 1'b1;
	assign rd_addr_0 = 3;
	assign wr_addr_0 = 0;
	assign wr_data_0 = 64'hab89127bbabecafe;

endmodule // usb_ep_buf_tb
