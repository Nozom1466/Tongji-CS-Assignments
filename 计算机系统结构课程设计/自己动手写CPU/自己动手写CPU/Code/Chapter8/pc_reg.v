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
// Module:  pc_reg
// File:    pc_reg.v
// Author:  Lei Silei
// E-mail:  leishangwen@163.com
// Description: ָ��ָ��Ĵ���PC
// Revision: 1.0
//////////////////////////////////////////////////////////////////////

`include "defines.v"

module pc_reg(

	input	wire										clk,
	input wire										rst,

	//���Կ���ģ�����Ϣ
	input wire[5:0]               stall,

	//��������׶ε���Ϣ
	input wire                    branch_flag_i,
	input wire[`RegBus]           branch_target_address_i,
	
	output reg[`InstAddrBus]			pc,
	output reg                    ce
	
);

	always @ (posedge clk) begin
		if (ce == `ChipDisable) begin
			pc <= 32'h00000000;
		end else if(stall[0] == `NoStop) begin
		  	if(branch_flag_i == `Branch) begin
					pc <= branch_target_address_i;
				end else begin
		  		pc <= pc + 4'h4;
		  	end
		end
	end
	
	always @ (posedge clk) begin
		if (rst == `RstEnable) begin
			ce <= `ChipDisable;
		end else begin
			ce <= `ChipEnable;
		end
	end

endmodule