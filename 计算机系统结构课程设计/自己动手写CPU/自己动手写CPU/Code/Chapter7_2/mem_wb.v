//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2014 leishangwen@163.com                       ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Module:  mem_wb
// File:    mem_wb.v
// Author:  Lei Silei
// E-mail:  leishangwen@163.com
// Description: MEM/WB�׶εļĴ���
// Revision: 1.0
//////////////////////////////////////////////////////////////////////

`include "defines.v"

module mem_wb(

	input	wire										clk,
	input wire										rst,

  //���Կ���ģ�����Ϣ
	input wire[5:0]               stall,	

	//���Էô�׶ε���Ϣ	
	input wire[`RegAddrBus]       mem_wd,
	input wire                    mem_wreg,
	input wire[`RegBus]					 mem_wdata,
	input wire[`RegBus]           mem_hi,
	input wire[`RegBus]           mem_lo,
	input wire                    mem_whilo,	

	//�͵���д�׶ε���Ϣ
	output reg[`RegAddrBus]      wb_wd,
	output reg                   wb_wreg,
	output reg[`RegBus]					 wb_wdata,
	output reg[`RegBus]          wb_hi,
	output reg[`RegBus]          wb_lo,
	output reg                   wb_whilo		       
	
);


	always @ (posedge clk) begin
		if(rst == `RstEnable) begin
			wb_wd <= `NOPRegAddr;
			wb_wreg <= `WriteDisable;
		  wb_wdata <= `ZeroWord;	
		  wb_hi <= `ZeroWord;
		  wb_lo <= `ZeroWord;
		  wb_whilo <= `WriteDisable;	
		end else if(stall[4] == `Stop && stall[5] == `NoStop) begin
			wb_wd <= `NOPRegAddr;
			wb_wreg <= `WriteDisable;
		  wb_wdata <= `ZeroWord;
		  wb_hi <= `ZeroWord;
		  wb_lo <= `ZeroWord;
		  wb_whilo <= `WriteDisable;		  	  
		end else if(stall[4] == `NoStop) begin
			wb_wd <= mem_wd;
			wb_wreg <= mem_wreg;
			wb_wdata <= mem_wdata;
			wb_hi <= mem_hi;
			wb_lo <= mem_lo;
			wb_whilo <= mem_whilo;			
		end    //if
	end      //always
			

endmodule