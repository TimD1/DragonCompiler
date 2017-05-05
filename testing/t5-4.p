(* ERROR: function updating non-local variable *)
program main( input, output );
  var a: integer;
  
  function foo(b: integer): integer;
  begin
    foo := a;
    a := b
  end;

begin

end.

