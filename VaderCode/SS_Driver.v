module SS_Driver(
 input Clk, Reset,
 
 input      [3:0]BCD3, BCD2, BCD1, BCD0,
 
 output reg [3:0]SegmentDrivers,
 output reg [7:0]SevenSegment
);
//------------------------------------------------------------------------------

wire [6:0]SS[3:0];

BCD_Decoder BCD_Decoder0(BCD0, SS[0]);
BCD_Decoder BCD_Decoder1(BCD1, SS[1]);
BCD_Decoder BCD_Decoder2(BCD2, SS[2]);
BCD_Decoder BCD_Decoder3(BCD3, SS[3]);
//------------------------------------------------------------------------------

reg [16:0]Count;

always @(posedge Clk) begin
 Count <= Count + 1'b1;
 if     ( Reset) SegmentDrivers <= 4'hE;
 else if(&Count) SegmentDrivers <= {SegmentDrivers[2:0], SegmentDrivers[3]};
end
//------------------------------------------------------------------------------

always @(*) begin
 SevenSegment[7] <= 1'b1; // Decimal point always off

 if(Reset) begin
  SevenSegment[6:0] <= 7'h7F;

 end else begin 
  case(~SegmentDrivers)
   4'h1   : SevenSegment[6:0] <= ~SS[0]; 
   4'h2   : SevenSegment[6:0] <= ~SS[1]; 
   4'h4   : SevenSegment[6:0] <= ~SS[2]; 
   4'h8   : SevenSegment[6:0] <= ~SS[3]; 
   default: SevenSegment[6:0] <=  7'h7F;
  endcase
 end
end
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------
