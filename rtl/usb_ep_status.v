/*
 * usb_ep_status.v
 *
 * vim: ts=4 sw=4
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

`default_nettype none

module usb_ep_status (
	// Priority port
	input  wire [ 8:0] p_addr_0,
	input  wire        p_read_0,
	input  wire        p_zero_0,
	input  wire        p_write_0,
	input  wire [15:0] p_din_0,
	output reg  [15:0] p_dout_3,

	// Aux R/W port
	input  wire [ 8:0] s_addr_0,
	input  wire        s_read_0,
	input  wire        s_zero_0,
	input  wire        s_write_0,
	input  wire [15:0] s_din_0,
	output reg  [15:0] s_dout_3,
	output wire        s_ready_0,

	// ROM read interface
	input  wire [ 7:0] rom_addr_0,
	output wire [15:0] rom_data_1,

	// Clock / Reset
	input  wire clk,
	input  wire rst
);

	// Shared SRAM
	// -----------

	// Signals
	wire        ram_web0;
	wire  [3:0] ram_wmask0;
	wire  [7:0] ram_addr0;
	wire [31:0] ram_din0;
	wire [31:0] ram_dout0;

	wire  [7:0] ram_addr1;
	wire [31:0] ram_dout1;

	// Instance
	sram_1rw1r_32_256_8_sky130 ram_I (
		.clk0   (clk),
		.csb0   (1'b0),
		.web0   (ram_web0),
		.wmask0 (ram_wmask0),
		.addr0  (ram_addr0),
		.din0   (ram_din0),
		.dout0  (ram_dout0),
		.clk1   (clk),
		.csb1   (1'b0),
		.addr1  (ram_addr1),
		.dout1  (ram_dout1)
	);


	// EP status
	// ---------

	// Signals
	wire s_ready_0_i;
	reg  [ 8:0] addr_1;
	reg  [15:0] din_1;
	reg  we_1;
	reg  p_read_1;
	reg  p_zero_1;
	reg  s_read_1;
	reg  s_zero_1;

	reg  addr_lsb_2;
	wire [15:0] dout_2;
	reg  p_read_2;
	reg  p_zero_2;
	reg  s_read_2;
	reg  s_zero_2;

	// "Arbitration"
	assign s_ready_0_i = ~p_read_0 & ~p_write_0;
	assign s_ready_0 = s_ready_0_i;

	// Stage 1 : Address mux and Write delay
	always @(posedge clk)
	begin
		addr_1   <= (p_read_0 | p_write_0) ? p_addr_0 : s_addr_0;
		we_1     <= p_write_0 | (s_write_0 & s_ready_0_i);
		din_1    <= p_write_0 ? p_din_0 : s_din_0;
		p_read_1 <= p_read_0;
		p_zero_1 <= p_zero_0;
		s_read_1 <= s_read_0 & s_ready_0_i;
		s_zero_1 <= s_zero_0 & s_ready_0_i;
	end

	// Stage 2 : Delays
	always @(posedge clk)
	begin
		p_read_2 <= p_read_1 | p_zero_1;
		p_zero_2 <= p_zero_1;
		s_read_2 <= s_read_1 | s_zero_1;
		s_zero_2 <= s_zero_1;
	end

	// Stage 3 : Output registers
	always @(posedge clk)
		if (p_read_2)
			p_dout_3 <= p_zero_2 ? 16'h0000 : dout_2;

	always @(posedge clk)
		if (s_read_2)
			s_dout_3 <= s_zero_2 ? 16'h0000 : dout_2;

	// RAM access
	always @(posedge clk)
		addr_lsb_2 <= addr_1[0];

	assign ram_web0   = ~we_1;
	assign ram_wmask0 = { addr_1[0], addr_1[0], ~addr_1[0], ~addr_1[0] };
	assign ram_addr0  = addr_1[8:1];
	assign ram_din0   = { din_1, din_1 };

	assign dout_2 = addr_lsb_2 ? ram_dout0[31:16] : ram_dout0[15:0];


	// ucode ROM
	// ---------

	reg rom_addr_lsb_1;

	always @(posedge clk)
		rom_addr_lsb_1 <= rom_addr_0[0];

	assign ram_addr1 = { 1'b1, rom_addr_0[7:1] };
	assign rom_data_1 = rom_addr_lsb_1 ? ram_dout1[31:16] : ram_dout1[15:0];

endmodule // usb_ep_status
