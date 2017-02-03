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

/* tokens (terminals) must return correct value type */
%token <ival> INUM
%token <fval> RNUM

%token <opval> ADDOP
%token <opval> MULOP
%token <opval> RELOP
%token <opval> ASSIGNOP
%token <opval> NOT		/* ? */

%token <sval> IDENT
%token <sval> PROGRAM
%token <sval> VAR
%token <sval> ARRAY
%token <sval> INTEGER
%token <sval> REAL
%token <sval> OF
%token <sval> FUNCTION
%token <sval> PROCEDURE
%token <sval> BEG
%token <sval> END
%token <sval> IF
%token <sval> THEN
%token <sval> ELSE
%token <sval> DO
%token <sval> WHILE
%token <sval> DOTDOT	/* ? */

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
%type <tval> args
%type <tval> param_list
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

/* order here specifies precedence */
%left ASSIGNOP
%left ADDOP
%left MULOP
%left RELOP
%left NOT


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

std_type: INTEGER
		| REAL
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
