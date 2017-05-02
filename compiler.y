%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "tree.h"
#include "hash.h"
#include "reg_stack.h"
#include "gencode.h"
#include "y.tab.h"
#include "externs.h"


int yydebug = 1;

int yywrap() { return 1; }

void yyerror(const char *str) { fprintf(stderr, "\nERROR, LINE %d: %s\n", yylineno, str); }

int main(int argc, char** argv) 
{ 
	// user must give single file name
	if(argc != 2)
	{
		fprintf(stderr, "Incorrect number of arguments given.\n");
		exit(0);
	}
	
	// try to open file
	FILE *infile = fopen(argv[1], "r");
	if(!infile)
	{
		fprintf(stderr, "Invalid input file name given.\n");
		exit(0);
	}

	// create output file by changing extension
	char* outfile_name = (char*)malloc(strlen(argv[1]));
	strcpy(outfile_name, (char*)argv[1]);
	outfile_name[strlen(outfile_name)-1] = 's';
	outfile = fopen(outfile_name, "w");

	/* Create base table for doubly linked "stack" of all tables 
	   tables on top have narrower scope, and prev is the top table */
	head_table = (table_t*)malloc(sizeof(table_t));
	for(int i = 0; i < TABLE_SIZE; i++)
		head_table->hash_table[i] = NULL;
	head_table->prev = head_table;
	head_table->next = head_table;
	head_table->id = -1;

	// set up register stack
	rstack = (stack_t*)malloc(sizeof(stack_t));
	for(int i = 0; i < MAX_REGS; i++)
		rstack->reg[i] = i;
	rstack->top_idx = MAX_REGS-1;

	// parse input file
	fprintf(stderr, "TOKENIZATION\n____________\n\n");
	yyin = infile;
	file_header(argv[1]);
	do { yyparse(); }
	while (!feof(yyin));
	file_footer();
	
	fclose(infile);
	fclose(outfile);
}
%}

%define parse.error verbose
%define parse.lac full

/* create union to hold value of current token */
%union {
	int ival;
	float fval;
	char *opval;
	char *sval;

	tree_t *tval;
}

/* number tokens */
%token <ival> INUM
%token <fval> RNUM

/* operator tokens */
%token <opval> ADDOP	/* 	+ - or 			*/
%token <opval> MULOP	/* 	* / and 		*/
%token <opval> RELOP	/* 	< > = <= >= <> 	*/
%token <opval> ASSOP	/* 	:= 				*/
%token <opval> NOT		/* 	not 			*/

%token <opval> ARRAYOP	/* 	[] 				*/
%token <opval> PARENOP	/* 	() 				*/
%token <opval> LISTOP	/* 	, ; : _ 		*/

/* identifier token */
%token <sval> IDENT
%token <sval> STRING

/* general keyword tokens */
%token <sval> PROGRAM
%token <ival> FUNCTION  /* count functions and procedures for code generation */
%token <ival> PROCEDURE

/* variable and array keyword tokens */
%token <sval> VAR
%token <sval> ARRAY OF
%token <sval> DOTDOT
%token <sval> INTEGER
%token <sval> REAL
%token <sval> BOOL

/* control flow keyword tokens */
%token <ival> IF WHILE FOR REPEAT /* count statements for assembly jumps */
%token <sval> BEG END
%token <sval> THEN THAN ELSE
%token <sval> DO
%token <sval> DOWNTO TO
%token <sval> UNTIL

/* empty token */
%token <sval> EMPTY

/* variables must also return correct value type */
%type <tval> start
%type <tval> program
%type <tval> ident_list
%type <tval> decls
%type <tval> type
%type <tval> std_type
%type <tval> subprogram_decls
%type <tval> subprogram_decl
%type <tval> subprogram_head
%type <tval> fn_header
%type <tval> proc_header
%type <tval> param_list
%type <tval> param
%type <tval> compound_stmt
%type <tval> opt_stmts
%type <tval> stmt_list
%type <tval> stmt
%type <tval> var
%type <tval> procedure_stmt
%type <tval> expr_list
%type <tval> expr
%type <tval> simple_expr
%type <tval> term
%type <tval> factor
%type <tval> id
%type <tval> fn
%type <tval> inum
%type <tval> rnum

/* order here specifies precedence */
%left ASSOP
%left RELOP
%left NOT
%left ADDOP
%left MULOP

%right THEN ELSE /* choose closest if statement */

/* set starting variable */
%start start

%%

start
	: program
		{ print_tree($1, 0); }

