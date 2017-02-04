(* ERROR: procedures don't return values *)
program main( input, output );
  var a: integer;
  procedure boo(b: integer);
  begin
    a := 1
  end;
begin
  a := boo(a)
end.

