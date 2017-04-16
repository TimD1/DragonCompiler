#include <stdio.h>
int main()
{
	int a, b, c, d, e;
	a = b = c = d = e = 3;
	if(a < 10) fprintf(stderr, "1\n");
	if( a > 5 && c < 10) fprintf(stderr, "2\n");
	if( a > 0 || c > 10) fprintf(stderr, "3\n");
	if( a > 0 || (c > 10 && b < 3)) fprintf(stderr, "3\n");

	return 0;
}
