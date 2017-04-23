#include <stdlib.h>
#include <stdio.h>

#include "reg_stack.h"
#include "externs.h"

stack_t* rstack;

void print(stack_t* stack)
{
	fprintf(stderr, "\n\nREGISTER STACK\n____________________\n");
	fprintf(stderr, "POS\tID\tNAME\n");
	for(int i = 0; i <= stack->top_idx; i++)
	{
		fprintf(stderr, "%d:\treg %d\t%s\n", i, stack->reg[i], reg_string(stack->reg[i]));
	}
}

int top(stack_t* stack)
{
	return stack->reg[ stack->top_idx ];
}

void swap(stack_t* stack)
{
	if(stack->top_idx < 1)
		fprintf(stderr, "\nERROR, LINE %d: Cannot perform swap on single element stack.\n", yylineno);
	int old_top = stack->reg[ stack->top_idx ];
	int new_top = stack->reg[ stack->top_idx-1 ];
	stack->reg[ stack->top_idx ] = new_top;
	stack->reg[ stack->top_idx-1] = old_top;
}

char* reg_string(int reg_id)
{
	static char* regs[] = {"r15", "r14", "r13", "r12", "r11", "r10", "r9", "r8", "rcx"};
	
	// check register id bounds
	static char illegal_reg[] = {"rERROR"};
	if(reg_id < 0 || reg_id >= MAX_REGS)
		return illegal_reg;

	return regs[reg_id];
}

int pop(stack_t* stack)
{
	if(size(stack) <= 0)
	{
		fprintf(stderr, "\nERROR, LINE %d: Cannot pop from empty stack.\n", yylineno);
		exit(1);
	}

	int top_val = top(stack);
	stack->reg[ stack->top_idx ] = 0;
	stack->top_idx--;
	return top_val;
}

void push(int new_reg, stack_t* stack)
{
	if(size(stack) >= MAX_REGS)
	{
		fprintf(stderr, "\nERROR, LINE %d: Cannot push onto full stack.\n", yylineno);
		exit(1);
	}

	stack->top_idx++;
	stack->reg[ stack->top_idx ] = new_reg;
}

int size(stack_t* stack)
{
	return stack->top_idx + 1;
}
