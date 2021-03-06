%{
	#include <string>
	#include <stdlib.h>
	#include "new_ast.hpp"
	#include "new_parser.tab.h"
	#define YY_DECL extern "C" int yylex()
%}
%option yylineno


%%
("//".*\n)|([ \t\n]+)	;

"("							{return T_OPENPAR;}
")"							{return T_CLOSEPAR;}
"["							{return T_OPENBRAC;}
"]"							{return T_CLOSEBRAC;}
"{"							{return T_OPENCURL;}
"}"							{return T_CLOSECURL;}
","							{return T_COMMA;}
";"							{return T_SEMICOL;}
"="							{return T_ASSIGN;}
"+"							{return T_PLUS;}
"-"							{return T_MINUS;}
"*" 						{return T_MUL;}
"/" 						{return T_DIV;}
"<" 						{return T_LT;}
">"							{return T_GT;}
"<=" 						{return T_LTE;}
">=" 						{return T_GTE;}
"==" 						{return T_EQUAL;}
"!=" 						{return T_NOTEQUAL;}
"&&" 						{return T_AND;}
"||" 						{return T_OR;}
"!"							{return T_NOT;}

"let"						{return T_LET;}
"def"						{return T_DEF;}
"if"						{return T_IF;}
"else"					{return T_ELSE;}
"while"					{return T_WHILE;}
"return"				{return T_RETURN;}
"break"					{return T_BREAK;}
"continue"			{return T_CONTINUE;}

[a-zA-Z][a-zA-Z0-9_]*		{yylval.sval = strdup(yytext); return T_ID;}
[0-9]+									{yylval.sval = strdup(yytext); return T_NUMBER;}

.				{exit(1);}

%%

int yywrap(void) {
	return 1;
}

//int main(int argc, char *argv[]) {
//	yyin = fopen(argv[1], "r");
//	yyout = fopen(argv[2], "w");
//	yylex();
//	fclose(yyin);
//	fclose(yyout);
//	return 0;
//}