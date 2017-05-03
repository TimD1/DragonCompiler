#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "reg_stack.h"
#include "hash.h"
#include "externs.h"

stack_t* rstack;
FILE* outfile;

/*
      REGISTER STACK
   --------------------

								the value of each register is its initial index
   -------------------
   |       r8        | 7		positive-index values are stored in reg array
   -------------------			register name is given by static name list
   |       r9        | 6
   -------------------
   |       r10       | 5
   -------------------
   |       r11       | 4
   -------------------
   |       r12       | 3
   -------------------
   |       r13       | 2
   -------------------
   |       r14       | 1
   -------------------
   |       r15       | 0
-------------------------		negative-index values stored in temp array
   |      temp1      | -1		index 0 of temp array is never used
   -------------------			names are calculated based on offset from rbp
   |      temp2      | -2
   -------------------
   |      temp3      | -3
   -------------------
            .
   			.
			.
*/


/* Print all registers (including temps) */
void print_regs(stack_t* stack)
{
	fprintf(stderr, "\n\nREGISTER STACK\n____________________\n");
	fprintf(stderr, "POS\tID\tNAME\n");
	for(int i = stack->top_idx; i >= -5; i--)
	{
		if(i >= 0)
			fprintf(stderr, "%d:\treg %d\t%s\n", 
				i, stack->reg[i], reg_string(stack->reg[i]));
		else
			fprintf(stderr, "%d:\treg %d\t%s\n", 
				i, stack->temp[-i], reg_string(stack->temp[-i]));
	}
	fprintf(stderr, "\n\n");
}


/* Return the next register, if more are available.
   If not, return the next temporary */
int top(stack_t* stack)
{
	if(stack->top_idx >= 0)
		return stack->reg[ stack->top_idx ];
	else
		return stack->temp[ -stack->top_idx ];
}


/* Swap next two registers on the stack, or the last register and first temp,
   or the next two temps. */
void swap(stack_t* stack)
{
	int old_top, new_top;

	// no temps required
	if(stack->top_idx > 0)
	{
		old_top = stack->reg[ stack->top_idx ];
		new_top = stack->reg[ stack->top_idx-1 ];
		stack->reg[ stack->top_idx ] = new_top;
		stack->reg[ stack->top_idx-1] = old_top;
	}

	// swap last register with a temp
	else if(stack->top_idx == 0)
	{
		old_top = stack->reg[0];
		new_top = stack->temp[1];
		stack->reg[0] = new_top;
		stack->temp[1] = old_top;
	}

	// all temp registers
	else
	{
		old_top = stack->temp[ -stack->top_idx ];
		new_top = stack->temp[ -(stack->top_idx)+1 ];
		stack->temp[ -stack->top_idx ] = new_top;
		stack->temp[ -(stack->top_idx)+1 ] = old_top;
	}
}


/* Return the register name, or pointer to temp */
char* reg_string(int reg_id)
{
	// check upper bound of register id
	static char illegal_reg[] = {"rERROR"};
	if(reg_id >= MAX_REGS)
		return illegal_reg;

	// if legal register, return register name
	static char* regs[] = {"r15", "r14", "r13", "r12", "r11", "r10", "r9", "r8"};
	if(reg_id >= 0)
		return regs[reg_id];
	
	// otherwise, calculate and return temp register name
	else
	{
		char temp_name[100] = "";
		sprintf(temp_name, "QWORD PTR [rbp-%d]", top_table()->temp_offset-8*reg_id);
		return strdup(temp_name);
	}
}


/* Return the top register or temp from the stack */
int pop(stack_t* stack)
{
	int top_val = top(stack);
	stack->top_idx--;
	return top_val;
}


/* Push a register or temp back onto the stack */
void push(int new_reg, stack_t* stack)
{
	stack->top_idx++;
	if(stack->top_idx >= 0)
		stack->reg[ stack->top_idx ] = new_reg;
	else
		stack->temp[ -stack->top_idx ] = new_reg;
}
