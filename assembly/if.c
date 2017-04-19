#include <stdio.h>
int main()
{
	long long int a, b;
	fscanf(stdin, "%lld", &a);
	if( b = a < 5) fprintf(stderr, "%lld\n", a);

	/* int a, b, c, d, e; */
	/* a = b = c = d = e = 3; */
	/* if(c = a < 10) fprintf(stderr, "1\n"); */
	/* if( a > 5 && c < 10) fprintf(stderr, "2\n"); */
	/* if( a > 0 || c > 10) fprintf(stderr, "3\n"); */
	/* if( a > 0 || (c > 10 && b < 3)) fprintf(stderr, "3\n"); */

	return 0;
}
