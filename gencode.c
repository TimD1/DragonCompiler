#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// order matters
#include "gencode.h"
#include "tree.h"
#include "hash.h"
#include "reg_stack.h"
#include "externs.h"
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

	rdi = reserved for io use
	rsi = reserved for io use
	rdx = reserved for io use
	
	rcx = temp var (top of stack)
	r8  = temp var
	r9  = temp var
	r10 = temp var	
	r11 = temp var
	r12 = temp var
	r13 = temp var
	r14 = temp var
	r15 = temp var (bottom of stack)
*/


/* Print this code at the start of the assembly file */
void file_header(char* filename)
{
	if (GENCODE_DEBUG) fprintf(outfile, "# file header\n");
	fprintf(outfile, "\t.file\t\"%s\"\n", filename);
	fprintf(outfile, "\t.intel_syntax noprefix\n\n");
}


/* Add input and output functions if passed as parameters to main */
void add_io_code()
{
	if(get_entry(top_table(), "write") || get_entry(top_table(), "read"))
	{
		if (GENCODE_DEBUG) fprintf(outfile, "\n# create io format strings\n");
		fprintf(outfile, "\t.section\t.rodata\n");
		fprintf(outfile, ".LC0: # reading\n");
		fprintf(outfile, "\t.string \"%%lld\"\n");
		fprintf(outfile, ".LC1: # writing\n");
		fprintf(outfile, "\t.string \"%%lld\\n\"\n");
	}
	fprintf(outfile, "\t.text\n\n\n");
}


/* Print this code at the end of the assembly file */
void file_footer()
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# file footer\n");
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
		fprintf(stderr, "\nERROR, LINE %d: function name expected.\n", yylineno);
		exit(1);
	}

	char* fn_name = name_ptr->attribute.sval;

	// print function header info
	if (GENCODE_DEBUG) fprintf(outfile, "\n# function header\n");
	fprintf(outfile, "\t.globl\t%s\n", fn_name);
	fprintf(outfile, "\t.type\t%s, @function\n", fn_name);
	fprintf(outfile, "%s:\n", fn_name);
	fprintf(outfile, "\tpush\trbp\n");
	fprintf(outfile, "\tmov\t\trbp, rsp\n\n");

	/* // copy values from registers to local parameters */
	/* if (GENCODE_DEBUG) fprintf(outfile, "\n# copy for function: register -> parameter\n"); */
	/* static char* arg_regs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"}; */
	/* entry_t* entry_ptr = find_entry(top_table(), fn_name); */
	/* if(entry_ptr != NULL) */
	/* { */
	/* 	for(int i = 0; i < entry_ptr->arg_num; i++) */
	/* 	{ */
	/* 		fprintf(outfile, "\tmov QWORD PTR [rbp-%d], %s\n", 8*(i+1), arg_regs[i]); */
	/* 	} */
	/* } */

}


/* Print this code at the end of each function */
void function_footer(tree_t* n)
{
	tree_t* name_ptr = n->left;
	if(name_ptr->type == PARENOP)
		name_ptr = name_ptr->left;
	if(name_ptr->type != IDENT)
	{
		fprintf(stderr, "\nERROR, LINE %d: function name expected.\n", yylineno);
		exit(1);
	}

	char* fn_name = name_ptr->attribute.sval;

	if (GENCODE_DEBUG) fprintf(outfile, "\n# function footer\n");
	fprintf(outfile, "\tpop\t\trbp\n");
	fprintf(outfile, "\tret\n");
	fprintf(outfile, "\t.size\t%s, .-%s\n\n\n", fn_name, fn_name);
}


/* Print this code at the start of the main function */
void main_header()
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# main header\n");
	fprintf(outfile, "\t.globl\tmain\n");
	fprintf(outfile, "\t.type\tmain, @function\n");
	fprintf(outfile, "main:\n");
	fprintf(outfile, "\tpush\trbp\n");
	fprintf(outfile, "\tmov\t\trbp, rsp\n");
	fprintf(outfile, "\tsub\t\trsp, 256\n"); // leave constant for now
	if (GENCODE_DEBUG) fprintf(outfile, "\n# main code\n");
}


/* Print this code at the end of the main function */
void main_footer()
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# main footer\n");
	fprintf(outfile, "\tmov\t\teax, 0\n");
	fprintf(outfile, "\tleave\n");
	fprintf(outfile, "\tret\n");
	fprintf(outfile, "\n\t.size\tmain, .-main\n\n\n");
}


/* Given a string describing operator, return the corresponding x86-64
   Intel Assembly instruction */
char* ia64(char* opval)
{
	if(!strcmp(opval, "+"))
		return strdup("add ");
	if(!strcmp(opval, "-"))
		return strdup("sub ");
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
	char str[50] = "QWORD PTR [rbp-";
	char num[20];
	sprintf(num, "%d", get_entry_id(name)*8);
	strcat(str, num);
	strcat(str, "]");
	return strdup(str);
}


