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

Node* Proot;


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
	Program DecVar	{Proot->addChildren($2);}
	| Program DecFunc	{Proot->addChildren($2);}
	| %empty	{}
;

DecVar:
	T_LET T_ID AssignExprOrNothing T_SEMICOL	{Node* n = new Node("decvar");
												 Node* n2 = new Node($2);
											 	 n->addChildren(n2);
												 if($3->value != "empty"){
												 	n->addChildren($3);
												 }
												 $$ = n;
												printf("decvar\n");}
;

AssignExprOrNothing:
	T_ASSIGN Expr    {$$=$2;}
	| %empty	{Node* n = new Node("empty");
				 $$ = n;}
;

DecFunc:
	T_DEF T_ID T_OPENPAR ParamListOrNothing T_CLOSEPAR Block    {Node* n = new Node("decfunc");
																 Node* n2 = new Node($2);
																 n->addChildren(n2);
																 n->addChildren($4);
																 n->addChildren($6);
																 $$ = n;
																 printf("decfunc\n");}
;

ParamListOrNothing:
	ParamList    {$$ = $1;}
	| %empty	{Node* n = new Node("paramlist");
				 $$ = n;}
;




ParamList:
	T_ID NCommaIdOrNothing	{Node* n = new Node("paramlist");
							 Node* n2 = new Node($1);
							 n->addChildren(n2);
							 if($2->value != "empty"){
							 	//importChildrenThenDeletenew Node(n2, $2, "paramlist");
							 	n->addChildren($2);
							 }
							 $$ = n;
							 printf("paramlist\n");}
;

NCommaIdOrNothing:
	NCommaIdOrNothing T_COMMA T_ID    {Node* n = new Node("paramlist");
									   if($1->value != "empty"){
										  n->addChildren($1);
									   }
									   $$ = n;
									   printf("paramlist\n");}
	| %empty	{Node* n = new Node("empty");
				 $$ = n;}
;

Block:
	T_OPENCURL NDecVarOrNothing NStmtOrNothing T_CLOSECURL	{Node* n = new Node("block");
															 if($2->value != "empty"){
															 	//importChildrenThenDeletenew Node(n2, $2, "decvar");
															 	n->addChildren($2);
															 }
															 if($3->value != "empty"){
															 	//importChildrenThenDeletenew Node(n2, $3, "stmt");
															 	n->addChildren($3);
															 }
															 $$ = n;
															printf("decvar\n");}

NDecVarOrNothing:
	NDecVarOrNothing DecVar   {Node* n = new Node("decvar");
							   if($1->value != "empty"){
								  n->addChildren($1);
							   }
							   $$ = n;
							   printf("decvar\n");}
	| %empty	{Node* n = new Node("empty");
				 $$ = n;}
;

NStmtOrNothing:
	NStmtOrNothing Stmt	  {Node* n = new Node($2->value);
						   if($1->value != "empty"){
							  n->addChildren($1);
						   }
						   $$ = n;
						   printf("%s\n", $2->value);}
	| %empty	{Node* n = new Node("empty");
				 $$ = n;}
;


Stmt:
	Assign T_SEMICOL  {$$=$1;}
	|FuncCall T_SEMICOL   {$$=$1;}
	|T_IF T_OPENPAR Expr T_CLOSEPAR Block ElseBlockOrNothing  {Node* n = new Node("if");
															   n->addChildren($3);
															   n->addChildren($5);
															   if($6->value != "empty"){
																  n->addChildren($6);
															   }
															   $$ = n;
															   printf("if\n");}
	|T_WHILE T_OPENPAR Expr T_CLOSEPAR Block  {Node* n = new Node("while");
											   n->addChildren($3);
											   n->addChildren($5);
											   $$ = n;
											   printf("while\n");}
	|T_RETURN ExprOrNothing T_SEMICOL {Node* n = new Node("return");
									   if($2->value != "empty"){
										  n->addChildren($2);
									   }
									   printf("return\n");
									   $$ = n;
									   }
	|T_BREAK T_SEMICOL	  {Node* n = new Node("break");
						   $$ = n;
						   printf("break\n");}
	|T_CONTINUE T_SEMICOL {Node* n = new Node("continue");
						   $$ = n;
						   printf("continue\n");}
;

ElseBlockOrNothing:
	T_ELSE Block {$$=$2;}
	| %empty	{Node* n = new Node("empty");
				 $$ = n;}
;

ExprOrNothing:
	Expr    {$$=$1;}
	| %empty    {Node* n = new Node("empty");
				 $$ = n;}
;

