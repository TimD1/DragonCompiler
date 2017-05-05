(* ERROR: procedure passed wrong number/type of arguments *)
program main( input, output );
  var b: integer;
  var y: real;

  procedure boo(a: integer; x: real);
  begin
  end;
begin
  boo(y,10);
  boo(b,y,b)
end.

