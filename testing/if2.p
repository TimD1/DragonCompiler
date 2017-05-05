(* begin else within then clause *)
program main( input, output );
  var a,b: integer;
begin
	b := 5;
	read(a);
	
	if (a > b) then
		begin
			write(a);
			a := 0;
			write(a)
		end
end.
