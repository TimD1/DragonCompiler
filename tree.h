#ifndef TREE_H
#define TREE_H

#include "hash.h"
#define DEBUG_TREE 0


/* Semantic analyzer's channel with parser */
typedef struct tree_s {
	/* token type */
	int type;
	
	/* actual value of token */
	union {
		int ival;
		float fval; 
		char *opval;	
		char *sval;
	} attribute;

	/* children nodes */
	struct tree_s *left, *right;

} tree_t;

tree_t* int_tree(int type, int ival, tree_t *left, tree_t *right);
tree_t* float_tree(int type, float fval, tree_t *left, tree_t *right);
tree_t* op_tree(int type, char* opval, tree_t *left, tree_t *right);
tree_t* str_tree(int type, char* sval, tree_t *left, tree_t *right);
tree_t* empty_tree();

void print_tree(tree_t *t, int spaces);
int eval_tree(tree_t *t);


#endif
