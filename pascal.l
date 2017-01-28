%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "y.tab.h"
%}

identifier	[a-zA-z][a-zA-z0-9]*
inum		[0-9]+
rnum		[0-9]+\.[0-9]+
relation_op "<" | ">" | "=" | ">=" | "<=" | "<>"
addition_op "+" | "-" | "or"
mult_op		"*" | "/" | "and"
dotdot		\.\.
whitespace 	[ \t]*
endline		[\n\r]

%%

program			{ fprintf( stderr, "[PROGRAM]" ); return PROGRAM; }
:=				{ fprintf( stderr, "[ASSIGNOP]" ); return ASSIGNOP; }
var				{ fprintf( stderr, "[VAR]" ); return VAR; }
array			{ fprintf( stderr, "[ARRAY]" ); return ARRAY; }
integer			{ fprintf( stderr, "[INTEGER]" ); return INTEGER; }
real			{ fprintf( stderr, "[REAL]" ); return REAL; }
of				{ fprintf( stderr, "[OF]" ); return OF; }
function		{ fprintf( stderr, "[FUNCTION]" ); return FUNCTION; }
procedure		{ fprintf( stderr, "[PROCEDURE]" ); return PROCEDURE; }
begin			{ fprintf( stderr, "[BEGIN]" ); return BEGIN; }
end				{ fprintf( stderr, "[END]" ); return END; }
if				{ fprintf( stderr, "[IF]" ); return IF; }
then			{ fprintf( stderr, "[THEN]" ); return THEN; }
else			{ fprintf( stderr, "[ELSE]" ); return ELSE; }
do				{ fprintf( stderr, "[DO]" ); return DO; }
while			{ fprintf( stderr, "[WHILE]" ); return WHILE; }
not				{ fprintf( stderr, "[NOT]" ); return NOT; }
{relation_op}	{ fprintf( stderr, "[RELOP]" ); return RELOP; }	/* operators need to return ids */
{addition_op}	{ fprintf( stderr, "[ADDOP]" ); return ADDOP; }
{mult_op}		{ fprintf( stderr, "[MULOP]" ); return MULOP; }
{dotdot}		{ fprintf( stderr, "[DOTDOT]" ); return DOTDOT; }
{rnum}			{ fprintf( stderr, "[RNUM:%f]", yylval.fval = atof(yytext) ); return RNUM; }
{inum}			{ fprintf( stderr, "[INUM:%d]", yylval.ival = atoi(yytext) ); return INUM; }
{identifier}	{ fprintf( stderr, "[IDENT:%s]", yylval.sval = yytext ); return IDENT; }
{whitespace} 	{ ; }
{endline} 		{ fprintf( stderr, "[EOL]" ); return yytext[0]; }
.				{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }

%%
