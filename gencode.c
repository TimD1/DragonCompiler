#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// order matters
#include "gencode.h"
#include "tree.h"
#include "hash.h"
#include "reg_stack.h"
#include "y.tab.h"

FILE* outfile;

/* Print this code at the start of the assembly file */
void file_header(char* filename)
{
	fprintf(outfile, "\t.file\t\"%s\"\n", filename);
	fprintf(outfile, "\t.intel_syntax noprefix\n\n");
}


/* Add input and output functions if passed as parameters to main */
void add_io_code()
{
	if(get_entry(top_table(), "input"))
	{

	}
	if(get_entry(top_table(), "output"))
	{
		fprintf(outfile, "\t.section\t.rodata\n");
		fprintf(outfile, ".LC0:\n");
		fprintf(outfile, "\t.string \"%%d\"\n");
	}
	fprintf(outfile, "\t.text\n\n\n");
}


/* Print this code at the end of the assembly file */
void file_footer()
{
	fprintf(outfile, "\t.ident\t\"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609\"\n");
	fprintf(outfile, "\n\t.section\t.note.GNU-stack,\"\",@progbits\n");
}


/* Print this code at the start of each function */
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

	fprintf(outfile, "\t.globl\t%s\n", fn_name);
	fprintf(outfile, "\t.type\t%s, @function\n", fn_name);
	fprintf(outfile, "%s:\n", fn_name);
	//fprintf(outfile, ".LFB%d:\n", n->attribute.ival);
	//fprintf(outfile, "\t.cfi_startproc\n");
	fprintf(outfile, "\tpush\trbp\n");
	//fprintf(outfile, "\t.cfi_def_cfa_offset 16\n");
	//fprintf(outfile, "\t.cfi_offset 6, -16\n");
	fprintf(outfile, "\tmov rbp, rsp\n\n");
	//fprintf(outfile, "\t.cfi_def_cfa_register 6\n\n");
}


/* Print this code at the end of each function */
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

	fprintf(outfile, "\tpop rbp\n");
	//fprintf(outfile, "\t.cfi_def_cfa 7, 8\n");
	fprintf(outfile, "\tret\n");
	//fprintf(outfile, "\t.cfi_endproc\n");
	//fprintf(outfile, ".LFE%d:\n", n->attribute.ival);
	fprintf(outfile, "\t.size\t%s, .-%s\n\n\n", fn_name, fn_name);
}


/* Print this code at the start of the main function */
void main_header()
{
	fprintf(outfile, "\t.globl\tmain\n");
	fprintf(outfile, "\t.type\tmain, @function\n");
	fprintf(outfile, "main:\n");
	//fprintf(outfile, ".LFB0:\n");
	//fprintf(outfile, "\t.cfi_startproc\n");
	fprintf(outfile, "\tpush\trbp\n");
	//fprintf(outfile, "\t.cfi_def_cfa_offset 16\n");
	//fprintf(outfile, "\t.cfi_offset 6, -16\n");
	fprintf(outfile, "\tmov rbp, rsp\n\n");
	//fprintf(outfile, "\t.cfi_def_cfa_register 6\n\n");
}


/* Print this code at the end of the main function */
void main_footer()
{
	fprintf(outfile, "\tmov eax, 0\n");
	fprintf(outfile, "\tpop rbp\n");
	//fprintf(outfile, "\t.cfi_def_cfa 7, 8\n");
	fprintf(outfile, "\tret\n");
	//fprintf(outfile, "\t.cfi_endproc\n");
	//fprintf(outfile, ".LFE0:\n");
	fprintf(outfile, "\n\t.size\tmain, .-main\n\n\n");
}


/* Given a string describing operator, return the corresponding x86-64
   Intel Assembly instruction */
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


/* Given a node of unknown type, return the node's value as a string */
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


/* Given variable name, return assembly pointer to location */
char* var_to_assembly(char* name)
{
	char str[50] = "DWORD PTR [rbp-";
	char num[20];
	sprintf(num, "%d", get_entry_id(name)*4);
	strcat(str, num);
	strcat(str, "]");
	fprintf(stderr, "%s\n", str);
	return strdup(str);
}


/* Given pointer to assignment statement, assign rval to left */
void assignment_gencode(tree_t* n)
{
	gencode(n->right);
	fprintf(outfile, "\tmov %s, %s\n", string_value(n->left), reg_string(top(rstack)));
}


/* Given pointer to procedure call, copy params into correct registers
   before calling procedure */
void copy_params_code(tree_t* n)
{
	char* name = strdup(n->left->attribute.sval);
	if(!strcmp(name, "input"))
	{
	}
	else if(!strcmp(name, "output"))
	{
		fprintf(outfile, "\tmov edx, 42\n");
		fprintf(outfile, "\tmov esi, OFFSET_FLAT:.LC0\n");
		fprintf(outfile, "\tmov rdi QWORD PTR stderr[rip]\n");
		fprintf(outfile, "\tmov eax, 0\n");
		fprintf(outfile, "\tcall\tfprintf\n\n");
	}
	else
	{
		entry_t* entry_ptr = find_entry(top_table(), name);
		if(entry_ptr != NULL) // function valid
		{

		}
	}

}


/* Given pointer to expression in tree, generate code for expression */
void gencode(tree_t* n)
{
	/* Case 0: n is a left leaf */
	if(leaf_node(n) && n->ershov_num == 1)
	{
		fprintf(outfile, "\tmov %s, %s\n", reg_string(top(rstack)), var_to_assembly(string_value(n)));
	}

	/* Case 1: the right child of n is a leaf */
	else if( !empty(n->right) && leaf_node(n->right) && n->right->ershov_num == 0)
	{
		gencode(n->left);
		fprintf(outfile, "\t%s %s, %s\n", ia64(n->attribute.opval), 
				reg_string(top(rstack)), var_to_assembly(string_value(n->right)));
	}

	/* Case 2: the right subproblem is larger */
	else if(n->left->ershov_num <= n->right->ershov_num)
	{
		swap(rstack);
		gencode(n->right);
		int r = pop(rstack);
		gencode(n->left);
		fprintf(outfile, "\t%s %s, %s\n", ia64(n->attribute.opval), 
				reg_string(top(rstack)), reg_string(r));
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

