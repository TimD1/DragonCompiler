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

	if(spaces == 0)
		fprintf(stderr, "\n\n\nSYNTAX TREE\n___________\n\n");
	
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

		case IDENT: 	fprintf(stderr, "[IDENT %s]", t->attribute.sval); break;
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


/* Given a pointer to a node in the syntax tree (within an expression),
   return the type of the node, recursively checking children and printing 
   the first semantic error found, without recovery. */
int type(tree_t* t)
{
	entry_t *var_ptr, *fn_ptr, *arr_ptr;
	switch(t->type)
	{
	case INUM:
	case RNUM:
	case STRING:
	case EMPTY:
		return t->type;
		break;

	case NOT:
		// check that correct type is used
		if(type(t->left) != INUM)
		{
			fprintf(stderr, "\nERROR: 'NOT' must take a boolean (integer) argument.\n");
			exit(0);
		}
		
		// everything is fine
		return INUM;
		break;


	case IDENT:
		var_ptr = find_entry(top_table(), t->attribute.sval);
		
		// check that variable exists
		if(var_ptr == NULL)
		{
			fprintf(stderr, "\nERROR: variable '%s' must be declared before it may be used.\n", t->attribute.sval);
			exit(0);
		}

		// everything is fine
		return var_ptr->return_type;
		break;
	

	case MULOP:
	case RELOP:
	case ADDOP:
		check_types(t->left, t->right);	

		// everything is fine
		return type(t->left);
		break;


	case PARENOP:
		fn_ptr = find_entry(top_table(), t->left->attribute.sval);
		
		// check that function exists
		if(fn_ptr == NULL)
		{
			fprintf(stderr, "\nERROR: function/procedure '%s' must be declared before it may be used.\n", t->left->attribute.sval);
			exit(0);
		}

		// must be a function, since procedures don't return values
		if(fn_ptr->entry_class == PROCEDURE)
		{
			fprintf(stderr, "\nERROR: procedure '%s' does not return a value.\n", 
				t->left->attribute.sval);
			exit(0);
		}

		// cover all other cases 
		if(fn_ptr->entry_class != FUNCTION)
		{
			fprintf(stderr, "\nERROR: '%s' is not a function.\n", 
				t->left->attribute.sval);
			exit(0);
		}

		// check that all arguments are of the correct type
		check_args(fn_ptr, t->right);
		
		// everything is fine
		return fn_ptr->return_type;
		break;


	case ARRAYOP:
		arr_ptr = find_entry(top_table(), t->left->attribute.sval);
		
		// check that variable exists
		if(arr_ptr == NULL)
		{
			fprintf(stderr, "\nERROR: array '%s' must be declared before it may be used.\n", t->left->attribute.sval);
			exit(0);
		}
		
		// check that this variable is an array
		if(arr_ptr->entry_class != ARRAY)
		{
			fprintf(stderr, "\nERROR: '%s' is not an array.\n", t->left->attribute.sval);
			exit(0);
		}

		// only integer indices are legal
		if(type(t->right) != INUM)
		{
			fprintf(stderr, "\nERROR: array '%s' must use integer indices.\n", 
				arr_ptr->entry_name);
			exit(0);
		}

		// everything is fine
		return arr_ptr->return_type;
		break;


	default:
		fprintf(stderr, "\nERROR: unexpected tree node type encountered.\n"); 
		exit(0);
			
	}
}


/* Given pointers to two nodes in syntax tree, make sure both types are same */
void check_types(tree_t* left, tree_t* right)
{
	// check that types match
	if(type(left) != type(right))
	{
		fprintf(stderr, "\nERROR: type mismatch, '%s' and '%s'\n", 
			type_string(type(left)), type_string(type(right)));
		exit(0);
	}
}


/* Given a pointer to a function call in the syntax tree, and a pointer to the function's entry in the symbol table, check that it was called correctly. */
void check_args(entry_t* fn, tree_t* fn_call)
{
	// check function has correct number of args
	if(count_args(fn_call) != fn->arg_num)
	{
		fprintf(stderr, "\nERROR: function/procedure '%s' called with %d arguments instead of %d.\n", fn->entry_name, count_args(fn_call), fn->arg_num);
		exit(0);
	}

	for(int i = 0; i < fn->arg_num; i++)
	{
		if(type(fn_call->right) != fn->arg_types[i])
		{
			fprintf(stderr, "\nERROR: function/procedure '%s' called with incorrect argument types, '%s' instead of '%s'.\n", fn->entry_name, type_string(type(fn_call->right)),type_string(fn->arg_types[i]));
			exit(0);
		}
		fn_call = fn_call->left;
	}
}