/* Given pointer to assignment statement, assign rval to left */
void assignment_gencode(tree_t* n)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# evaluate expression\n");
	gencode(n->right);
	if (GENCODE_DEBUG) fprintf(outfile, "\n# assignment\n");
	fprintf(outfile, "\tmov\t\t%s, %s\n", string_value(n->left), reg_string(top(rstack)));
}


void start_if_gencode(tree_t* n, int label_num)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# start if\n");
	gencode(n); // evaluate conditional
	fprintf(outfile, "\tcmp\t\t%s, 0\n", reg_string(top(rstack)));
	fprintf(outfile, "\tje .L%d\n", label_num);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end conditional\n");
}


void start_if_else_gencode(tree_t* n, int label_num)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# start if-else\n");
	gencode(n); // evaluate conditional
	fprintf(outfile, "\tcmp\t\t%s, 0\n", reg_string(top(rstack)));
	fprintf(outfile, "\tje .L%d\n", label_num);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end conditional\n");
}


void mid_if_else_gencode(int label_num)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# mid if-else\n");
	fprintf(outfile, "\tjmp .L%d\n", label_num+1);
	fprintf(outfile, ".L%d:\n", label_num);
}


void end_if_gencode(int label_num)
{
	fprintf(outfile, ".L%d:\n", label_num);
	if (GENCODE_DEBUG) fprintf(outfile, "\n# end if\n");
}


void start_while_do_gencode(tree_t* n, int label_num)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# start while\n");
	fprintf(outfile, ".L%d:\n", label_num);
	gencode(n); // evaluate conditional
	fprintf(outfile, "\tcmp\t\t%s, 0\n", reg_string(top(rstack)));
	fprintf(outfile, "\tje .L%d\n", label_num+1);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end while, start do\n");
}


void end_while_do_gencode(int label_num)
{
	fprintf(outfile, "\tjmp .L%d\n", label_num);
	fprintf(outfile, ".L%d:\n", label_num+1);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end do\n");
}


void start_repeat_until_gencode(int label_num)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# start repeat\n");
	fprintf(outfile, ".L%d:\n", label_num);
}


void end_repeat_until_gencode(tree_t* n, int label_num)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end repeat, start until\n");
	gencode(n); // evaluate conditional
	fprintf(outfile, "\tcmp\t\t%s, 0\n", reg_string(top(rstack)));
	fprintf(outfile, "\tje .L%d\n", label_num);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end until\n");
}


/* Given pointer to procedure call, copy params into correct registers
   before calling it. If it's 'write' or 'read', use overloaded fprintf. */
void call_procedure(tree_t* n)
{
	// if procedure takes no arguments, there's nothing to do
	if(n->type != PARENOP)
	{	
		char* name = strdup(n->attribute.sval);
		if (GENCODE_DEBUG) fprintf(outfile, "\n# call procedure '%s'\n", name);
		fprintf(outfile, "\tcall\t%s\n\n", name);
		return;
	}

	/* static char* arg_regs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"}; */
	char* name = strdup(n->left->attribute.sval);
	if(!strcmp(name, "read")) // special case
	{
		char* var_name = n->right->right->attribute.sval;
		if (GENCODE_DEBUG) fprintf(outfile, "\n# call 'read' using fscanf\n");
		fprintf(outfile, "\tmov\t\trax, QWORD PTR fs:40\n");
		fprintf(outfile, "\txor\t\teax, eax\n");
		fprintf(outfile, "\tmov\t\trax, QWORD PTR stdin[rip]\n");
		fprintf(outfile, "\tlea\t\trdx, [rbp-%d]\n", get_entry_id(var_name)*8);
		fprintf(outfile, "\tmov\t\tesi, OFFSET FLAT:.LC0\n");
		fprintf(outfile, "\tmov\t\trdi, rax\n");
		fprintf(outfile, "\tmov\t\teax, 0\n");
		fprintf(outfile, "\tcall\t__isoc99_fscanf\n\n");
	}
	else if(!strcmp(name, "write")) // special case
	{
		tree_t* var_ptr = n->right->right;
		if (GENCODE_DEBUG) fprintf(outfile, "\n# call 'write' using fprintf\n");
		fprintf(outfile, "\tmov\t\trdx, %s\n", string_value(var_ptr));
		fprintf(outfile, "\tmov\t\trax, QWORD PTR stderr[rip]\n");
		fprintf(outfile, "\tmov\t\tesi, OFFSET FLAT:.LC1\n");
		fprintf(outfile, "\tmov\t\trdi, rax\n");
		fprintf(outfile, "\tmov\t\teax, 0\n");
		fprintf(outfile, "\tcall\tfprintf\n\n");
	}
	/* else // normal function */
	/* { */
	/* 	if (GENCODE_DEBUG) fprintf(outfile, "\n# call procedure '%s'\n", name); */
	/* 	entry_t* entry_ptr = find_entry(top_table(), name); */
	/* 	if(entry_ptr != NULL) // function valid */
	/* 	{ */
	/* 		tree_t* list_ptr = n->right; */
	/* 		fprintf(outfile, "# copy for procedure: variable -> register\n"); */
	/* 		for(int i = 0; i < entry_ptr->arg_num; i++) */
	/* 		{ */
	/* 			fprintf(outfile,"\tmov %s, %s\n", arg_regs[i], string_value(list_ptr->right)); */
	/* 			list_ptr = list_ptr->left; */
	/* 		} */
	/* 		fprintf(outfile, "\tcall %s\n", name); */
	/* 	} */
	/* } */

}


