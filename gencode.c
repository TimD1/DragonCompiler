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

/* 
   	REGISTERS
   	---------
	
	eax = return value
	rbx = save for later
	rsp = stack pointer
	rbp = base pointer
	rip = instruction pointer

	edi	= 1st argument
	esi	= 2nd argument
	edx	= 3rd argument
	ecx	= 4th argument
	r8d	= 5th argument
	r9d	= 6th argument

	r10d = temp var (top of stack)
	r11d = temp var
	r12d = temp var
	r13d = temp var
	r14d = temp var
	r15d = temp var (bottom of stack)
*/


/* Print this code at the start of the assembly file */
void file_header(char* filename)
{
	fprintf(outfile, "# file header\n");
	fprintf(outfile, "\t.file\t\"%s\"\n", filename);
	fprintf(outfile, "\t.intel_syntax noprefix\n\n");
}


/* Add input and output functions if passed as parameters to main */
void add_io_code()
{
	if(get_entry(top_table(), "write") || get_entry(top_table(), "read"))
	{
		fprintf(outfile, "# create io format strings\n");
		fprintf(outfile, "\t.section\t.rodata\n");
		fprintf(outfile, ".LC0: # reading\n");
		fprintf(outfile, "\t.string \"%%d\"\n");
		fprintf(outfile, ".LC1: # writing\n");
		fprintf(outfile, "\t.string \"%%d\\n\"\n");
	}
	fprintf(outfile, "\t.text\n\n\n");
}


/* Print this code at the end of the assembly file */
void file_footer()
{
	fprintf(outfile, "\n# file footer\n");
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

	// print function header info
	fprintf(outfile, "\n# function header\n");
	fprintf(outfile, "\t.globl\t%s\n", fn_name);
	fprintf(outfile, "\t.type\t%s, @function\n", fn_name);
	fprintf(outfile, "%s:\n", fn_name);
	fprintf(outfile, "\tpush\trbp\n");
	fprintf(outfile, "\tmov rbp, rsp\n\n");

	// copy values from registers to local parameters
	fprintf(outfile, "\n# copy for function: register -> parameter\n");
	static char* arg_regs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
	entry_t* entry_ptr = find_entry(top_table(), fn_name);
	if(entry_ptr != NULL)
	{
		for(int i = 0; i < entry_ptr->arg_num; i++)
		{
			fprintf(outfile, "\tmov DWORD PTR [rbp-%d], %s\n", 4*(i+1), arg_regs[i]);
		}
	}
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

	fprintf(outfile, "\n# function footer\n");
	fprintf(outfile, "\tpop rbp\n");
	fprintf(outfile, "\tret\n");
	fprintf(outfile, "\t.size\t%s, .-%s\n\n\n", fn_name, fn_name);
}


/* Print this code at the start of the main function */
void main_header()
{
	fprintf(outfile, "\n# main header\n");
	fprintf(outfile, "\t.globl\tmain\n");
	fprintf(outfile, "\t.type\tmain, @function\n");
	fprintf(outfile, "main:\n");
	fprintf(outfile, "\tpush\trbp\n");
	fprintf(outfile, "\tmov rbp, rsp\n");
	fprintf(outfile, "\n# main code\n");
}


/* Print this code at the end of the main function */
void main_footer()
{
	fprintf(outfile, "\n# main footer\n");
	fprintf(outfile, "\tmov eax, 0\n");
	fprintf(outfile, "\tpop rbp\n");
	fprintf(outfile, "\tret\n");
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
			return var_to_assembly(n->attribute.sval);
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
	return strdup(str);
}


/* Given pointer to assignment statement, assign rval to left */
void assignment_gencode(tree_t* n)
{
	gencode(n->right);
	fprintf(outfile, "\tmov %s, %s\n", string_value(n->left), reg_string(top(rstack)));
}


/* Given pointer to procedure call, copy params into correct registers
   before calling it. If it's 'write' or 'read', use overloaded fprintf. */
void call_procedure(tree_t* n)
{
	// if procedure takes no arguments, there's nothing to do
	if(n->type != PARENOP)
	{	
		char* name = strdup(n->attribute.sval);
		fprintf(outfile, "\n# call procedure '%s'\n", name);
		fprintf(outfile, "\tcall\t%s\n\n", name);
		return;
	}

	static char* arg_regs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
	char* name = strdup(n->left->attribute.sval);
	if(!strcmp(name, "read")) // special case
	{
		char* var_name = n->right->right->attribute.sval;
		fprintf(outfile, "\n# call 'read' using fscanf\n");
		fprintf(outfile, "\tlea rdx, [rbp-%d]\n", get_entry_id(var_name)*4);
		fprintf(outfile, "\tmov esi, OFFSET FLAT:.LC0\n");
		fprintf(outfile, "\tmov rdi, QWORD PTR stdin[rip]\n");
		fprintf(outfile, "\tmov eax, 0\n");
		fprintf(outfile, "\tcall __isoc99_fscanf\n\n");
	}
	else if(!strcmp(name, "write")) // special case
	{
		tree_t* var_ptr = n->right->right;
		fprintf(outfile, "\n# call 'write' using fprintf\n");
		fprintf(outfile, "\tmov edx, %s\n", string_value(var_ptr));
		fprintf(outfile, "\tmov esi, OFFSET FLAT:.LC1\n");
		fprintf(outfile, "\tmov rdi, QWORD PTR stderr[rip]\n");
		fprintf(outfile, "\tmov eax, 0\n");
		fprintf(outfile, "\tcall\tfprintf\n\n");
	}
	else // normal function
	{
		fprintf(outfile, "\n# call procedure '%s'\n", name);
		entry_t* entry_ptr = find_entry(top_table(), name);
		if(entry_ptr != NULL) // function valid
		{
			tree_t* list_ptr = n->right;
			fprintf(outfile, "# copy for procedure: variable -> register\n");
			for(int i = 0; i < entry_ptr->arg_num; i++)
			{
				fprintf(outfile,"\tmov %s, %s\n", arg_regs[i], string_value(list_ptr->right));
				list_ptr = list_ptr->left;
			}
			fprintf(outfile, "\tcall %s\n", name);
		}
	}

}


/* Given pointer to expression in tree, generate code for expression */
void gencode(tree_t* n)
{
	/* Case 0: n is a left leaf */
	if(leaf_node(n) && n->ershov_num == 1)
	{
		fprintf(outfile, "\tmov %s, %s\n", reg_string(top(rstack)), string_value(n));
	}

	/* Case 1: the right child of n is a leaf */
	else if( !empty(n->right) && leaf_node(n->right) && n->right->ershov_num == 0)
	{
		gencode(n->left);
		fprintf(outfile, "\t%s %s, %s\n", ia64(n->attribute.opval), 
				reg_string(top(rstack)), string_value(n->right));
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

