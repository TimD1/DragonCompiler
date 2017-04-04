#include <stdio.h>

int main()
{
	fprintf(stderr, "Hello World 1\n");

	char hello2[20] = "Hello World 2\n";
	fprintf(stderr, "%s", hello2);

	fprintf(stdout, "%s", "Hello World 3\n");
	fprintf(stderr, "Hello World %d", 4);

}
