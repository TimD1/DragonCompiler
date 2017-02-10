%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"
int yydebug = 1;

int yywrap() { return 1; }
void yyerror(const char *str) { fprintf(stderr, "error: %s\n", str); }
int main() { yyparse(); }
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
%token <opval> ADDOP
%token <opval> MULOP
%token <opval> RELOP
%token <opval> ASSOP
%token <opval> NOT		/* ? */

/* identifier token */
%token <sval> IDENT
%token <sval> STRING

/* general keyword tokens */
%token PROGRAM
%token FUNCTION
%token PROCEDURE

/* variable and array keyword tokens */
%token VAR
%token ARRAY OF
%token DOTDOT	/* ? */
%token INTEGER
%token REAL

/* control flow keyword tokens */
%token BEG END
%token IF THEN ELSE
%token DO WHILE
%token FOR DOWNTO TO
%token REPEAT UNTIL


/* variables must also return correct value type */
/* %type <tval> start */
/* %type <tval> program */
/* %type <tval> ident_list */
/* %type <tval> decls */
/* %type <tval> type */
/* %type <tval> std_type */
/* %type <tval> subprogram_decls */
/* %type <tval> subprogram_decl */
/* %type <tval> subprogram_head */
/* %type <tval> args */
/* %type <tval> param_list */
/* %type <tval> compound_stmt */
/* %type <tval> opt_stmts */
/* %type <tval> stmt_list */
/* %type <tval> stmt */
/* %type <tval> var */
/* %type <tval> procedure_stmt */
/* %type <tval> expr_list */
/* %type <tval> expr */
/* %type <tval> simple_expr */
/* %type <tval> term */
/* %type <tval> factor */

/* order here specifies precedence */
%left ASSOP
%left ADDOP
%left MULOP
%left RELOP
%left NOT

%right THEN ELSE /* choose closest if statement */

/* set starting variable */
%start program

%%

program
	: PROGRAM IDENT '(' ident_list ')' ';' decls subprogram_decls compound_stmt '.'
	;

ident_list
	: IDENT
	| ident_list ',' IDENT
	;

decls
	: decls VAR ident_list ':' type ';'
	| /* empty */
	;

type
	: std_type
	| ARRAY '[' INUM DOTDOT INUM ']' OF std_type
	;

std_type
	: INTEGER
	| REAL
	;

subprogram_decls
	: subprogram_decls subprogram_decl ';'
	| /* empty */
	;

subprogram_decl
	: subprogram_head decls subprogram_decls compound_stmt
	;

subprogram_head
	: FUNCTION IDENT args ':' std_type ';'
	| PROCEDURE IDENT args ';'
	;

args
	: '(' param_list ')'
	| /* empty */
	;

param_list
	: ident_list ':' type
	| param_list ';' ident_list ':' type
	;

compound_stmt
	: BEG opt_stmts END
	;

opt_stmts
	: stmt_list
	| /* empty */
	;

stmt_list
	: stmt
	| stmt_list ';' stmt
	;

stmt
	: var ASSOP expr
	| procedure_stmt
	| compound_stmt
	| IF expr THEN stmt	
	| IF expr THEN stmt ELSE stmt
	| WHILE expr DO stmt
	| REPEAT stmt UNTIL expr
	| FOR var ASSOP expr TO expr DO stmt
	| FOR var ASSOP expr DOWNTO expr DO stmt
	;

var
	: IDENT
	| IDENT '[' expr ']'
	;

procedure_stmt
	: IDENT
	| IDENT '(' expr_list ')'
	;

expr_list
	: expr
	| expr_list ',' expr
	;

expr
	: simple_expr
	| expr RELOP simple_expr 			/* allow multiple relops */
	;

simple_expr
	: term
	| ADDOP term						/* optional sign */
	| simple_expr ADDOP term
	| STRING							/* ? */
	;

term
	: factor
	| term MULOP factor
	;

factor
	: IDENT
	| IDENT '[' expr ']'
	| IDENT '(' expr_list ')'
	| INUM
	| RNUM
	| '(' expr ')'
	| NOT factor
	;

%%
