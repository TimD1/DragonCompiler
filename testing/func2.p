program main( input, output );

  procedure sub(c: integer; d: integer);
	var e: integer;  
	begin
		e := c - d;
    	write(e)
  end;

begin
  sub(2,3)
end.
