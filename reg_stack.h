#ifndef STACK_H
#define STACK_H

#define MAX_REGS 1
#define MAX_TEMPS 50

/* STRUCTURES */
typedef struct stack_s {
	int reg[MAX_REGS];
	int temp[MAX_TEMPS];
	int top_idx;
} stack_t;


/* GLOBALS */
extern stack_t* rstack;

/* FUNCTIONS */
int top(stack_t* stack);
int pop(stack_t* stack);
void swap(stack_t* stack);
void push(int reg, stack_t* stack);

char* reg_string(int reg);
void print_regs(stack_t* stack);

#endif
