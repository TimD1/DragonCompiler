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
	
	rax = return value & division
	rbx = for loops
	rcx = nonlocal variable access
	rsp = stack pointer
	rbp = base pointer
	rip = instruction pointer

	rdi = reserved for io use
	rsi = reserved for io use
	rdx = reserved for io use & division
	

	r8  = temp var (top of stack)
	r9  = temp var
	r10 = temp var	
	r11 = temp var
	r12 = temp var
	r13 = temp var
	r14 = temp var
	r15 = temp var (bottom of stack)





   STACK FRAME CONTENTS
   --------------------

   low memory addresses
			.
   			.
			.
   -------------------
   |     temp_reg3   | 	<--- rsp
   -------------------
   |     temp_reg2   |
   -------------------
   |     temp_reg1   |
   -------------------
   |     local3      |
   -------------------
   |     local2      |
   -------------------
   |     local1      |
   -------------------
   |     parent      |  rbp - 16
   -------------------
   |     return_val  |  rbp - 8
   -------------------
   |     old_rbp     | 	<--- rbp
   -------------------
   |     old_rip     |	(automatically added/removed by call/ret)
   -------------------
   |     param3      |
   -------------------
   |     param2      |
   -------------------
   |     param1      |
   -------------------
			.
   			.
			.
   high memory addresses

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

	// allocate room for static parent pointer, return value, and local vars
	if (GENCODE_DEBUG) fprintf(outfile, "\n# set up stack frame\n");
	fprintf(outfile, "\tpush\t0\n");
	push_static_parent();
	fprintf(outfile, "\tsub\t\trsp, %d\n", 8*(top_table()->num_locals));
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end function header\n");
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
	fprintf(outfile, ".L%d:\n", n->attribute.ival);
	fprintf(outfile, "\tmov\t\trax, QWORD PTR [rbp-8]\n");
	fprintf(outfile, "\tmov\t\trsp, rbp\n");
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

	// allocate room for static parent pointer, return value, and local vars
	if (GENCODE_DEBUG) fprintf(outfile, "\n# set up stack frame\n");
	fprintf(outfile, "\tpush\t0\n");
	fprintf(outfile, "\tpush\trbp\n");
	fprintf(outfile, "\tsub\t\trsp, %d\n", 8*(top_table()->num_locals));
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end function header\n");

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
			grab_nonlocal_var(n->attribute.sval);
			return var_to_assembly(n->attribute.sval);
		case PARENOP: // function call
			return strdup("rax");
		default:
			return strdup("???");
	}
}


/* If it's a nonlocal variable, find the corrrect location and copy
   the variable into reserved register 'rcx'.
*/
void grab_nonlocal_var(char* name)
{
	// only act if variable is nonlocal
	if(get_entry_id(top_table(), name))
		return;
	
	if (GENCODE_DEBUG) fprintf(outfile, "\n# find nonlocal variable\n");

	// count how many scopes back the variable is
	int scopes_back = 0;
	table_t* table = top_table();
	int id = get_entry_id(table, name);
	while(id == 0)
	{
		scopes_back++;
		if(table == head_table)
		{
			fprintf(stderr, "\nERROR, LINE %d: variable '%s' could not be found.\n", 
				yylineno, name);
			exit(0);
		}
		table = table->parent;
		id = get_entry_id(table, name);
	}

	// traverse back correct number of scopes to access variable 
	fprintf(outfile, "\tmov\t\trcx, rbp\n");
	for(int i = 0; i < scopes_back; i++)
	{
		fprintf(outfile, "\tmov\t\trcx, QWORD PTR [rcx-16]\n");
	}

	if (GENCODE_DEBUG) fprintf(outfile, "\t# end nonlocal variable\n");
}


/* Given variable name, return assembly pointer to location. Nonlocal variables
   are handled by the grab_nonlocal_var() function, which is always called directly
   before this and stores the correct base pointer in rcx. */
char* var_to_assembly(char* name)
{
	table_t* table = top_table();
	int id = get_entry_id(table, name);

	char* base_ptr = strdup("rbp"); // if local
	if(!id)
		base_ptr = strdup("rcx"); // if nonlocal

	// find entry_id in closest table
	while(id == 0)
	{
		if(table == head_table)
		{
			fprintf(stderr, "\nERROR, LINE %d: variable '%s' could not be found.\n", 
				yylineno, name);
			exit(0);
		}
		table = table->parent;
		id = get_entry_id(table, name);
	}

	// find the necesssary offset from the base pointer
	int params = table->num_params;
	if(id <= params) // parameter
	{
		char str[150];
		sprintf(str, "QWORD PTR [%s+", base_ptr);
		char num[20];
		sprintf(num, "%d", (params-id+2)*8); // store below base pointer and rip
		strcat(str, num);
		strcat(str, "]");
		return strdup(str);
	}
	else // local variable
	{
		char str[150];
		sprintf(str, "QWORD PTR [%s-", base_ptr);
		char num[20];
		sprintf(num, "%d", (id-params+2)*8); 	// store above base pointer,
		strcat(str, num);						// leaving room for return value
		strcat(str, "]");						// and static parent pointer
		return strdup(str);
	}
}


