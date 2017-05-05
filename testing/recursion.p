program main( input, output );
  var a: integer;
function printTo(c: integer):integer;
  begin
	if ( c < 0 )
	than 
		printTo := c
	else
		begin
		  write(c);
		  printTo := printTo(c-1)
		end
  end;
begin
  a := printTo(10)
end.
