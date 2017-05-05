(* ERROR: variable redeclared *)
program main( input, output );
  var a: integer;

  procedure boo;
  begin
  end;

  function boo(b:integer):integer;
  begin
	boo := 1
  end;

begin
end.

