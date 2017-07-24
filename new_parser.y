%{
#include <stdlib.h>
#include <stdio.h>
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" FILE *yyout;
//extern "C" FILE *yyin;
//#include "node.h"
//NBlock *programBlock; /* the top level root node of our final AST */

void yyerror(const char *s);


%}
/*
%union {
		Node *node;
		NBlock *block;
		NExpression *expr;
		NStatement *stmt;
		NIdentifier *ident;
		NVariableDeclaration *var_decl;
		std::vector<NVariableDeclaration*> *varvec;
		std::vector<NExpression*> *exprvec;
		std::string *string;
		int token;
}
*/

/* Define our terminal symbols (tokens). This should
	 match our tokens.l lex file. We also define the node type
	 they represent.
 */

%token T_OPENPAR;
%token T_CLOSEPAR;
%token T_OPENBRAC;
%token T_CLOSEBRAC;
%token T_OPENCURL;
%token T_CLOSECURL;
%token T_COMMA;
%token T_SEMICOL;
%token T_ASSIGN;
%token T_PLUS;
%token T_MINUS;
%token T_MUL;
%token T_DIV;
%token T_LT;
%token T_GT;
%token T_LTE;
%token T_GTE;
%token T_EQUAL;
%token T_NOTEQUAL;
%token T_AND;
%token T_OR;
%token T_NOT;

%token T_LET;
%token T_DEF;
%token T_IF;
%token T_ELSE;
%token T_WHILE;
%token T_BREAK;
%token T_RETURN;
%token T_CONTINUE;        

%token T_ID;
%token T_NUMBER;

%token T_ERROR;



/* Define the type of node our nonterminal symbols represent.
	 The types refer to the %union declaration above. Ex: when
	 we call an ident (defined by union type ident) we are really
	 calling an (NIdentifier*). It makes the compiler happy.

%type <ident> ident
%type <expr> numeric expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> Program Stmt Block
%type <stmt> stmt var_decl func_decl
%type <token> comparison
 */





%left T_LT T_GT T_LTE T_GTE T_EQUAL T_NOTEQUAL T_AND T_OR
%left  T_PLUS T_MINUS
%left  T_MUL T_DIV
%left  T_NOT

%start Program


%%

Program:
	Program DecVar                                                           {}
	| Program DecFunc	                                                     {}
	| %empty                                                                 {}
;

DecVar:
	T_LET T_ID AssignExprOrNothing T_SEMICOL
;

AssignExprOrNothing:
	T_ASSIGN Expr        													 {}
	| %empty             													 {}
;

DecFunc:
	T_DEF T_ID T_OPENPAR ParamListOrNothing T_CLOSEPAR Block                 {}
;

ParamListOrNothing:
	ParamList            													 {}
	| %empty             													 {}
;




ParamList:
	T_ID NCommaIdOrNothing 													 {}
;

NCommaIdOrNothing:
	NCommaIdOrNothing T_COMMA T_ID    										 {}
	| %empty																 {}

Block:
	T_OPENCURL NDecVarOrNothing NStmtOrNothing T_CLOSECURL

NDecVarOrNothing:
	NDecVarOrNothing DecVar 												 {}
	| %empty																 {}
;

NStmtOrNothing:
	NStmtOrNothing Stmt 													 {}
	| %empty																 {}
;


Stmt:
	Assign T_SEMICOL														 {}
	|FuncCall T_SEMICOL														 {}
	|T_IF T_OPENPAR Expr T_CLOSEPAR Block ElseBlockOrNothing				 {}
	|T_WHILE T_OPENPAR Expr T_CLOSEPAR Block 								 {}
	|T_RETURN ExprOrNothing T_SEMICOL										 {}
	|T_BREAK T_SEMICOL														 {}
	|T_CONTINUE T_SEMICOL													 {}
;

ElseBlockOrNothing:
	T_ELSE Block 															 {}
	| %empty																 {}
;

ExprOrNothing:
	Expr 																	 {}
	| %empty 																 {}
;

Assign:
	T_ID T_ASSIGN Expr 														 {}
;

FuncCall:
	T_ID T_OPENPAR ArgListOrNothing T_CLOSEPAR								 {}
;

ArgListOrNothing:
	ArgList 																 {}
	| %empty																 {}
;

ArgList:
	Expr NCommaExprOrNothing 												 {}
;

NCommaExprOrNothing:
	NCommaExprOrNothing T_COMMA Expr 										 {}
	| %empty
;

Expr:
	Expr BinOp Expr 														 {}
	|UnOp Expr 																 {}
	|T_OPENPAR Expr T_CLOSEPAR 												 {}
	|FuncCall 																 {}
	|T_NUMBER 																 {}
	|T_ID 																	 {}
;

BinOp:
	T_PLUS																	 {}
	|T_MINUS																 {}
	|T_MUL 																	 {}
	| T_DIV  																 {}
	| T_LT 																	 {}
	| T_LTE 																 {}
	| T_GT 																	 {} 
	| T_GTE 																 {}
	| T_EQUAL 																 {}
	| T_NOTEQUAL 															 {}
	| T_AND 																 {}
	| T_OR 																	 {}
;

UnOp:
	T_MINUS 																 {}
	| T_NOT 																 {}
;

%%


void yyerror(const char *s) { printf("ERROR: %s", s); }


main(int argc, char *argv[]){
	yyin = fopen(argv[1], "r");
	yyout = fopen(argv[2], "w");
    do {
		yyparse();
	} while (!feof(yyin));
	fclose(yyin);
	fclose(yyout);
	return 0;
}

