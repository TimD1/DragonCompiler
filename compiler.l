%{
/* negative numbers */
/* which types of comments to use? */
/* strings? not handled right */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"
%}

 /* regular expressions */
identifier	[a-zA-Z][a-zA-Z0-9]*
inum		[0-9]+
rnum		[0-9]*"."[0-9]+
relation_op "<"|">"|"="|">="|"<="|"<>"
addition_op "+"|"-"|"or"
mult_op		"*"|"/"|"and"
dotdot		".."
whitespace 	[ \t]+
endline		[\r\n]
string		"'"[a-zA-Z0-9 ]*"'"

 /* define exlusive comment states */
%x		COMMENT1
%x 		COMMENT2
%x		COMMENT3

%%

 /* detect comment states */
"{"					{ BEGIN COMMENT1; }
<COMMENT1>[^}] 		{ ; }
<COMMENT1>"}"		{ fprintf(stderr, " {COMMENT}"); BEGIN INITIAL; }

"(*"				{ BEGIN COMMENT2; }
<COMMENT2>. 		|
<COMMENT2>\n 		{ ; }
<COMMENT2>"*)"		{ fprintf(stderr, " (*COMMENT*)"); BEGIN INITIAL; }

"//"				{ BEGIN COMMENT3; }
<COMMENT3>. 		{ ; }
<COMMENT3>\n 		{ fprintf(stderr, " //COMMENT"); BEGIN INITIAL; }

 /* find all tokens */
program			{ fprintf( stderr, " [PROGRAM]" ); return PROGRAM; }
:=				{ fprintf( stderr, " [ASSIGNOP]" ); return ASSIGNOP; }
var				{ fprintf( stderr, " [VAR]" ); return VAR; }
array			{ fprintf( stderr, " [ARRAY]" ); return ARRAY; }
integer			{ fprintf( stderr, " [INTEGER]" ); return INTEGER; }
real			{ fprintf( stderr, " [REAL]" ); return REAL; }
of				{ fprintf( stderr, " [OF]" ); return OF; }
function		{ fprintf( stderr, " [FUNCTION]" ); return FUNCTION; }
procedure		{ fprintf( stderr, " [PROCEDURE]" ); return PROCEDURE; }
begin			{ fprintf( stderr, " [BEGIN]" ); return BEG; }
end				{ fprintf( stderr, " [END]" ); return END; }
if				{ fprintf( stderr, " [IF]" ); return IF; }
then			{ fprintf( stderr, " [THEN]" ); return THEN; }
else			{ fprintf( stderr, " [ELSE]" ); return ELSE; }
do				{ fprintf( stderr, " [DO]" ); return DO; }
while			{ fprintf( stderr, " [WHILE]" ); return WHILE; }
not				{ fprintf( stderr, " [NOT]" ); return NOT; }
{relation_op}	{ fprintf( stderr, " [RELOP:%s]", yylval.sval = strdup(yytext) ); return RELOP; } 
{addition_op}	{ fprintf( stderr, " [ADDOP:%s]", yylval.sval = strdup(yytext) ); return ADDOP; }
{mult_op}		{ fprintf( stderr, " [MULOP:%s]", yylval.sval = strdup(yytext) ); return MULOP; }
{dotdot}		{ fprintf( stderr, " [DOTDOT]" ); return DOTDOT; }
{rnum}			{ fprintf( stderr, " [RNUM:%f]", yylval.fval = atof(yytext) ); return RNUM; }
{inum}			{ fprintf( stderr, " [INUM:%d]", yylval.ival = atoi(yytext) ); return INUM; }
{identifier}	{ fprintf( stderr, " [IDENT:%s]", yylval.sval = yytext ); return IDENT; }
{string}		{ fprintf( stderr, " [STRING:%s]", yylval.sval = yytext ); return STRING; }
{whitespace} 	{ ; }
{endline} 		{ fprintf( stderr, "\n" ); }
.				{ fprintf( stderr, " {%c}", yytext[0] ); return yytext[0]; }

%%
