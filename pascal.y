%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
%}

%union { // current attribute
	int ival;
	float fval;
	int opval;
	char *sval;
}

/* these tokens must return the correct value type */
%token <ival> INUM
%token <fval> RNUM
%token <sval> IDENT
%token PROGRAM ASSIGNOP VAR ARRAY INTEGER REAL OF FUNCTION PROCEDURE BEGIN END IF THEN ELSE DO WHILE NOT RELOP ADDOP MULOP DOTDOT

/* the variable expr will return an integer value */
%type <ival> expr

/* order here specifies precedence */
%left '+' '-'
%left '*' '/'

%%

start: program	{ fprintf( stderr, "\nvalue = %d\n", $1 ); }
	 ;

program: PROGRAM IDENT '(' ident_list ')' ';' decls subprogram_decls compound_stmt '.'
	   ;

ident_list: IDENT
		  | ident_list ',' IDENT
		  ;

decls: decls VAR ident_list ':' type ':'
	 | ''
	 ;

type: std_type
	| ARRAY '[' INUM DOTDOT INUM ']' OF std_type
	;

std_type: INUM
		| RNUM
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
