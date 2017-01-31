%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int yydebug = 1;
%}

%union { // current attribute
	int ival;
	float fval;
	int opval;
	char *sval;

	tree_t *tval;
}

/* these tokens must return the correct value type */
%token <ival> INUM
%token <fval> RNUM
%token <sval> IDENT
%token PROGRAM ASSIGNOP VAR ARRAY INTEGER REAL OF FUNCTION PROCEDURE BEG END IF THEN ELSE DO WHILE NOT RELOP ADDOP MULOP DOTDOT

/* the variable expr will return an integer value */
/* %type <ival> expr */

/* order here specifies precedence */
%left '+' '-'
%left '*' '/'

%%

start: program	/* { fprintf( stderr, "\nvalue = %d\n", $1 ); } */
	 ;

program: PROGRAM IDENT '(' ident_list ')' ';' decls subprogram_decls compound_stmt '.'
	   ;

ident_list: IDENT
		  | ident_list ',' IDENT
		  ;

decls: decls VAR ident_list ':' type ':'
	 | %empty
	 ;

type: std_type
	| ARRAY '[' INUM DOTDOT INUM ']' OF std_type
	;

std_type: INUM
		| RNUM
		;

subprogram_decls: subprogram_decls subprogram_decl ';'
				| %empty
				;

subprogram_decl: subprogram_head decls subprogram_decls compound_stmt
			   ;

subprogram_head: FUNCTION IDENT args ':' std_type ';'
			   | PROCEDURE IDENT args ';'
			   ;

args: '(' param_list ')'
	| %empty
	;

param_list: ident_list ':' type
		  | param_list ';' ident_list ':' type
		  ;

compound_stmt: BEG opt_stmts END
			 ;

opt_stmts: stmt_list
		 | %empty
		 ;

stmt_list: stmt
		 | stmt_list ';' stmt
		 ;

stmt: var ASSIGNOP expr
	| procedure_stmt
	| compound_stmt
	| IF expr THEN stmt ELSE stmt
	| WHILE stmt DO stmt
	;

var: IDENT
   | IDENT '[' expr ']'
   ;

procedure_stmt: IDENT
			  | IDENT '(' expr_list ')'
			  ;

expr_list: expr
			   | expr_list ',' expr
			   ;

expr: simple_expr
		  | simple_expr RELOP simple_expr
		  ;

simple_expr: term
				 | ADDOP term
				 | simple_expr ADDOP term
				 ;

term: factor
	| term MULOP factor
	;

factor: IDENT
	  | IDENT '[' expr ']'
	  | IDENT '(' expr_list ')'
	  | INUM
	  | RNUM
	  | '(' expr ')'
	  | NOT factor
	  ;


/* expr: expr '+' expr { $$ = $1 + $3; } $2 is the plus sign
	| expr '*' expr	{ $$ = $1 * $3; } $$ is the integer value
	| '(' expr ')'	{ $$ = $2; }
	| NUM			{ $$ = $1; }
	;
*/

%%

int main()
{
	yyparse();
}
