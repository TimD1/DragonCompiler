%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "hash.h"
#include "y.tab.h"
int yydebug = 1;

int yywrap() { return 1; }

void yyerror(const char *str) { fprintf(stderr, "error: %s\n", str); }

int main() 
{ 
	/* Create base table for doubly linked "stack" of all tables 
	   tables on top have narrower scope, and prev is the top table */
	head_table = (table_t*)malloc(sizeof(table_t));
	for(int i = 0; i < TABLE_SIZE; i++)
		head_table->hash_table[i] = NULL;
	head_table->prev = head_table;
	head_table->next = head_table;
	head_table->id = -1;

	yyparse();
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
%token <sval> FUNCTION
%token <sval> PROCEDURE

/* variable and array keyword tokens */
%token <sval> VAR
%token <sval> ARRAY OF
%token <sval> DOTDOT
%token <sval> INTEGER
%token <sval> REAL

/* control flow keyword tokens */
%token <sval> BEG END
%token <sval> IF THEN ELSE
%token <sval> DO WHILE
%token <sval> FOR DOWNTO TO
%token <sval> REPEAT UNTIL

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
%left ADDOP
%left MULOP
%left RELOP
%left NOT

%right THEN ELSE /* choose closest if statement */

/* set starting variable */
%start start

%%

start
	: program
		{ print_tree($1, 0); }

program
	: PROGRAM fn { push_table($2->attribute.sval, FUNCTION); } '(' ident_list ')' ';' decls subprogram_decls compound_stmt '.'
		{
			add_io($5);
			$$ = str_tree(PROGRAM, "head body",
					op_tree(PARENOP, "()", $2, $5),
					str_tree(PROGRAM, "decls compound_stmt",
						str_tree(PROGRAM, "decls sub_decls", $8, $9),
					$10)
				);
			print_table(top_table());
			pop_table();
		}
	;

ident_list
	: IDENT
		{
			$$ = op_tree(LISTOP, ",", 
					empty_tree(),
					str_tree(IDENT, $1, NULL, NULL)
				);
		}
	| ident_list ',' IDENT
		{
			$$ = op_tree(LISTOP, ",", 
					$1,
					str_tree(IDENT, $3, NULL, NULL)
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
		{ $$ = str_tree(INTEGER, $1, NULL, NULL); }
	| REAL
		{ $$ = str_tree(REAL, $1, NULL, NULL); }
	;

subprogram_decls
	: subprogram_decls subprogram_decl ';'
		{ op_tree(LISTOP, "_", $1, $2); }
	| /* empty */
		{ $$ = empty_tree(); }
	;

subprogram_decl
	: subprogram_head decls subprogram_decls compound_stmt
		{
			check_function($1, $4);
			$$ = op_tree(LISTOP, "_", $1, 
					op_tree(LISTOP, "_", $2, 
						op_tree(LISTOP, "_", $3, $4)
					)
				 );
			print_table(top_table());
			pop_table();
		}
	;

subprogram_head
	: FUNCTION fn_header ':' std_type ';'
		{
			$$ = str_tree(FUNCTION, "function type", $2, $4);
			make_function($2, $4);
		}
	| PROCEDURE proc_header ';'
		{
			$$ = str_tree(PROCEDURE, $1, $2, empty_tree());
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
		{
			$$ = str_tree(BEG, "begin-end", $2, empty_tree());
		}
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
		}
	| procedure_stmt
		{ $$ = $1; }
	| compound_stmt
		{ $$ = $1; }
	| IF expr THEN stmt
		{ $$ = str_tree(IF, "if then", $2, $4); }
	| IF expr THEN stmt ELSE stmt
		{ $$ = str_tree(IF, "if then-else", $2, str_tree(IF, "then else", $4, $6)); }
	| WHILE expr DO stmt
		{ $$ = str_tree(WHILE, "while do", $2, $4); }
	| REPEAT stmt UNTIL expr
		{ $$ = str_tree(REPEAT, "repeat until", $2, $4); }
	| FOR var ASSOP expr TO expr DO stmt
		{
			check_types($4, $6);
			check_types($2, $4);
			$$ = str_tree(FOR, $1,
					op_tree(ASSOP, $3, $2, $4),
					str_tree(TO, "to do", $6, $8)
				);
		}
	| FOR var ASSOP expr DOWNTO expr DO stmt
		{
			$$ = str_tree(FOR, $1,
					op_tree(ASSOP, $3, $2, $4),
					str_tree(DOWNTO, "downto do", $6, $8)
				);
		}
	;

var
	: id
		{ $$ = $1; }
	| id '[' expr ']'
		{ $$ = op_tree(ARRAYOP, "[]", $1, $3); }
	;

procedure_stmt
	: id
		{ $$ = $1; }
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
			type(t);
			$$ = t;
		}
	| simple_expr RELOP simple_expr 	/* or.. recurse, allowing multiple relops */
		{ 
			tree_t* t = op_tree(RELOP, $2, $1, $3);
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
		{ $$ = str_tree(STRING, $1, NULL, NULL); }
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
		{ $$ = str_tree(IDENT, $1, NULL, NULL); }
	;

fn
	: IDENT
		{ $$ = str_tree(IDENT, $1, NULL, NULL); }

inum
	: INUM
		{ $$ = int_tree(INUM, $1, NULL, NULL); }
	;

rnum
	: RNUM
		{ $$ = float_tree(RNUM, $1, NULL, NULL); }
	;

%%
