program main( input, output );

  procedure sub(c: integer; d: integer);
	var e: integer;  
	begin
		e := c - d;
    	write(e)
  end;
  function add(c: integer; d: integer): integer;
	var e: integer;  
	begin
		add := c + d
  end;

begin
  sub(add(2,4),3)
end.
