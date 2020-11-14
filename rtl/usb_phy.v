/*
 * usb_phy.v
 *
 * vim: ts=4 sw=4
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

`default_nettype none

module usb_phy #(
	parameter TARGET = "ICE40"
)(
	// IO interface
	input  wire pad_dp_i,
	output reg  pad_dp_o,
	output reg  pad_dp_oe,

	input  wire pad_dn_i,
	output reg  pad_dn_o,
	output reg  pad_dn_oe,

	// RX
	output wire rx_dp,
	output wire rx_dn,
	output wire rx_chg,

	// TX
	input  wire tx_dp,
	input  wire tx_dn,
	input  wire tx_en,

	// Common
	input  wire clk,
	input  wire rst
);

	reg  [2:0] rx_dp_i;
	reg  [2:0] rx_dn_i;

	// Output registers
	always @(posedge clk)
	begin
		pad_dp_o  <= tx_dp;
		pad_dp_oe <= tx_en;
		pad_dn_o  <= tx_dn;
		pad_dn_oe <= tx_en;
	end

	// Input register / synchronizer
	always @(posedge clk)
	begin
		rx_dp_i <= { rx_dp_i[1:0], pad_dp_i };
		rx_dn_i <= { rx_dn_i[1:0], pad_dn_i };
	end

	assign rx_dp  = rx_dp_i[1];
	assign rx_dn  = rx_dn_i[1];
	assign rx_chg = ^rx_dp_i[2:1] | ^rx_dn_i[2:1];

endmodule // usb_phy
