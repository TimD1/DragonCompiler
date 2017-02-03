#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"

tree_t *make_tree(int type, tree_t *left, tree_t *right)
{
	tree_t *ptr = (tree_t *)malloc(sizeof(tree_t));
	assert(ptr != NULL); // need better error handling than this!
	
	ptr->type = type;
	ptr->left = left;
	ptr-> right = right;
	
	return ptr;
}

void print_tree(tree_t *t, int spaces)
{
	if( t == NULL ) return;
	
	for(int i = 0; i < spaces; i++)
		fprintf(stderr, " ");
	
	switch( t -> type ) {
		case INUM:
			fprintf(stderr, "[INUM:%d]", t->attribute.ival);
			break;
		case RNUM:
			fprintf(stderr, "[RNUM:%f]", t->attribute.fval);
			break;
		case ADDOP:
			fprintf(stderr, "[ADDOP:%c]", t->attribute.opval);
			break;
		case MULOP:
			fprintf(stderr, "[MULOP:%c]", t->attribute.opval);
			break;
		default:
			fprintf(stderr, "[UNKNOWN]" );
			exit(1);
	}
	fprintf(stderr, "\n");
	
	print_tree(t->left, spaces+4);
	print_tree(t->right, spaces+4);
			
}

int eval_tree(tree_t *t)
{
	int lvalue, rvalue;

	assert( t != NULL );

	switch(t->type) {
		case INUM:
			return t->attribute.ival;
		case RNUM:
			return t->attribute.fval;
		case ADDOP:
			lvalue = eval_tree( t->left );
			rvalue = eval_tree( t->right );
			if(t->attribute.opval == '+') {
				return lvalue + rvalue;
			} else if (t->attribute.opval == '-') {
				return lvalue - rvalue;
			}
			else assert(0);
		case MULOP:
			lvalue = eval_tree( t->left );
			rvalue = eval_tree( t->right );
			if(t->attribute.opval == '*') {
				return lvalue * rvalue;
			} else if (t->attribute.opval == '/') {
				return lvalue / rvalue;
			}
			else assert(0);
	}
}
