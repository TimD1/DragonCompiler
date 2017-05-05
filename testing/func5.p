(* function with no assignment *)
program main( input, output );
  function sub(c: integer; d: integer): integer;
	begin
		write(c-d);
		sub := c - d
  end;

begin
  sub(5+4,3);
end.
