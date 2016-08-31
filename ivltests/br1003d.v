timeunit 100ps;
timeprecision 10ps;

package testpackage;
  task delay(output [63:0] t);
    begin
      $printtimescale(top);
      $printtimescale;
      #5ns t = $time;
    end
  endtask
endpackage

timeunit 1ns;
timeprecision 1ps;

module top();

import testpackage::delay;

reg [63:0] t1;
reg [63:0] t2;

initial begin
  $printtimescale;
  delay(t1);
  t2 = $time;
  $display("%0d %0d", t1, t2);
  if ((t1 === 50) && (t2 === 5))
    $display("PASSED");
  else
    $display("FAILED");
end

endmodule
