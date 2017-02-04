
program main( input, output );

  (* local variables *)
  var x, y: integer;
  var a, b: real;
  var c: array[ 1..10 ] of integer;
  var d: array[ 11..20 ] of real;

  (* local function: mixed argument types *)
  function foo( a: integer; x: real; z: integer ): integer;
	procedure boo( a: real );
	begin
		(* scope check on boo's a, main's b, d[] and y; and foo's z*)
		boo( a * b + d[y + z] )	
	end;
  begin
	(* boo is visible; so is main's b *)
	boo( b );	

	(* function return statement; scope and type check on foo's a and main's y *)
	foo := a + y
  end;

  (* local procedure *)
  procedure bar( c: integer );
  begin
	(* nonlocal update *)
	d[ c[foo(x,a,y)] ] := b
  end;

  (* location function: recursive *)
  function moo( d: integer ): integer;
  begin
	if ( d = 0 ) then
		moo := 1
	else
		moo := d * moo( d - 1 )
  end;

begin

  (* FUNCTION call check: recursive and correct arguments *)
  y := foo( x + foo( y, 0.001, a ) * 1, 2.3, b );
  
  (* ARRAY access check: recursive, correct arguments *)
  y := c[ x + c[y] * 45 ];

  (* FUNCTION call and ARRAY access *)
  y := foo( x + c[y + foo(c[1], d[2], a)] * 1, 2.3 + d[c[foo(c[3],b,d[y])]] );

  (* IF-THEN check *) 
  if ( c[x] * 6 < 7 + moo( y ) and a > d[c[x]] ) then
  begin
	c[moo(foo(8,9.10,11.0)) + c[12]] := moo( c[y - 1] )
  end;

  (* PROCEDURE call: correct arguments, not used as expression *)
  bar( c[x] );

  (* FOR check *)
  b := 10.0;
  for a := 1.0 to 2.0 * b do
  begin
	bar( c[x + moo( c[x + moo( c[x + moo(1)] )] )] );
	a := a + 0.0000001
  end

end.

