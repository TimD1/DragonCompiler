#ifndef TREE_H
#define TREE_H

#include "hash.h"
#define DEBUG_TREE 0


/* Semantic analyzer's channel with parser */
typedef struct tree_s {
	/* token type */
	int type;

	/* codegen tree numbering */
	int ershov_num;
	
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
void print_node(tree_t *t);
void debug_tree(tree_t *t, int spaces);
void number_tree(tree_t *t);

int type(tree_t* expr);
int empty(tree_t* node);
int leaf_node(tree_t* node);
void enforce_type(tree_t* expr, int type);
void check_types(tree_t* left, tree_t* right);
void check_args(entry_t* fn, tree_t* fn_call);
void check_function(tree_t* head_ptr, tree_t* body_ptr);
tree_t* find_return_stmt(char* fn_name, tree_t* t);

#endif
