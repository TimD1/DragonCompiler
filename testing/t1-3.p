(* LEGAL: non-local names visible from inner scopes *)
program main( input, output );
  var a: integer;
  procedure boo(x: integer);
  begin
    a := x
  end;
begin
  boo(a)
end.

