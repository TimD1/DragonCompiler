program main( input, output );
  var A: integer;

  function increaseA(c: integer):integer;
  begin
	(* the following line is illegal *)
    A := c + A;
	increaseA := c + A
  end;
begin
  A := 8;
  A := increaseA(5);
  write(A)
end.

