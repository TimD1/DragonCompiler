(* test tree numbering *)
program main(input, output);
    var a, b, c: integer;
    var x, y, z: integer;
    begin
		read(a);
		b := 1;
		c := 1;
		x := 2;
		y := 3;
		z := 4;
        a := a - ((b + c) - (x*y+z));
		write(a)
    end.
