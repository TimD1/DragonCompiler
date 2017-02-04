(* ERROR: non-integer type for array index *)
program main( input, output );
  var a: array[3 .. 7] of real;
  var x: real;
begin
  a[x] := 1.23
end.

