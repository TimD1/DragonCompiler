%{
/* negative numbers */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"
%}

identifier	[a-zA-z][a-zA-z0-9]*
inum		[0-9]+
rnum		[0-9]*"."[0-9]+
relation_op "<"|">"|"="|">="|"<="|"<>"
addition_op "+"|"-"|"or"
mult_op		"*"|"/"|"and"
dotdot		".."
comment 	"{"[^}\n]*"}"
whitespace 	[ \t]+
endline		[\r\n]

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
begin			{ fprintf( stderr, "[BEGIN]" ); return BEG; }
end				{ fprintf( stderr, "[END]" ); return END; }
if				{ fprintf( stderr, "[IF]" ); return IF; } /* may need to rename */
then			{ fprintf( stderr, "[THEN]" ); return THEN; }
else			{ fprintf( stderr, "[ELSE]" ); return ELSE; }
do				{ fprintf( stderr, "[DO]" ); return DO; }
while			{ fprintf( stderr, "[WHILE]" ); return WHILE; } /* may need to rename */
not				{ fprintf( stderr, "[NOT]" ); return NOT; }
{relation_op}	{ fprintf( stderr, "[RELOP:%s]", yylval.sval = strdup(yytext) ); return RELOP; } 
{addition_op}	{ fprintf( stderr, "[ADDOP:%s]", yylval.sval = strdup(yytext) ); return ADDOP; }
{mult_op}		{ fprintf( stderr, "[MULOP:%s]", yylval.sval = strdup(yytext) ); return MULOP; }
{dotdot}		{ fprintf( stderr, "[DOTDOT]" ); return DOTDOT; }
{rnum}			{ fprintf( stderr, "[RNUM:%f]", yylval.fval = atof(yytext) ); return RNUM; }
{inum}			{ fprintf( stderr, "[INUM:%d]", yylval.ival = atoi(yytext) ); return INUM; }
{identifier}	{ fprintf( stderr, "[IDENT:%s]", yylval.sval = yytext ); return IDENT; }
{whitespace} 	{ ; }
{comment} 		{ ; }
{endline} 		{ fprintf( stderr, "[EOL]\n" ); return yytext[0]; }
.				{ fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; } /* error? */

%%
