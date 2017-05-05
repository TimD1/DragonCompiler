(* ERROR: function passed wrong number/type of arguments *)
program main( input, output );
  var b: integer;
  var y: real;
  var a: array[1 .. 13] of integer;
  var z: array[1 .. 13] of real;

  function foo(a: integer; x: real): integer;
  begin
    foo := a
  end;
begin
  b := foo(y,b) + foo(b,y,10)
end.




