int add2(int a, int b) { return a + b; }
int add4(int a, int b, int c, int d) { return a + b + c + d; }
int add6(int a, int b, int c, int d, int e, int f) { return a + b + c + d + e + f; }
int add8(int a, int b, int c, int d, int e, int f, int g, int h) 
{ return a + b + c + d + e + f + g + h; }
int add10(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j) 
{ return a + b + c + d + e + f + g + h + i + j; }

int main()
{
	int a, b, c, d, e, f, g, h, i, j;
	a = b = c = d = e = f = g = h = i = j = 1;
	add2(a,b);
	add4(a,b,c,d);
	add6(a,b,c,d,e,f);
	add8(a,b,c,d,e,f,g,h);
	add10(a,b,c,d,e,f,g,h,i,j);

	return 0;
}
