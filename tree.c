#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tree.h"
#include "y.tab.h"

tree_t *int_tree(int type, int ival, tree_t *left, tree_t *right)
{
	tree_t *ptr = (tree_t *)malloc(sizeof(tree_t));
	assert(ptr != NULL); // need better error handling than this!
	
	ptr->type = type;
	ptr->left = left;
	ptr->right = right;
	ptr->attribute.ival = ival;

	if(DEBUG_TREE)
	{
		fprintf(stderr, "\nN: %p\t\t%d", ptr, ptr->attribute.ival);
		fprintf(stderr, "\n  L: %p", ptr->left);
		fprintf(stderr, "\n  R: %p\n", ptr->right);
	}
	
	return ptr;
}


tree_t *entry_tree(int type, entry_t* pval)
{
	tree_t *ptr = (tree_t *)malloc(sizeof(tree_t));
	assert(ptr != NULL); // need better error handling than this!
	
	ptr->type = type;
	ptr->left = NULL;
	ptr->right = NULL;
	ptr->attribute.pval = pval;

	if(DEBUG_TREE)
	{
		fprintf(stderr, "\nN: %p\t\t%s", ptr, (ptr->attribute.pval)->entry_name);
		fprintf(stderr, "\n  L: %p", ptr->left);
		fprintf(stderr, "\n  R: %p\n", ptr->right);
	}
	
	return ptr;
}


tree_t *empty_tree()
{
	tree_t *ptr = (tree_t *)malloc(sizeof(tree_t));
	assert(ptr != NULL); // need better error handling than this!
	
	ptr->type = EMPTY;
	ptr->left = NULL;
	ptr->right = NULL;
	ptr->attribute.sval = " ";

	if(DEBUG_TREE)
	{
		fprintf(stderr, "\nE: %p\t\t%s", ptr, ptr->attribute.sval);
	}
	
	return ptr;
}


tree_t *float_tree(int type, float fval, tree_t *left, tree_t *right)
{
	tree_t *ptr = (tree_t *)malloc(sizeof(tree_t));
	assert(ptr != NULL); // need better error handling than this!
	
	ptr->type = type;
	ptr->left = left;
	ptr->right = right;
	ptr->attribute.fval = fval;
	
	if(DEBUG_TREE)
	{
		fprintf(stderr, "\nN: %p\t\t%f", ptr, ptr->attribute.fval);
		fprintf(stderr, "\n  L: %p", ptr->left);
		fprintf(stderr, "\n  R: %p\n", ptr->right);
	}
	
	return ptr;
}


tree_t *str_tree(int type, char* sval, tree_t *left, tree_t *right)
{
	tree_t *ptr = (tree_t *)malloc(sizeof(tree_t));
	assert(ptr != NULL); // need better error handling than this!
	
	ptr->type = type;
	ptr->left = left;
	ptr->right = right;
	ptr->attribute.sval = strdup(sval);

	if(DEBUG_TREE)
	{
		fprintf(stderr, "\nN: %p\t\t%s", ptr, ptr->attribute.sval);
		fprintf(stderr, "\n  L: %p", ptr->left);
		fprintf(stderr, "\n  R: %p\n", ptr->right);
	}
	
	return ptr;
}


tree_t *op_tree(int type, char* opval, tree_t *left, tree_t *right)
{
	tree_t *ptr = (tree_t *)malloc(sizeof(tree_t));
	assert(ptr != NULL); // need better error handling than this!
	
	ptr->type = type;
	ptr->left = left;
	ptr->right = right;
	ptr->attribute.opval = strdup(opval);

	if(DEBUG_TREE)
	{
		fprintf(stderr, "\nN: %p\t\t%s", ptr, ptr->attribute.opval);
		fprintf(stderr, "\n  L: %p", ptr->left);
		fprintf(stderr, "\n  R: %p\n", ptr->right);
	}
	
	return ptr;
}


