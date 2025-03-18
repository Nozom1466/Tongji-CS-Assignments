`timescale 1ns / 1ns

module Seg7x16(
    input Clk,
	input Reset,
	input Cs,
	input [31:0] I_Data,
	output [7:0] O_Seg,
	output [7:0] O_Sel
    ); 

    reg [14:0] Cnt; 
	 always @ (posedge Clk, posedge Reset)
      if (Reset)
        Cnt <= 0; 
      else
        Cnt <= Cnt + 1'B1; 
 
    wire Seg7_Clk = Cnt[14];  
	 
	 reg [2:0] Seg7_Addr; 
	 
	 always @ (posedge Seg7_Clk, posedge Reset)
	   if(Reset)
		  Seg7_Addr <= 0; 
		else
		  Seg7_Addr <= Seg7_Addr + 1'B1; 
		  
	 reg [7:0] O_Sel_R; 
	 
	 always @ (*)
	   case(Seg7_Addr)
		  7 : O_Sel_R = 8'B01111111; 
		  6 : O_Sel_R = 8'B10111111; 
		  5 : O_Sel_R = 8'B11011111; 
		  4 : O_Sel_R = 8'B11101111; 
		  3 : O_Sel_R = 8'B11110111; 
		  2 : O_Sel_R = 8'B11111011; 
		  1 : O_Sel_R = 8'B11111101; 
		  0 : O_Sel_R = 8'B11111110; 
		endcase
	
	 reg [31:0] I_Data_Store; 
	 always @ (posedge Clk, posedge Reset)
	   if(Reset)
		  I_Data_Store <= 0; 
		else if(Cs)
		  I_Data_Store <= I_Data; 
		  
	 reg [7:0] Seg_Data_R; 
	 always @ (*)
	   case(Seg7_Addr)
		  0 : Seg_Data_R = I_Data_Store[3:0]; 
		  1 : Seg_Data_R = I_Data_Store[7:4]; 
		  2 : Seg_Data_R = I_Data_Store[11:8]; 
		  3 : Seg_Data_R = I_Data_Store[15:12]; 
		  4 : Seg_Data_R = I_Data_Store[19:16]; 
		  5 : Seg_Data_R = I_Data_Store[23:20]; 
		  6 : Seg_Data_R = I_Data_Store[27:24]; 
		  7 : Seg_Data_R = I_Data_Store[31:28]; 
		endcase
	 
	 reg [7:0] O_Seg_R; 
	 always @ (posedge Clk, posedge Reset)
	   if(Reset)
		  O_Seg_R <= 8'Hff; 
		else
		  case(Seg_Data_R)
		    4'H0 : O_Seg_R <= 8'HC0; 
          4'H1 : O_Seg_R <= 8'HF9; 
          4'H2 : O_Seg_R <= 8'HA4; 
          4'H3 : O_Seg_R <= 8'HB0; 
          4'H4 : O_Seg_R <= 8'H99; 
          4'H5 : O_Seg_R <= 8'H92; 
          4'H6 : O_Seg_R <= 8'H82; 
          4'H7 : O_Seg_R <= 8'HF8; 
          4'H8 : O_Seg_R <= 8'H80; 
          4'H9 : O_Seg_R <= 8'H90; 
          4'HA : O_Seg_R <= 8'H88; 
          4'HB : O_Seg_R <= 8'H83; 
          4'HC : O_Seg_R <= 8'HC6; 
          4'HD : O_Seg_R <= 8'HA1; 
          4'HE : O_Seg_R <= 8'H86; 
          4'HF : O_Seg_R <= 8'H8E; 
		  endcase
		  
	 assign O_Sel = O_Sel_R; 
	 assign O_Seg = O_Seg_R; 

endmodule