program
	: PROGRAM fn { push_table($2->attribute.sval, FUNCTION); } '(' ident_list ')' { add_io($5); } ';' decls subprogram_decls { main_header(); caller = top_table(); } compound_stmt '.'
		{
			$$ = str_tree(PROGRAM, "head body",
					op_tree(PARENOP, "()", $2, $5),
					str_tree(PROGRAM, "decls compound_stmt",
						str_tree(PROGRAM, "decls sub_decls", $9, $10),
					$12)
				);
			main_footer();
			print_table(top_table());
			pop_table();
		}
	;

ident_list
	: IDENT
		{
			$$ = op_tree(LISTOP, ",", 
					empty_tree(),
					str_tree(IDENT, $1, empty_tree(), empty_tree())
				);
		}
	| ident_list ',' IDENT
		{
			$$ = op_tree(LISTOP, ",", 
					$1,
					str_tree(IDENT, $3, empty_tree(), empty_tree())
				);
		}
	;

decls
	: decls VAR ident_list ':' type ';'
		{
			$$ = op_tree(LISTOP, ":",
					op_tree(VAR, $2, $1, $3),
				 $5);
			make_vars($3, $5);
		}
	| /* empty */
		{ $$ = empty_tree(); }
	;

type
	: std_type
		{ $$ = $1; }
	| ARRAY '[' inum DOTDOT inum ']' OF std_type
		{ 
			$$ = str_tree(ARRAY, "array-range type",
					op_tree(DOTDOT, $4, $3, $5),
				 $8);
		}
	;

std_type
	: INTEGER
		{ $$ = str_tree(INTEGER, $1, empty_tree(), empty_tree()); }
	| REAL
		{ $$ = str_tree(REAL, $1, empty_tree(), empty_tree()); }
	;

subprogram_decls
	: subprogram_decls subprogram_decl ';'
		{ $$ = op_tree(LISTOP, "_", $1, $2); }
	| /* empty */
		{ $$ = empty_tree(); }
	;

subprogram_decl
	: subprogram_head decls subprogram_decls { function_header($1); caller = top_table(); } compound_stmt
		{
			check_function($1, $5);
			$$ = op_tree(LISTOP, "_", $1, 
					op_tree(LISTOP, "_", $2, 
						op_tree(LISTOP, "_", $3, $5)
					)
				 );
			function_footer($1); 
			print_table(top_table());
			pop_table();
		}
	;

subprogram_head
	: FUNCTION fn_header ':' std_type ';'
		{
			$$ = int_tree(FUNCTION, $1, $2, $4);
			make_function($2, $4);
		}
	| PROCEDURE proc_header ';'
		{
			$$ = int_tree(PROCEDURE, $1, $2, empty_tree());
			make_procedure($2);
		}
	;

fn_header
	: fn { push_table($1->attribute.sval, FUNCTION); } '(' param_list ')'
		{ 
			add_params($4);
			$$ = op_tree(PARENOP, "()", $1, $4);
		}
	| fn { push_table($1->attribute.sval, FUNCTION); }
		{ $$ = $1; }
	;

proc_header
	: fn { push_table($1->attribute.sval, PROCEDURE); } '(' param_list ')'
		{ 
			add_params($4);
			$$ = op_tree(PARENOP, "()", $1, $4);
		}
	| fn { push_table($1->attribute.sval, PROCEDURE); }
		{ $$ = $1; }
	;

param_list
	: param
		{ $$ = op_tree(LISTOP, ";", empty_tree(), $1); }
	| param_list ';' param
		{ $$ = op_tree(LISTOP, ";", $1, $3); }
	;

param
	: ident_list ':' type
		{ $$ = op_tree(LISTOP, ":", $1, $3); }
	;

compound_stmt
	: BEG opt_stmts END
		{ $$ = str_tree(BEG, "begin-end", $2, empty_tree()); }
	;

opt_stmts
	: stmt_list
		{ $$ = $1; }
	| /* empty */
		{ $$ = empty_tree(); }
	;

stmt_list
	: stmt
		{ $$ = $1; }
	| stmt_list ';' stmt
		{ $$ = op_tree(LISTOP, ";", $1, $3); }
	;