/* Given pointer to expression in tree, generate code for expression */
void gencode(tree_t* n)
{
	/* Case 0: n is a left leaf */
	if(leaf_node(n) && n->ershov_num == 1)
		print_code("mov", reg_string(top(rstack)), string_value(n));

	/* Case 1: the right child of n is a leaf */
	else if( !empty(n->right) && leaf_node(n->right) && n->right->ershov_num == 0)
	{
		gencode(n->left);
		print_code(n->attribute.opval, reg_string(top(rstack)), string_value(n->right));
	}

	/* Case 2: the right subproblem is larger */
	else if(n->left->ershov_num <= n->right->ershov_num)
	{
		swap(rstack);
		gencode(n->right);
		int r = pop(rstack);
		gencode(n->left);
		print_code(n->attribute.opval, reg_string(top(rstack)), reg_string(r));
		push(r, rstack);
		swap(rstack);
	}

	/* Case 3: the left subproblem is larger */
	else if(n->left->ershov_num >= n->right->ershov_num)
	{
		gencode(n->left);
		int r = pop(rstack);
		gencode(n->right);
		print_code(n->attribute.opval, reg_string(r), reg_string(top(rstack)));
		push(r, rstack);
	}

	/* Case 4: insufficient registers */ //shouldn't need this for now
	else
	{
		fprintf(stderr, "ERROR: Case 4 of gencode reached.");
	}
}


void print_code(char* opval, char* left, char* right)
{
	// leaf
	if(!strcmp(opval, "mov"))
		fprintf(outfile, "\tmov\t\t%s, %s\n", left, right);

	// expressions
	else if(!strcmp(opval, "+"))
		fprintf(outfile, "\tadd\t\t%s, %s\n", left, right);

	else if(!strcmp(opval, "-"))
		fprintf(outfile, "\tsub\t\t%s, %s\n", left, right);

	else if(!strcmp(opval, "*"))
		fprintf(outfile, "\timul\t%s, %s\n", left, right);

	else if(!strcmp(opval, "/"))
		fprintf(outfile, "\tidiv\t%s, %s\n", left, right);

	// booleans
	else if(!strcmp(opval, "and"))
		fprintf(outfile, "\tand\t\t%s, %s\n", left, right);

	else if(!strcmp(opval, "or"))
		fprintf(outfile, "\tor\t\t%s, %s\n", left, right);

	/* if(!strcmp(opval, "not")) */
	/* 	fprintf(outfile, "\tnot %s\n", left); */

	// relations
	else 
	{
		char* op;
		if(!strcmp(opval, "=")) 	  op = "sete";
		else if(!strcmp(opval, "<>")) op = "setne";
		else if(!strcmp(opval, "<"))  op = "setl";
		else if(!strcmp(opval, "<=")) op = "setle";
		else if(!strcmp(opval, ">"))  op = "setg";
		else if(!strcmp(opval, ">=")) op = "setge";
		else op = "ERROR";
			
		fprintf(outfile, "\tcmp\t\t%s, %s\n", left, right);
		fprintf(outfile, "\t%s\t%s\n", op, get_end(left));
		fprintf(outfile, "\tmovzx\t%s, %s\n", left, get_end(left));
	}

}


/* Assumes we will only need to do this for registers on the stack. */
char* get_end(char* reg)
{
	// get names of legacy registers
	if(!strcmp(reg, "rdi")) return strdup("dil");
	if(!strcmp(reg, "rsi")) return strdup("sil");
	if(!strcmp(reg, "rdx")) return strdup("dl");
	if(!strcmp(reg, "rcx")) return strdup("cl");

	// add 'b' onto registers r8+
	char new_reg[5];
	strcpy(new_reg, reg);
	if(new_reg[2] == '\0') // r8 or r9
	{
		new_reg[2] = 'b';
		new_reg[3] = '\0';
	}
	else // r10+
	{
		new_reg[3] = 'b';
		new_reg[4] = '\0';
	}
	return strdup(new_reg);
}
