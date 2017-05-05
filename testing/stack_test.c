#include <stdlib.h>
#include <stdio.h>
#include "reg_stack.h"

int main()
{
	rstack = (stack_t*)malloc(sizeof(stack_t));
	for(int i = 0; i < MAX_REGS; i++)
		rstack->reg[i] = i;
	rstack->top_idx = MAX_REGS-1;
	print(rstack);
	
	fprintf(stderr, "Swapping...\n");
	swap(rstack);
	print(rstack);
	
	fprintf(stderr, "Popping 3...\n");
	pop(rstack); pop(rstack); pop(rstack);
	print(rstack);

	fprintf(stderr, "Swapping...\n");
	swap(rstack);
	print(rstack);

	fprintf(stderr, "Pushing...\n");
	push(1, rstack); push(2, rstack); push(3, rstack);
	print(rstack);
	
	fprintf(stderr, "Swapping...\n");
	swap(rstack);
	print(rstack);
}