/* Given an integer l, assign it to the top scope's assembly label.
   This will make label '.Ll' visible throughout the function and allow jumps
   to the end whenever return statements are encountered. */
void set_fn_assembly_label(int l)
{
	top_table()->assembly_label = l;
}


/* Given pointer to assignment statement, assign rval to left. 
   If the left value is a string named identically to the current function,
   then this is actually a return statement. */
void assignment_gencode(tree_t* n)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# evaluate expression\n");
	gencode(n->right);
	if(!strcmp(n->left->attribute.sval, top_table()->table_name))
	{
		if (GENCODE_DEBUG) fprintf(outfile, "\n# return statement\n");
		fprintf(outfile, "\tmov\t\tQWORD PTR [rbp-8], %s\n", reg_string(top(rstack)));
		fprintf(outfile, "\tjmp\t\t.L%d\n", top_table()->assembly_label);
	}
	else
	{
		if (GENCODE_DEBUG) fprintf(outfile, "\n# assignment\n");
		fprintf(outfile, "\tmov\t\t%s, %s\n", 
			string_value(n->left), reg_string(top(rstack)));
	}
}


void start_if_gencode(tree_t* n, int label_num)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# start if\n");
	gencode(n); // evaluate conditional
	fprintf(outfile, "\tcmp\t\t%s, 0\n", reg_string(top(rstack)));
	fprintf(outfile, "\tje\t\t.L%d\n", label_num);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end conditional\n");
}


void start_if_else_gencode(tree_t* n, int label_num)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# start if-else\n");
	gencode(n); // evaluate conditional
	fprintf(outfile, "\tcmp\t\t%s, 0\n", reg_string(top(rstack)));
	fprintf(outfile, "\tje\t\t.L%d\n", label_num);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end conditional\n");
}


void mid_if_else_gencode(int label_num)
{
	if (GENCODE_DEBUG) fprintf(outfile, "\n# mid if-else\n");
	fprintf(outfile, "\tjmp\t\t.L%d\n", label_num+1);
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
	fprintf(outfile, "\tje\t\t.L%d\n", label_num+1);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end while, start do\n");
}


void end_while_do_gencode(int label_num)
{
	fprintf(outfile, "\tjmp\t\t.L%d\n", label_num);
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
	fprintf(outfile, "\tje\t\t.L%d\n", label_num);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end until\n");
}


void start_for_gencode(int l, tree_t* var, tree_t* e1, char* to_downto, tree_t* e2)
{
	// determine whether variable counts up or down
	int UP;
	if(!strcmp(to_downto, "to")) UP = 1;
	else UP = 0;

	// assign variable to start loop
	if (GENCODE_DEBUG) fprintf(outfile, "\n# start for\n");
	gencode(e1);
	fprintf(outfile, "\tmov\t\t%s, %s\n", string_value(var), reg_string(top(rstack)));
	if (GENCODE_DEBUG) fprintf(outfile, "\t# evaluated start position\n");
	
	// save value which ends the loop
	gencode(e2);
	fprintf(outfile, "\tmov\t\trbx, %s\n", reg_string(top(rstack)));
	if (GENCODE_DEBUG) fprintf(outfile, "\t# evaluated end position\n");

	fprintf(outfile, ".L%d:\n", l);
	if(UP)
	{
		fprintf(outfile, "\tcmp\t\t%s, rbx\n", string_value(var));
		fprintf(outfile, "\tjg\t\t.L%d\n", l+1);
		if (GENCODE_DEBUG) fprintf(outfile, "\t# end for\n\n# start do");
	}
	else
	{
		fprintf(outfile, "\tcmp\t\t%s, rbx\n", string_value(var));
		fprintf(outfile, "\tjl\t\t.L%d\n", l+1);
		if (GENCODE_DEBUG) fprintf(outfile, "\t# end for\n\n# start do");
	}
}


