/*
 * usb_crc.v
 *
 * vim: ts=4 sw=4
 *
 * Copyright (C) 2019-2021  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

`default_nettype none

module usb_crc #(
	parameter integer WIDTH = 5,
	parameter POLY  = 5'b00011,
	parameter MATCH = 5'b00000
)(
	// Input
	input  wire in_bit,
	input  wire in_first,
	input  wire in_valid,

	// Output (updated 1 cycle after input)
	output wire [WIDTH-1:0] crc,
	output wire crc_match,

	// Common
	input  wire clk,
	input  wire rst
);

	reg  [WIDTH-1:0] state;
	wire [WIDTH-1:0] state_fb_mux;
	wire [WIDTH-1:0] state_upd_mux;
	wire [WIDTH-1:0] state_nxt;

	assign state_fb_mux  = state & { WIDTH{~in_first} };
	assign state_upd_mux = (state_fb_mux[WIDTH-1] == in_bit) ? POLY : 0;
	assign state_nxt = { state_fb_mux[WIDTH-2:0], 1'b1 } ^ state_upd_mux;

	always @(posedge clk)
		if (in_valid)
			state <= state_nxt;

	assign crc_match = (state == ~MATCH);

	genvar i;
	generate
		for (i=0; i<WIDTH; i=i+1)
			assign crc[i] = state[WIDTH-1-i];
	endgenerate

endmodule // usb_crc
