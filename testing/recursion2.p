program main( input, output );
  var a: integer;
function factorial(c: integer):integer;
  begin
	if (c = 1) than
		factorial := c
	else
		factorial := c * factorial(c-1)
  end;
begin
  a := factorial(5);
  write(a)
end.