void end_for_gencode(int l, tree_t* var, char* to_downto)
{
	int UP;
	if(!strcmp(to_downto, "to")) UP = 1;
	else UP = 0;

	if(UP)
		fprintf(outfile, "\tinc\t\t%s\n", string_value(var));
	else
		fprintf(outfile, "\tdec\t\t%s\n", string_value(var));

	fprintf(outfile, "\tjmp\t\t.L%d\n", l);
	fprintf(outfile, ".L%d:\n", l+1);
	if (GENCODE_DEBUG) fprintf(outfile, "\t# end for-do\n");
}


/* Given pointer to procedure call, copy params onto the stack
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

	char* name = strdup(n->left->attribute.sval);
	if(!strcmp(name, "read")) // special case
	{
		char* var_name = n->right->right->attribute.sval;
		if (GENCODE_DEBUG) fprintf(outfile, "\n# call 'read' using fscanf\n");
		fprintf(outfile, "\tmov\t\trax, QWORD PTR fs:40\n");
		fprintf(outfile, "\txor\t\teax, eax\n");
		fprintf(outfile, "\tmov\t\trax, QWORD PTR stdin[rip]\n");
		grab_nonlocal_var(var_name);
		fprintf(outfile, "\tlea\t\trdx, %s\n", var_to_assembly(var_name));
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
	else // normal function
	{
		if (GENCODE_DEBUG) fprintf(outfile, "\n# call procedure '%s'\n", name);
		entry_t* entry_ptr = find_entry(top_table(), name);
		if(entry_ptr != NULL) // function valid
		{
			if(entry_ptr->entry_class != PROCEDURE)
			{
				fprintf(stderr, "\nERROR, LINE %d: '%s' is not a procedure.\n", yylineno, name);
				exit(0);
			}

			tree_t* list_ptr = n->right;
			for(int i = 0; i < entry_ptr->arg_num; i++)
			{
				fprintf(outfile,"\tpush %s\n", string_value(list_ptr->right));
				list_ptr = list_ptr->left;
			}
			fprintf(outfile, "\tcall\t%s\n", name);
			fprintf(outfile, "\tadd\t\trsp, %d\n", 8*entry_ptr->arg_num);
		}
	}
}


/* Given pointer to function call, copy params onto the stack before calling. */
void call_function(tree_t* n)
{
	// if procedure takes no arguments, there's nothing to do
	if(n->type != PARENOP)
	{	
		char* name = strdup(n->attribute.sval);
		if (GENCODE_DEBUG) fprintf(outfile, "\n# call function '%s'\n", name);
		fprintf(outfile, "\tcall\t%s\n\n", name);
		return;
	}

	char* name = strdup(n->left->attribute.sval);

	if (GENCODE_DEBUG) fprintf(outfile, "\n# call function '%s'\n", name);
	entry_t* entry_ptr = find_entry(top_table(), name);
	if(entry_ptr != NULL) // function valid
	{
		if(entry_ptr->entry_class != FUNCTION)
		{
			fprintf(stderr, "\nERROR, LINE %d: '%s' is not a function.\n", yylineno, name);
			exit(0);
		}

		tree_t* list_ptr = n->right;
		for(int i = 0; i < entry_ptr->arg_num; i++)
		{
			fprintf(outfile,"\tpush %s\n", string_value(list_ptr->right));
			list_ptr = list_ptr->left;
		}
		fprintf(outfile, "\tcall\t%s\n", name);
		fprintf(outfile, "\tadd\t\trsp, %d\n", 8*entry_ptr->arg_num);
	}

}


/* Given pointer to expression in tree, generate code for expression */
void gencode(tree_t* n)
{
	if(empty(n))
		return;

	/* Case 0: n is a left leaf */
	if((leaf_node(n) && n->ershov_num == 1) || n->type == PARENOP)
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
	{
		fprintf(outfile, "\tmov\t\trax, %s\n", left);
		fprintf(outfile, "\tcqo\n");
		fprintf(outfile, "\tidiv\t%s\n", right);
		fprintf(outfile, "\tmov\t\t%s, rax\n", left);
	}

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


/* Gets the static parent pointer, under the following assumptions:
   1. top_table() points to the new function's symbol table entry
   2. caller points to the caller's symbol table entry
   3. the new function was called by a sibling or parent
*/
void push_static_parent()
{
	if(top_table()->parent == caller) // called by parent
		fprintf(outfile, "\tpush\t[rbp]\n");
	else // called by sibling, use that function's parent
	{
		int r = pop(rstack);
		fprintf(outfile, "\tmov\t\t%s, QWORD PTR [rbp]\n", reg_string(r));
		fprintf(outfile, "\tmov\t\t%s, QWORD PTR [%s-16]\n", reg_string(r), reg_string(r));
		fprintf(outfile, "\tpush\t%s\n", reg_string(r));
		push(r, rstack);
	}
}
