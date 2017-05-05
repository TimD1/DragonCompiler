program main( input, output );
	var a: integer;
  function sub(c: integer; d: integer): integer;
	begin
		sub := c - d
  end;

begin
  a := sub(5+4,3);
  write(a)
end.
