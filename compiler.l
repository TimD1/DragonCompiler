%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"

int function_id = 1;

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

 /* define exclusive comment states */
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
{rnum}			{ 
					fprintf( stderr, " [RNUM:%f]", yylval.fval = atof(yytext) ); 
					return RNUM; 
				}
{inum}			{ 
					fprintf( stderr, " [INUM:%d]", yylval.ival = atoi(yytext) ); 
					return INUM; 
				}

 /* operator tokens */
{relation_op}	{ 
					fprintf( stderr, " [RELOP:%s]", yylval.opval = strdup(yytext) ); 
					return RELOP; 
				} 
{addition_op}	{
					fprintf( stderr, " [ADDOP:%s]", yylval.opval = strdup(yytext) );
					return ADDOP;
}
{mult_op}		{
					fprintf( stderr, " [MULOP:%s]", yylval.opval = strdup(yytext) );
					return MULOP;
				}
:=				{
					fprintf( stderr, " [ASSOP]" );
					yylval.opval = strdup(yytext);
					return ASSOP;
				}
not				{
					fprintf( stderr, " [NOT]" );
					yylval.opval = strdup(yytext);
					return NOT;
				}

 /* general keyword tokens */
program			{
					fprintf( stderr, " [PROGRAM]" );
					yylval.sval = strdup(yytext);
					return PROGRAM;
				}
function		{
					fprintf( stderr, " [FUNCTION]" );
					yylval.ival = function_id;
					function_id++;
					return FUNCTION;
				}
procedure		{
					fprintf( stderr, " [PROCEDURE]" );
					yylval.ival = function_id;
					function_id++;
					return PROCEDURE;
				}

 /* variable and array keyword tokens */
var				{
					fprintf( stderr, " [VAR]" );
					yylval.sval = strdup(yytext);
					return VAR;
				}
array			{
					fprintf( stderr, " [ARRAY]" );
					yylval.sval = strdup(yytext);
					return ARRAY;
				}
of				{
					fprintf( stderr, " [OF]" );
					yylval.sval = strdup(yytext);
					return OF;
				}
{dotdot}		{
					fprintf( stderr, " [DOTDOT]" );
					yylval.sval = strdup(yytext);
					return DOTDOT;
				}
integer			{
					fprintf( stderr, " [INTEGER]" );
					yylval.sval = strdup(yytext);
					return INTEGER;
				}
real			{
					fprintf( stderr, " [REAL]" );
					yylval.sval = strdup(yytext);
					return REAL;
				}

 /* control-flow keyword tokens */
if				{
					fprintf( stderr, " [IF]" );
					yylval.sval = strdup(yytext);
					return IF;
				}
then			{
					fprintf( stderr, " [THEN]" );
					yylval.sval = strdup(yytext);
					return THEN;
				}
else			{
					fprintf( stderr, " [ELSE]" );
					yylval.sval = strdup(yytext);
					return ELSE;
				}
do				{
					fprintf( stderr, " [DO]" );
					yylval.sval = strdup(yytext);
					return DO;
				}
while			{
					fprintf( stderr, " [WHILE]" );
					yylval.sval = strdup(yytext);
					return WHILE;
				}
for				{
					fprintf( stderr, " [FOR]" );
					yylval.sval = strdup(yytext);
					return FOR;
				}
to				{
					fprintf( stderr, " [TO]" );
					yylval.sval = strdup(yytext);
					return TO;
				}
"down to"		{
					fprintf( stderr, " [DOWNTO]" );
					yylval.sval = strdup(yytext);
					return DOWNTO;
				}
repeat			{
					fprintf( stderr, " [REPEAT]" );
					yylval.sval = strdup(yytext);
					return REPEAT;
				}
until			{
					fprintf( stderr, " [UNTIL]" );
					yylval.sval = strdup(yytext);
					return UNTIL;
				}
begin			{
					fprintf( stderr, " [BEGIN]" );
					yylval.sval = strdup(yytext);
					return BEG;
				}
end				{
					fprintf( stderr, " [END]" );
					yylval.sval = strdup(yytext);
					return END;
				}

 /* identifier tokens */
{identifier}	{
					fprintf( stderr, " [IDENT:%s]", yylval.sval = strdup(yytext));
					return IDENT;
				}
{string}		{
					fprintf( stderr, " [STRING:%s]", yylval.sval = strdup(yytext) );
					return STRING;
				}

 /* ignore whitespace and set default */
{whitespace} 	{ ; }
{endline} 		{
					fprintf( stderr, "\n%d ", yylineno );
				}
.				{
					fprintf( stderr, " {%c}", yytext[0] );
					return yytext[0];
				}

%%