Assign:
	T_ID T_ASSIGN Expr {Node* n = new Node("assign");
						Node* n2 = new Node($1);
						n->addChildren(n2);
						n->addChildren($3);
					    $$ = n;
					    printf("assign\n");}
;

FuncCall:
	T_ID T_OPENPAR ArgListOrNothing T_CLOSEPAR	 {Node* n = new Node("funccall");
												  Node* n2 = new Node($1);
												  n->addChildren(n2);
												  if($3->value != "empty"){
												  	n->addChildren($3);
												  }
												  $$ = n;
												 printf("funccall\n");}
;

ArgListOrNothing:
	ArgList 	{$$=$1;}
	| %empty    {Node* n = new Node("empty");
				 $$ = n;}
;

ArgList:
	Expr NCommaExprOrNothing 	 {Node* n = new Node("arglist");
								  n->addChildren($1);
								  if($2->value != "empty"){
								  	n->addChildren($2);
								  }
								  $$ = n;
								  printf("arglist\n");}
;

NCommaExprOrNothing:
	NCommaExprOrNothing T_COMMA Expr {Node* n = new Node("ncommaexprornothing");
									   if($3->value != "empty"){
										 n->addChildren($3);
									   }
									   $$ = n;
									   printf("%s\n", $3->value);}
	| %empty    {Node* n = new Node("empty");
				 $$ = n;}
;

Expr:
	Expr BinOp Expr {$2->addChildren($1);
					$2->addChildren($3);
					$$=$2;}
	|UnOp Expr {$1->addChildren($2);
				$$=$1;}
	|T_OPENPAR Expr T_CLOSEPAR {$$=$2;}
	|FuncCall {$$=$1;}
	|T_NUMBER 			{Node* n = new Node($1);
						   $$ = n;
						   printf("%s\n", $1);}
	|T_ID 				{Node* n = new Node($1);
						   $$ = n;
						   printf("%s\n", $1);}
;

BinOp:
	T_PLUS				{Node* n = new Node("+");
						   $$ = n;
							printf("+\n");}
	| T_MINUS			{Node* n = new Node("-");
						   $$ = n;
						   printf("-\n");}
	| T_MUL 			{Node* n = new Node("*");
						   $$ = n;
						   printf("*\n");}
	| T_DIV  			{Node* n = new Node("/");
						   $$ = n;
						   printf("/\n");}
	| T_LT 				{Node* n = new Node("<");
						   $$ = n;
						   printf("<\n");}
	| T_LTE 			{Node* n = new Node("<=");
						   $$ = n;
						   printf("<=\n");}
	| T_GT 				{Node* n = new Node(">");
						   $$ = n;
						   printf(">\n");} 
	| T_GTE 			{Node* n = new Node(">=");
						   $$ = n;
						   printf(">=\n");}
	| T_EQUAL 			{Node* n = new Node("==");
						   $$ = n;
						   printf("==\n");}
	| T_NOTEQUAL 		{Node* n = new Node("!=");
						   $$ = n;
						   printf("!=\n");}
	| T_AND 			{Node* n = new Node("&");
						   $$ = n;
						   printf("&\n");}
	| T_OR 				{Node* n = new Node("|");
						   
						   $$ = n;
						   printf("|\n");}
;

UnOp:
	T_MINUS 			{Node* n = new Node("-");
						   $$ = n;
						   printf("-\n");}
	| T_NOT 			{Node* n = new Node("!");
						   $$ = n;
						   printf("!\n");}
;

%%


void yyerror(const char *s) { printf("ERROR: %s", s); }


main(int argc, char *argv[]){
	yyin = fopen(argv[1], "r");
	yyout = fopen(argv[2], "w");
	Node* root = new Node("program");
	printf("---------\n");
    do {
		yyparse();
	} while (!feof(yyin));

	// Node n1 = Node("paramlist");
	// Node n2 = Node("id");
	// Node n3 = Node("ncomma");
	// Node n4 = Node("ncomma");
	// Node n5 = Node("id");
	// Node n6 = Node("ncomma");
	// Node n7 = Node("id");

	// Node* ptr1 = &n1;
	// 
	// Node* ptr3 = &n3;
	// Node* ptr4 = &n4;
	// Node* ptr5 = &n5;
	// Node* ptr6 = &n6;
	// Node* ptr7 = &n7;
	// n1.addChildren(ptr2);
	// n1.addChildren(ptr3);
	// n3.addChildren(ptr4);
	// n3.addChildren(ptr5);
	// n4.addChildren(ptr6);
	// n4.addChildren(ptr7);
	// printTree(ptr1);

	printf("---------\n");
	printTree(root);
	// printf("%s", Proot->value);



	fclose(yyin);
	fclose(yyout);

	return 0;
}

