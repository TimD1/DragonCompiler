#include "stdio.h"

int c = 7;

int square(int a)
{
	return a*a + c;
}

int main()
{
	int b = 3;
	b = square(b);
	fprintf(stderr, "%d\n", b);
}
