/*
 * usb_ep_buf.v
 *
 * vim: ts=4 sw=4
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

`default_nettype none

module usb_ep_buf #(
	parameter TARGET = "ICE40",
	parameter integer RWIDTH = 8,	// 8/16/32/64
	parameter integer WWIDTH = 8,	// 8/16/32/64
	parameter integer AWIDTH = 11,	// Assuming 'byte' access

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
	input  wire wr_en_0,
	input  wire wr_clk
);

	// Signals
	// -------

	// Raw RAM signals
	wire        ram_web0;
	wire  [3:0] ram_wmask0;	// 1=write, 0=do not write
	wire  [7:0] ram_addr0;
	wire [31:0] ram_din0;

	wire  [7:0] ram_addr1;
	wire [31:0] ram_dout1;

	reg         ram_re1_r;
	reg  [31:0] ram_dout1_save;
	wire [31:0] ram_dout1_mux;


	// Write width adapt
	// -----------------

	generate
		if (WWIDTH == 8) begin

			// Mask writes as needed
			assign ram_web0   = ~wr_en_0;
			assign ram_wmask0 = {
				(wr_addr_0[1:0] == 2'b11),
				(wr_addr_0[1:0] == 2'b10),
				(wr_addr_0[1:0] == 2'b01),
				(wr_addr_0[1:0] == 2'b00)
			};
			assign ram_addr0  = wr_addr_0[9:2];
			assign ram_din0   = { 4{wr_data_0} };

		end else if (WWIDTH == 32) begin

			// Direct
			assign ram_web0   = ~wr_en_0;
			assign ram_wmask0 = 4'b1111;
			assign ram_addr0  = wr_addr_0;
			assign ram_din0   = wr_data_0;

		end
	endgenerate


	// RAM instance
	// ------------

	sram_1rw1r_32_256_8_sky130 ram_I (
		.clk0   (wr_clk),
		.csb0   (1'b0),
		.web0   (ram_web0),
		.wmask0 (ram_wmask0),
		.addr0  (ram_addr0),
		.din0   (ram_din0 ),
		.clk1   (rd_clk),
		.csb1   (1'b0),
		.addr1  (ram_addr1),
		.dout1  (ram_dout1)
	);


	// Read Enable simulation
	// ----------------------

		// The OpenRAM module doesn't have any "read enable"
		// and we need one ... so we need to emulate one

	always @(posedge rd_clk)
		ram_re1_r <= rd_en_0;

	always @(posedge rd_clk)
		if (ram_re1_r)
				ram_dout1_save <= ram_dout1;

	assign ram_dout1_mux = ram_re1_r ? ram_dout1 : ram_dout1_save;


	// Read width adapt
	// ----------------

	generate
		if (RWIDTH == 8) begin

			// Mux after read
			reg [1:0] lsb;

			always @(posedge rd_clk)
				if (rd_en_0)
					lsb <= rd_addr_0[1:0];

			assign ram_addr1 = rd_addr_0[9:2];
			assign rd_data_1 = ram_dout1_mux[8*lsb+:8];

		end else if (RWIDTH == 32) begin

			// Direct
			assign ram_addr1 = rd_addr_0;
			assign rd_data_1 = ram_dout1_mux;

		end
	endgenerate

endmodule // usb_ep_buf