stmt
	: var ASSOP expr
		{ 
			check_types($1, $3);
			$$ = op_tree(ASSOP, $2, $1, $3);
			assignment_gencode($$);
		}
	| procedure_stmt
		{ 
			$$ = $1;
			call_procedure($$);
		}
	| compound_stmt
		{ $$ = $1; }
 	| IF expr THEN { start_if_gencode($2, $1); } stmt 
		{
			$$ = str_tree(IF, "if then", $2, $5);
			enforce_type($2, BOOL);
			end_if_gencode($1);
		}
 	| IF expr THAN { start_if_else_gencode($2, $1); } stmt { mid_if_else_gencode($1); } ELSE stmt 
		{
			$$ = str_tree(IF, "if then-else", $2, str_tree(IF, "then else", $5, $8));
			enforce_type($2, BOOL);
			end_if_gencode(($1+1));
		}
	| WHILE expr { start_while_do_gencode($2, $1); } DO stmt
		{
			$$ = str_tree(WHILE, "while do", $2, $5);
			enforce_type($2, BOOL);
			end_while_do_gencode($1);
		}
	| REPEAT { start_repeat_until_gencode($1); } stmt UNTIL expr
		{
			$$ = str_tree(REPEAT, "repeat until", $3, $5);
			enforce_type($5, BOOL);
			end_repeat_until_gencode($5, $1);
		}
	| FOR var ASSOP expr TO expr DO { start_for_gencode($1, $2, $4, $5, $6); } stmt
		{
			check_types($4, $6);
			check_types($2, $4);
			$$ = str_tree(FOR, "for",
					op_tree(ASSOP, $3, $2, $4),
					str_tree(TO, "to do", $6, $9)
				);
			end_for_gencode($1, $2, $5);
		}
	| FOR var ASSOP expr DOWNTO expr DO { start_for_gencode($1, $2, $4, $5, $6); } stmt
		{
			check_types($4, $6);
			check_types($2, $4);
			$$ = str_tree(FOR, "for",
					op_tree(ASSOP, $3, $2, $4),
					str_tree(DOWNTO, "downto do", $6, $9)
				);
			end_for_gencode($1, $2, $5);
		}
	;

var
	: id
		{ $$ = $1; }
	| id '[' expr ']'
		{ $$ = op_tree(ARRAYOP, "[]", $1, $3); }
	;

procedure_stmt
	: id { $$ = $1; }
	| id '(' expr_list ')'
		{ 
			$$ = op_tree(PARENOP, "()", $1, $3);
			entry_t* fn_entry = find_entry(top_table(), $1->attribute.sval);
			check_args(fn_entry, $3);
		}
	;

expr_list
	: expr
		{ $$ = op_tree(LISTOP, ",", empty_tree(), $1); }
	| expr_list ',' expr
		{ $$ = op_tree(LISTOP, ",", $1, $3); }
	;

expr
	: simple_expr
		{ 
			tree_t* t = $1;
			number_tree(t);
			type(t);
			$$ = t;
		}
	| simple_expr RELOP simple_expr 	/* or.. recurse, allowing multiple relops */
		{ 
			tree_t* t = op_tree(RELOP, $2, $1, $3);
			number_tree(t);
			type(t);
			$$ = t;
		}
	;

simple_expr
	: term
		{ $$ = $1; }
	| ADDOP term						/* optional sign, ? */ 
		{ $$ = op_tree(ADDOP, $1, $2, empty_tree()); }
	| simple_expr ADDOP term
		{ $$ = op_tree(ADDOP, $2, $1, $3); }
	| STRING							/* ? */
		{ $$ = str_tree(STRING, $1, empty_tree(), empty_tree()); }
	;

term
	: factor
		{ $$ = $1; }
	| term MULOP factor
		{ $$ = op_tree(MULOP, $2, $1, $3); }
	;

factor
	: id
		{ $$ = $1; }
	| id '[' expr ']'
		{ $$ = op_tree(ARRAYOP, "[]", $1, $3); }
	| id '(' expr_list ')'
		{ $$ = op_tree(PARENOP, "()", $1, $3); }
	| inum
		{ $$ = $1; }
	| rnum
		{ $$ = $1; }
	| '(' expr ')'
		{ $$ = $2; }
	| NOT factor
		{ $$ = op_tree(NOT, $1, $2, empty_tree()); }
	;

id
	: IDENT
		{ $$ = str_tree(IDENT, $1, empty_tree(), empty_tree()); }
	;

fn
	: IDENT
		{ $$ = str_tree(IDENT, $1, empty_tree(), empty_tree()); }

inum
	: INUM
		{ $$ = int_tree(INUM, $1, empty_tree(), empty_tree()); }
	;

rnum
	: RNUM
		{ $$ = float_tree(RNUM, $1, empty_tree(), empty_tree()); }
	;

%%
