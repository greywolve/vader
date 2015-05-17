/*
Authors: 		Zico da Silva
					Nirav Domah
					Oliver Powell
					Ayeshah Bharoochi
					
Date: 			21 May 2015

Module to recieve data that needs to be searched from a computer, 
and returns a result back to the computer, based on the success of 
finding the encrypted password or not.
*/

module Vadar(
	 input Reset,
	 input Clk,
	 output [3:0]SS_Drivers,
	 output [7:0]SS_Segments,
    input       Rx,
	 output      Tx
);

//Characters to print out to the Display to check
//for debuging purposes.
reg [3:0]First_Char;		// for SS display
reg [3:0]Second_Char;	// for SS display
reg [3:0]Third_Char;		// for SS display
reg [3:0]Fourth_Char;	// for SS display

//registers and wires for UART communication.
reg Rx_Ack = 1'b0;
wire Rx_Ready;
reg  [7:0]Tx_Data = "A";
wire [7:0]Rx_Data;
reg  Tx_Send;
wire Tx_Busy;

//Operation variables.
reg Result;
reg [127:0]Hash_value;
reg [3:0] Byte_Count = 0;


//Create seven segment driver circuit.
SS_Driver SS_Driver1(Clk, 
							Reset,
							Fourth_Char, 
							Third_Char, 
							Second_Char, 
							First_Char,
							SS_Drivers,
							SS_Segments);

//Create Uart driver circuit.
UART UART1(Reset,
			  Clk,
			  Rx_Data,
			  Rx_Ready,
			  Rx_Ack,
			  Tx_Data,
			  Tx_Send,
			  Tx_Busy,
			  Rx,
			  Tx);
			
//Create the Comparator module used to compare hashed values.
//Compare_Hash(Clk, 
//				 Reset,
//				 Hash_value,
//				 Result);

//Recieves data and then sends the same data back.
always @(posedge Clk) begin
	if(Reset) begin
		Rx_Ack <= 0;
		Tx_Data <= 0;
		Tx_Send <= 0;
	end else begin
	//receive 16 bytes from UART.
		if(Rx_Ready) begin
				if (Rx_Data == 8'h2C) begin
					Byte_Count <= 1'b1;
				end else begin
					Hash_value <= {Hash_value[119:0],Rx_Data};
					First_Char <= Rx_Data[3:0];
					Second_Char <= Rx_Data[7:4];
				end
				Rx_Ack <= 1'b1;
		end else if (~Rx_Ready) begin
			Rx_Ack <= 1'b0;
		end
	//sending a byte to the computer.
		if (~Tx_Busy & Byte_Count == 1'b1) begin
			Byte_Count <= 1'b0;
			Tx_Data <= Hash_value[7:0];
			Tx_Send <= 1'b1;
		end else if (Tx_Busy) begin
			Tx_Send <= 1'b0;
		end
	end

end


endmodule
