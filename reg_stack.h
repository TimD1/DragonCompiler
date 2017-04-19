#ifndef STACK_H
#define STACK_H

#define MAX_REGS 9

/* STRUCTURES */
typedef struct stack_s {
	int reg[MAX_REGS];
	int top_idx;
} stack_t;


/* GLOBALS */
extern stack_t* rstack;

/* FUNCTIONS */
int size(stack_t* stack);
int top(stack_t* stack);
int pop(stack_t* stack);
void swap(stack_t* stack);
void push(int reg, stack_t* stack);

char* reg_string(int reg);
void print(stack_t* stack);

#endif
