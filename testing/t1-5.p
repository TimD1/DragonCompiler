(* ERROR: local objects not defined outside their scopes *)
program main( input, output );
  var a: integer;
  procedure boo(a: integer);
    var x: integer;
  begin
  end;
begin
  a := x
end.

