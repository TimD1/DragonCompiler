(* ERROR: test expressions in IF/WHILE statements must be Boolean *)
program main( input, output );
  var a: integer;
begin
	a := 0;
  while (a < 10) do
    a := a + 1;
	write(a)
end.

