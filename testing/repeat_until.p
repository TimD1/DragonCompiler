(* ERROR: test expressions in IF/WHILE statements must be Boolean *)
program main( input, output );
  var a: integer;
begin
	a := 0;
  repeat a := a + 1 until (a > 10);
	write(a)
end.

