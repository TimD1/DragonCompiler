program main( input, output );
  var a,b: integer;

  function display(c: integer): integer;
  begin
    write(c);
	display := c + 1
  end;
begin
  a := display(10);
  write(a)
end.

