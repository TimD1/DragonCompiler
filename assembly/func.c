int increment(int a)
{
	return a + 1;
}

int complicated_operation(int a, int b, int c, int x, int y, int z)
{
	a = a - ( (b+c) - (x*y+z) );
	return a;
}

int main()
{
	int a,b,c,x,y,z;
	a=b=c=x=y=z=2;
	increment(a);
	complicated_operation(a,b,c,x,y,z);

	return 0;
}
