module bug();

reg [1:2][1:16][1:8] array;

reg failed = 0;

integer i;

reg [3:0] index;

initial begin
  i = $bits(array);
  $display("width 0 = %0d", i);
  if (i !== 256) failed = 1;

  i = $bits(array[0]);
  $display("width 1 = %0d", i);
  if (i !== 128) failed = 1;

  i = $bits(array[0][0]);
  $display("width 2 = %0d", i);
  if (i !== 8) failed = 1;

  for (i = 0; i < 16; i++) begin
    index = i[3:0];
    array[1][5'd1+index] = {4'd0, index};
    array[2][5'd1+index] = {4'd1, index};
  end
  $display("%h", array);
  if (array !== 256'h000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f)
    failed = 1;
  for (i = 0; i < 16; i++) begin
    index = i[3:0];
    $display("%h : %h %h", index, array[1][5'd1+index], array[2][5'd1+index]);
    if (array[1][5'd1+index] !== {4'd0, index}) failed = 1;
    if (array[2][5'd1+index] !== {4'd1, index}) failed = 1;
  end
  if (failed)
    $display("FAILED");
  else
    $display("PASSED");
end

endmodule
