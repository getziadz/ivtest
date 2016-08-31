timeunit 100ps;
timeprecision 10ps;

class testclass;
  task delay(output [63:0] t);
    begin
      $printtimescale(top);
      $printtimescale;
      #5ns t = $time;
    end
  endtask
endclass

timeunit 1ns;
timeprecision 1ps;

module top();

testclass tc;

reg [63:0] t1;
reg [63:0] t2;

initial begin
  $printtimescale;
  tc = new;
  tc.delay(t1);
  t2 = $time;
  $display("%0d %0d", t1, t2);
  if ((t1 === 50) && (t2 === 5))
    $display("PASSED");
  else
    $display("FAILED");
end

endmodule