void print_tree(tree_t *t, int spaces)
{
	if( t == NULL ) return;
	
	for(int i = 0; i < spaces; i++)
		fprintf(stderr, "| ");
	
	switch( t -> type ) {
		case INUM: 		fprintf(stderr, "[INUM %d]", t->attribute.ival); break;
		case RNUM: 		fprintf(stderr, "[RNUM %f]", t->attribute.fval); break;

		case ADDOP: 	fprintf(stderr, "[ADDOP %s]", t->attribute.opval); break;
		case MULOP: 	fprintf(stderr, "[MULOP %s]", t->attribute.opval); break;
		case RELOP: 	fprintf(stderr, "[RELOP %s]", t->attribute.opval); break;
		case ASSOP: 	fprintf(stderr, "[ASSOP %s]", t->attribute.opval); break;
		case NOT: 		fprintf(stderr, "[NOT %s]", t->attribute.opval); break;
		
		case ARRAYOP: 	fprintf(stderr, "[ARRAYOP %s]", t->attribute.opval); break;
		case PARENOP: 	fprintf(stderr, "[PARENOP %s]", t->attribute.opval); break;
		case LISTOP: 	fprintf(stderr, "[LISTOP %s]", t->attribute.opval); break;

		case IDENT: 	fprintf(stderr, "[IDENT %p]", t->attribute.pval); break;
		case STRING: 	fprintf(stderr, "[STRING %s]", t->attribute.sval); break;

		case PROGRAM: 	fprintf(stderr, "[PROGRAM %s]", t->attribute.sval); break;
		case FUNCTION: 	fprintf(stderr, "[FUNCTION %s]", t->attribute.sval); break;
		case PROCEDURE: fprintf(stderr, "[PROCEDURE %s]", t->attribute.sval); break;

		case VAR: 		fprintf(stderr, "[VAR %s]", t->attribute.sval); break;
		case ARRAY: 	fprintf(stderr, "[ARRAY %s]", t->attribute.sval); break;
		case OF: 		fprintf(stderr, "[OF %s]", t->attribute.sval); break;
		case DOTDOT: 	fprintf(stderr, "[DOTDOT %s]", t->attribute.sval); break;
		case INTEGER: 	fprintf(stderr, "[INTEGER %s]", t->attribute.sval); break;
		case REAL: 		fprintf(stderr, "[REAL %s]", t->attribute.sval); break;

		case BEG: 		fprintf(stderr, "[BEG %s]", t->attribute.sval); break;
		case END: 		fprintf(stderr, "[END %s]", t->attribute.sval); break;
		case IF: 		fprintf(stderr, "[IF %s]", t->attribute.sval); break;
		case THEN: 		fprintf(stderr, "[THEN %s]", t->attribute.sval); break;
		case ELSE: 		fprintf(stderr, "[ELSE %s]", t->attribute.sval); break;
		case DO: 		fprintf(stderr, "[DO %s]", t->attribute.sval); break;
		case WHILE: 	fprintf(stderr, "[WHILE %s]", t->attribute.sval); break;
		case FOR: 		fprintf(stderr, "[FOR %s]", t->attribute.sval); break;
		case DOWNTO: 	fprintf(stderr, "[DOWNTO %s]", t->attribute.sval); break;
		case TO: 		fprintf(stderr, "[TO %s]", t->attribute.sval); break;
		case REPEAT: 	fprintf(stderr, "[REPEAT %s]", t->attribute.sval); break;
		case UNTIL: 	fprintf(stderr, "[UNTIL %s]", t->attribute.sval); break;

		case EMPTY:		fprintf(stderr, "[EMPTY]"); break;

		default:
			fprintf(stderr, "[UNKNOWN]\n" );
			exit(1);
	}
	fprintf(stderr, "\n");
	
	print_tree(t->left, spaces+1);
	print_tree(t->right, spaces+1);
			
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
			if( !strcmp(t->attribute.opval, "+") ) {
				return lvalue + rvalue;
			} else if ( !strcmp(t->attribute.opval, "-") ) {
				return lvalue - rvalue;
			}
			else assert(0);
		case MULOP:
			lvalue = eval_tree( t->left );
			rvalue = eval_tree( t->right );
			if( !strcmp(t->attribute.opval, "*") ) {
				return lvalue * rvalue;
			} else if ( !strcmp(t->attribute.opval, "/") ) {
				return lvalue / rvalue;
			}
			else assert(0);
	}
}
