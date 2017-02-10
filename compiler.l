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

 /* record line number */
%option yylineno

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

 /* number tokens */
{rnum}			{ fprintf( stderr, " [RNUM:%f]", yylval.fval = atof(yytext) ); return RNUM; }
{inum}			{ fprintf( stderr, " [INUM:%d]", yylval.ival = atoi(yytext) ); return INUM; }

 /* operator tokens */
{relation_op}	{ fprintf( stderr, " [RELOP:%s]", yylval.opval = strdup(yytext) ); return RELOP; } 
{addition_op}	{ fprintf( stderr, " [ADDOP:%s]", yylval.opval = strdup(yytext) ); return ADDOP; }
{mult_op}		{ fprintf( stderr, " [MULOP:%s]", yylval.opval = strdup(yytext) ); return MULOP; }
:=				{ fprintf( stderr, " [ASSOP]" ); return ASSOP; }
not				{ fprintf( stderr, " [NOT]" ); return NOT; }

 /* general keyword tokens */
program			{ fprintf( stderr, " [PROGRAM]" ); return PROGRAM; }
function		{ fprintf( stderr, " [FUNCTION]" ); return FUNCTION; }
procedure		{ fprintf( stderr, " [PROCEDURE]" ); return PROCEDURE; }

 /* variable and array keyword tokens */
var				{ fprintf( stderr, " [VAR]" ); return VAR; }
array			{ fprintf( stderr, " [ARRAY]" ); return ARRAY; }
of				{ fprintf( stderr, " [OF]" ); return OF; }
{dotdot}		{ fprintf( stderr, " [DOTDOT]" ); return DOTDOT; }
integer			{ fprintf( stderr, " [INTEGER]" ); return INTEGER; }
real			{ fprintf( stderr, " [REAL]" ); return REAL; }

 /* control-flow keyword tokens */
if				{ fprintf( stderr, " [IF]" ); return IF; }
then			{ fprintf( stderr, " [THEN]" ); return THEN; }
else			{ fprintf( stderr, " [ELSE]" ); return ELSE; }
do				{ fprintf( stderr, " [DO]" ); return DO; }
while			{ fprintf( stderr, " [WHILE]" ); return WHILE; }
for				{ fprintf( stderr, " [FOR]" ); return FOR; }
to				{ fprintf( stderr, " [TO]" ); return TO; }
"down to"		{ fprintf( stderr, " [DOWNTO]" ); return DOWNTO; }
repeat			{ fprintf( stderr, " [REPEAT]" ); return REPEAT; }
until			{ fprintf( stderr, " [UNTIL]" ); return UNTIL; }
begin			{ fprintf( stderr, " [BEGIN]" ); return BEG; }
end				{ fprintf( stderr, " [END]" ); return END; }

 /* identifier tokens */
{identifier}	{ fprintf( stderr, " [IDENT:%s]", yylval.sval = yytext ); return IDENT; }
{string}		{ fprintf( stderr, " [STRING:%s]", yylval.sval = yytext ); return STRING; }

 /* ignore whitespace and set default */
{whitespace} 	{ ; }
{endline} 		{ fprintf( stderr, "\n" ); }
.				{ fprintf( stderr, " {%c}", yytext[0] ); return yytext[0]; }

%%
