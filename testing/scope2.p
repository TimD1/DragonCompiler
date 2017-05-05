program main( input, output );
  var A: integer;

  procedure increaseA(c: integer);
  begin
    A := c + A
  end;
begin
  A := 8;
  increaseA(5);
  write(A)
end.

