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
 							 Node* ptr2 = &n;
							 if($2->value != "empty"){
							 	importChildrenThenDeleteNode(ptr2, $2, "paramlist");
							 }
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
															 Node* ptr2 = &n;
															 if($2->value != "empty"){
															 	importChildrenThenDeleteNode(ptr2, $2, "decvar");
															 }
															 if($3->value != "empty"){
															 	importChildrenThenDeleteNode(ptr2, $3, "stmt");
															 }
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
	Assign T_SEMICOL  {$$=$1;}
	|FuncCall T_SEMICOL   {$$=$1;}
	|T_IF T_OPENPAR Expr T_CLOSEPAR Block ElseBlockOrNothing  {Node n = Node("if");
															   n.addChildren($3);
															   n.addChildren($5);
															   if($6->value != "empty"){
																  n.addChildren($6);
															   }
															   Node* ptr = &n;
															   $$ = ptr;
															   }
	|T_WHILE T_OPENPAR Expr T_CLOSEPAR Block  {Node n = Node("while");
											   n.addChildren($3);
											   n.addChildren($5);
											   Node* ptr = &n;
											   $$ = ptr;
											   }
	|T_RETURN ExprOrNothing T_SEMICOL {Node n = Node("return");
									   if($2->value != "empty"){
										  n.addChildren($2);
									   }
									   Node* ptr = &n;
									   $$ = ptr;
									   }
	|T_BREAK T_SEMICOL	  {Node n = Node("break");
						   Node* ptr = &n;
						   $$ = ptr;
						   }
	|T_CONTINUE T_SEMICOL {Node n = Node("continue");
						   Node* ptr = &n;
						   $$ = ptr;
						   }
;

ElseBlockOrNothing:
	T_ELSE Block {$$=$2;}
	| %empty	{Node n = Node("empty");
				 Node *ptr = &n;
				 $$ = ptr;}
;

ExprOrNothing:
	Expr    {$$=$1;}
	| %empty    {Node n = Node("empty");
				 Node *ptr = &n;
				 $$ = ptr;}
;

Assign:
	T_ID T_ASSIGN Expr {Node n = Node("assign");
						Node n2 = Node($1);
						Node* ptr2 = &n2;
						n.addChildren(ptr2);
						n.addChildren($3);
					    Node* ptr = &n;
					    $$ = ptr;
					    }
;

FuncCall:
	T_ID T_OPENPAR ArgListOrNothing T_CLOSEPAR	 {Node n = Node("funccall");
												  Node n2 = Node($1);
												  Node* ptr2 = &n2;
												  n.addChildren(ptr2);
												  if($3->value != "empty"){
												  	n.addChildren($3);
												  }
												  Node* ptr = &n;
												  $$ = ptr;
												  }
;

ArgListOrNothing:
	ArgList 	{$$=$1;}
	| %empty    {Node n = Node("empty");
				 Node *ptr = &n;
				 $$ = ptr;}
;

ArgList:
	Expr NCommaExprOrNothing 	 {Node n = Node("arglist");
								  n.addChildren($1);
								  if($2->value != "empty"){
								  	n.addChildren($2);
								  }
								  Node* ptr = &n;
								  $$ = ptr;
								  }
;

NCommaExprOrNothing:
	NCommaExprOrNothing T_COMMA Expr 										 {}
	| %empty    {Node n = Node("empty");
				 Node *ptr = &n;
				 $$ = ptr;}
;

Expr:
	Expr BinOp Expr {$2->addChildren($1);
					$2->addChildren($3);
					$$=$2;}
	|UnOp Expr {$1->addChildren($2);
				$$=$1;}
	|T_OPENPAR Expr T_CLOSEPAR {$$=$2;}
	|FuncCall {$$=$1;}
	|T_NUMBER 			{Node n = Node("$1");
						   Node* ptr = &n;
						   $$ = ptr;}
	|T_ID 				{Node n = Node("$1");
						   Node* ptr = &n;
						   $$ = ptr;}
;

BinOp:
	T_PLUS				{Node n = Node("+");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_MINUS			{Node n = Node("-");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_MUL 			{Node n = Node("*");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_DIV  			{Node n = Node("/");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_LT 				{Node n = Node("<");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_LTE 			{Node n = Node("<=");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_GT 				{Node n = Node(">");
						   Node* ptr = &n;
						   $$ = ptr;} 
	| T_GTE 			{Node n = Node(">=");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_EQUAL 			{Node n = Node("==");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_NOTEQUAL 		{Node n = Node("!=");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_AND 			{Node n = Node("&");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_OR 				{Node n = Node("|");
						   Node* ptr = &n;
						   $$ = ptr;}
;

UnOp:
	T_MINUS 			{Node n = Node("-");
						   Node* ptr = &n;
						   $$ = ptr;}
	| T_NOT 			{Node n = Node("!");
						   Node* ptr = &n;
						   $$ = ptr;
						   }
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

