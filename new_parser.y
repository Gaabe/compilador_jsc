%{
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "new_ast.hpp"
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" FILE *yyout;

using namespace std;

void yyerror(const char *s);

Node root = Node("program");


%}

%union {
	Node *node;
	char *ival;
  	char *sval;	
}


%token T_OPENPAR
%token T_CLOSEPAR
%token T_OPENBRAC
%token T_CLOSEBRAC
%token T_OPENCURL
%token T_CLOSECURL
%token T_COMMA
%token T_SEMICOL
%token T_ASSIGN
%token T_PLUS
%token T_MINUS
%token T_MUL
%token T_DIV
%token T_LT
%token T_GT
%token T_LTE
%token T_GTE
%token T_EQUAL
%token T_NOTEQUAL
%token T_AND
%token T_OR
%token T_NOT

%token T_LET
%token T_DEF
%token T_IF
%token T_ELSE
%token T_WHILE
%token T_BREAK
%token T_RETURN
%token T_CONTINUE        

%token <sval> T_ID
%token <ival> T_NUMBER

%token T_ERROR



%type <node> Program
%type <node> DecVar
%type <node> AssignExprOrNothing
%type <node>DecFunc
%type <node> ParamListOrNothing
%type <node> ParamList
%type <node> NCommaIdOrNothing
%type <node> Block
%type <node> NDecVarOrNothing
%type <node> NStmtOrNothing
%type <node> Stmt
%type <node> ElseBlockOrNothing
%type <node> ExprOrNothing
%type <node> Assign
%type <node> FuncCall
%type <node> ArgListOrNothing
%type <node> ArgList
%type <node> NCommaExprOrNothing
%type <node> Expr
%type <node> BinOp
%type <node> UnOp




%left T_LT T_GT T_LTE T_GTE T_EQUAL T_NOTEQUAL T_AND T_OR
%left  T_PLUS T_MINUS
%left  T_MUL T_DIV
%left  T_NOT

%start Program


%%

Program:
	Program DecVar	{root.addChildren($2);}
	| Program DecFunc	{root.addChildren($2);}
	| %empty	{}
;

DecVar:
	T_LET T_ID AssignExprOrNothing T_SEMICOL	{Node n = Node("decvar");
												 Node n2 = Node($2);
											 	 Node *ptr = &n2;
											 	 n.addChildren(ptr);
												 if($3->value != "empty"){
												 	n.addChildren($3);
												 }
												 Node *ptr2 = &n;
												 $$ = ptr2;}
;

AssignExprOrNothing:
	T_ASSIGN Expr    {$$=$2;}
	| %empty	{Node n = Node("empty");
				 Node *ptr = &n;
				 $$ = ptr;}
;

DecFunc:
	T_DEF T_ID T_OPENPAR ParamListOrNothing T_CLOSEPAR Block    {Node n = Node("decfunc");
																 Node n2 = Node($2);
																 Node* ptr = &n2;
																 n.addChildren(ptr);
																 if($4->value != "empty"){
																 	n.addChildren($4);
																 }
																 n.addChildren($6);
																 Node* ptr2 = &n;
																 $$ = ptr2;}
;

ParamListOrNothing:
	ParamList    {$$ = $1;}
	| %empty	{Node n = Node("empty");
				 Node *ptr = &n;
				 $$ = ptr;}
;




ParamList:
	T_ID NCommaIdOrNothing	{Node n = Node("paramlist");
							 Node n2 = Node($1);
							 Node* ptr = &n2;
							 n.addChildren(ptr);
							 if($2->value != "empty"){
							 	n.importChildren($2, "paramlist");
							 }
							 Node* ptr2 = &n;

							 $$ = ptr2;}
;

NCommaIdOrNothing:
	NCommaIdOrNothing T_COMMA T_ID    {Node n = Node("paramlist");
									   if($1->value != "empty"){
										  n.addChildren($1);
									   }
									   Node* ptr = &n;
									   $$ = ptr;

									   }
	| %empty	{Node n = Node("empty");
				 Node *ptr = &n;
				 $$ = ptr;}
;

Block:
	T_OPENCURL NDecVarOrNothing NStmtOrNothing T_CLOSECURL	{Node n = Node("block");
															 if($2->value != "empty"){
															 	n.importChildren($2, "decvar");
															 }
															 if($3->value != "empty"){
															 	n.importChildren($3, "stmt");
															 }
															 Node* ptr2 = &n;
															 $$ = ptr2;}

NDecVarOrNothing:
	NDecVarOrNothing DecVar   {Node n = Node("decvar");
							   if($1->value != "empty"){
								  n.addChildren($1);
							   }
							   Node* ptr = &n;
							   $$ = ptr;
							   }
	| %empty	{Node n = Node("empty");
				 Node *ptr = &n;
				 $$ = ptr;}
;

NStmtOrNothing:
	NStmtOrNothing Stmt	  {Node n = Node("stmt");
						   if($1->value != "empty"){
							  n.addChildren($1);
						   }
						   Node* ptr = &n;
						   $$ = ptr;
						   }
	| %empty	{Node n = Node("empty");
				 Node *ptr = &n;
				 $$ = ptr;}
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
	| %empty	{}
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
	| T_MINUS																 {}
	| T_MUL 																 {}
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
	//root = Node("program");
	Node *ptr = &root;
	printTree(ptr);
    do {
		yyparse();
	} while (!feof(yyin));
	fclose(yyin);
	fclose(yyout);
	return 0;
}

