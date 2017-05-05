#include <stdio.h>

int main()
{
	char a = 'c';
	char* ptr;
	a--;
	ptr = &a;
	(*ptr)++;
	fprintf(stderr, "%c\n", a);
}
