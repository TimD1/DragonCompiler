int main()
{
	long** a;
	long* b;
	long c = 7;
	long d, e;

	b = &c;
	a = &b;
	
	d = (long)*a;
	e = (long)*((long*)*b);
	
	return 0;
}
