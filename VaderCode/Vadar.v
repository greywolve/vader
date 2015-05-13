module Vadar(
	 input Reset,
	 input Clk,

    input       Rx,
	 output      Tx
);

wire Rx_Ack = 1'b0;

reg  Tx_Data = "A";
reg  Tx_Send;
wire Tx_Busy;

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

always @(posedge Clk) begin
 if(Reset) begin
  Tx_Send <= 0;
  
 end else begin
  if(~Tx_Busy) Tx_Send <= 1'b1;
  else begin
   Tx_Send <= 1'b0;
	if(Tx_Data == "Z") Tx_Data <= Tx_Data + 1'b1;
	else               Tx_Data <= "A";
  end
 end
end


//always @(posedge Clk) begin
//	if(Reset) begin
//		Rx_Ack <= 0;
//		Tx_Data <= 0;
//		Tx_Send <= 0;
//	end else begin
//		if(Rx_Ready & ~Tx_Busy) begin
//			Tx_Data <= Rx_Data;
//			Tx_Send <= 1'b1;
//			Rx_Ack <= 1'b1;
//		end else if(~Rx_Ready & Tx_Busy) begin
//			Rx_Ack <= 1'b0;
//			Tx_Send <= 1'b0;
//		end
//	end
//
//end


endmodule
