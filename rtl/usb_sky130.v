/*
 * usb_sky130.v
 *
 * vim: ts=4 sw=4
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

`default_nettype none

module usb_sky130 #(
	parameter integer EVT_DEPTH = 4
)(
	// IO interface
	input  wire pad_dp_i,
	output wire pad_dp_o,
	output wire pad_dp_oe,

	input  wire pad_dn_i,
	output wire pad_dn_o,
	output wire pad_dn_oe,

	output wire pad_pu_o,
	output wire pad_pu_oe,

	// Bus interface
	input  wire [13:0] wb_addr,
	output wire [31:0] wb_rdata,
	input  wire [31:0] wb_wdata,
	input  wire        wb_we,
	input  wire        wb_cyc,
	output wire        wb_ack,

	// IRQ
	output wire irq,

	// SOF indication
	output wire sof,

	// Common
	input  wire clk,
	input  wire rst
);

	// Signals
	// -------

	// EP buffer interface
	wire [ 7:0] ep_tx_addr_0;
	wire [31:0] ep_tx_data_0;
	wire        ep_tx_we_0;

	wire [ 7:0] ep_rx_addr_0;
	wire [31:0] ep_rx_data_1;

	// Core wishbone
	wire [11:0] wb_c_addr;
	wire [15:0] wb_c_rdata;
	wire [15:0] wb_c_wdata;
	wire        wb_c_we;
	wire        wb_c_cyc;
	wire        wb_c_ack;

	// EP wishbone
	wire [31:0] wb_ep_rdata;
	wire        wb_ep_cyc;
	reg         wb_ep_ack;


	// Core
	// ----

	usb #(
		.TARGET("SKY130"),
		.EPDW(32),
		.EVT_DEPTH(4)
	) core_I (
		.pad_dp_i     (pad_dp_i),
		.pad_dp_o     (pad_dp_o),
		.pad_dp_oe    (pad_dp_oe),
		.pad_dn_i     (pad_dn_i),
		.pad_dn_o     (pad_dn_o),
		.pad_dn_oe    (pad_dn_oe),
		.pad_pu_o     (pad_pu_o),
		.pad_pu_oe    (pad_pu_oe),
		.ep_tx_addr_0 (ep_tx_addr_0),
		.ep_tx_data_0 (ep_tx_data_0),
		.ep_tx_we_0   (ep_tx_we_0),
		.ep_rx_addr_0 (ep_rx_addr_0),
		.ep_rx_data_1 (ep_rx_data_1),
		.ep_rx_re_0   (1'b1),
		.ep_clk       (clk),
		.wb_addr      (wb_c_addr),
		.wb_rdata     (wb_c_rdata),
		.wb_wdata     (wb_c_wdata),
		.wb_we        (wb_c_we),
		.wb_cyc       (wb_c_cyc),
		.wb_ack       (wb_c_ack),
		.irq          (irq),
		.sof          (sof),
		.clk          (clk),
		.rst          (rst)
	);


	// Wishbone to EP
	// --------------

	assign ep_tx_addr_0 = wb_addr[7:0];
	assign ep_rx_addr_0 = wb_addr[7:0];

	assign ep_tx_data_0 = wb_wdata;
	assign wb_ep_rdata  = wb_ep_ack ? ep_rx_data_1 : 32'h00000000;

    assign ep_tx_we_0 = wb_ep_cyc & wb_we & ~wb_ep_ack;

    always @(posedge clk)
        if (rst)
            wb_ep_ack <= 1'b0;
        else
            wb_ep_ack <= wb_ep_cyc & ~wb_ep_ack;


	// Wishbone splitter
	// -----------------

	assign wb_c_addr  = wb_addr[11:0];
	assign wb_c_wdata = wb_wdata[15:0];
	assign wb_c_we    = wb_we;

	assign wb_c_cyc   = wb_cyc & ~wb_addr[13];
	assign wb_ep_cyc  = wb_cyc &  wb_addr[13];

	assign wb_rdata   = { 16'h0000, wb_c_rdata } | wb_ep_rdata;
	assign wb_ack     = wb_c_ack | wb_ep_ack;

endmodule
