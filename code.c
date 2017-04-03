#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// order matters
#include "code.h"
#include "tree.h"
#include "hash.h"
#include "reg_stack.h"
#include "y.tab.h"

FILE* outfile;

void file_header(char* filename)
{
	fprintf(outfile, "\t.file\t\"%s\"\n", filename);
	fprintf(outfile, "\t.intel_syntax noprefix\n");
	fprintf(outfile, "\t.text\n\n\n");
}

void file_footer()
{
	fprintf(outfile, "\n\n\t.ident\t\"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609\"\n");
	fprintf(outfile, "\t.section\t.note.GNU-stack,\"\",@progbits");
}

void function_header(tree_t* n)
{
	tree_t* name_ptr = n->left;
	if(name_ptr->type == PARENOP)
		name_ptr = name_ptr->left;
	if(name_ptr->type != IDENT)
	{
		fprintf(stderr, "\nERROR: function name expected.\n");
		exit(1);
	}

	char* fn_name = name_ptr->attribute.sval;

	fprintf(outfile, "\n\n\t.globl\t%s\n", fn_name);
	fprintf(outfile, "\t.type\t%s, @function\n", fn_name);
	fprintf(outfile, "%s:\n", fn_name);
	fprintf(outfile, ".LFB%d:\n", n->attribute.ival);
	fprintf(outfile, "\t.cfi_startproc\n");
	fprintf(outfile, "\tpush\trbp\n");
	fprintf(outfile, "\t.cfi_def_cfa_offset 16\n");
	fprintf(outfile, "\t.cfi_offset 6, -16\n");
	fprintf(outfile, "\tmov rbp, rsp\n");
	fprintf(outfile, "\t.cfi_def_cfa_register 6\n\n");
}

void function_footer(tree_t* n)
{
	tree_t* name_ptr = n->left;
	if(name_ptr->type == PARENOP)
		name_ptr = name_ptr->left;
	if(name_ptr->type != IDENT)
	{
		fprintf(stderr, "\nERROR: function name expected.\n");
		exit(1);
	}

	char* fn_name = name_ptr->attribute.sval;

	fprintf(outfile, "\n\n\tpop rbp\n");
	fprintf(outfile, "\t.cfi_def_cfa 7, 8\n");
	fprintf(outfile, "\tret\n");
	fprintf(outfile, "\t.cfi_endproc\n");
	fprintf(outfile, ".LFE%d:\n", n->attribute.ival);
	fprintf(outfile, "\t.size\t%s, .-%s\n\n", fn_name, fn_name);
}

char* ia64(char* opval)
{
	if(!strcmp(opval, "+"))
		return strdup("add");
	if(!strcmp(opval, "-"))
		return strdup("sub");
	if(!strcmp(opval, "*"))
		return strdup("imul");
	if(!strcmp(opval, "/"))
		return strdup("idiv");
	return strdup("not_op");
}


char* string_value(tree_t* n)
{
	char str[100];
	switch(n->type)
	{
		case INUM:
			sprintf(str, "%d", n->attribute.ival);
			return strdup(str);
		
		case RNUM:
			sprintf(str, "%f", n->attribute.fval);
			return strdup(str);
		
		case IDENT:
		case STRING:
			return strdup(n->attribute.sval);
		default:
			return strdup("???");
	}
}


void assignment_gencode(tree_t* n)
{
	gencode(n->right);
	fprintf(outfile, "\tmov %s, %s\n", reg_string(top(rstack)), string_value(n->left));
}


void gencode(tree_t* n)
{
	/* Case 0: n is a left leaf */
	if(leaf_node(n) && n->ershov_num == 1)
	{
		fprintf(outfile, "\tmov %s, %s\n", string_value(n), reg_string(top(rstack)));
	}

	/* Case 1: the right child of n is a leaf */
	else if( !empty(n->right) && leaf_node(n->right) && n->right->ershov_num == 0)
	{
		gencode(n->left);
		fprintf(outfile, "\t%s %s, %s\n", ia64(n->attribute.opval), 
				string_value(n->right), reg_string(top(rstack)));
	}

	/* Case 2: the right subproblem is larger */
	else if(n->left->ershov_num <= n->right->ershov_num)
	{
		swap(rstack);
		gencode(n->right);
		int r = pop(rstack);
		gencode(n->left);
		fprintf(outfile, "\t%s %s, %s\n", ia64(n->attribute.opval), 
				reg_string(r), reg_string(top(rstack)));
		push(r, rstack);
		swap(rstack);
	}

	/* Case 3: the left subproblem is larger */
	else if(n->left->ershov_num >= n->right->ershov_num)
	{
		gencode(n->left);
		int r = pop(rstack);
		gencode(n->right);
		fprintf(outfile, "\t%s %s, %s\n", ia64(n->attribute.opval), 
				reg_string(r), reg_string(top(rstack)));
		push(r, rstack);
	}

	/* Case 4: insufficient registers */ //shouldn't need this for now
	else
	{
		fprintf(stderr, "ERROR: Case 4 of gencode reached.");
	}
}
