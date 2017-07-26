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
%token <sval> T_NUMBER

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
	Program DecVar	{//Proot->addChildren($2);
					addChild(Proot, $2);}
	| Program DecFunc	{//Proot->addChildren($2);
						addChild(Proot, $2);}
	| %empty	{}
;

DecVar:
	T_LET T_ID AssignExprOrNothing T_SEMICOL	{Node n = Node("decvar");
												Node* ptr = &n;
												Node n2 = Node("teste");
												Node* ptr2 = &n2;
												addChild(ptr, ptr2);
												$$=ptr;}
;

AssignExprOrNothing:
	T_ASSIGN Expr    {}
	| %empty	{}
;

DecFunc:
	T_DEF T_ID T_OPENPAR ParamListOrNothing T_CLOSEPAR Block    {Node n = Node("decfunc");
																Node* ptr = &n;	
																Node n2 = Node("teste");
																Node* ptr2 = &n2;
																addChild(ptr, ptr2);
																$$=ptr;}
;

ParamListOrNothing:
	ParamList    {}
	| %empty	{}
;




ParamList:
	T_ID NCommaIdOrNothing	{}
;

NCommaIdOrNothing:
	NCommaIdOrNothing T_COMMA T_ID    {}
	| %empty	{}
;

Block:
	T_OPENCURL NDecVarOrNothing NStmtOrNothing T_CLOSECURL	{}

NDecVarOrNothing:
	NDecVarOrNothing DecVar   {}
	| %empty	{}
;

NStmtOrNothing:
	NStmtOrNothing Stmt	  {}
	| %empty	{}
;


Stmt:
	Assign T_SEMICOL  {}
	|FuncCall T_SEMICOL   {}
	|T_IF T_OPENPAR Expr T_CLOSEPAR Block ElseBlockOrNothing  {}
	|T_WHILE T_OPENPAR Expr T_CLOSEPAR Block  {}
	|T_RETURN ExprOrNothing T_SEMICOL {}
	|T_BREAK T_SEMICOL	  {}
	|T_CONTINUE T_SEMICOL {}
;

ElseBlockOrNothing:
	T_ELSE Block {}
	| %empty	{}
;

ExprOrNothing:
	Expr    {}
	| %empty    {}
;

Assign:
	T_ID T_ASSIGN Expr {}
;

FuncCall:
	T_ID T_OPENPAR ArgListOrNothing T_CLOSEPAR	 {}
;

ArgListOrNothing:
	ArgList 	{}
	| %empty    {}
;

ArgList:
	Expr NCommaExprOrNothing 	 {}
;

NCommaExprOrNothing:
	NCommaExprOrNothing T_COMMA Expr {}
	| %empty    {}
;

Expr:
	Expr BinOp Expr {}
	|UnOp Expr {}
	|T_OPENPAR Expr T_CLOSEPAR {}
	|FuncCall {}
	|T_NUMBER 			{}
	|T_ID 				{}
;

BinOp:
	T_PLUS				{}
	| T_MINUS			{}
	| T_MUL 			{}
	| T_DIV  			{}
	| T_LT 				{}
	| T_LTE 			{}
	| T_GT 				{} 
	| T_GTE 			{}
	| T_EQUAL 			{}
	| T_NOTEQUAL 		{}
	| T_AND 			{}
	| T_OR 				{}
;

UnOp:
	T_MINUS 			{}
	| T_NOT 			{}
;

%%


void yyerror(const char *s) { printf("ERROR: %s", s); }

void printar(Node* n){
	printf("%s\n", n->value);
	printf("%s\n", n->childrenList[0]->value);
	printf("%s\n", n->childrenList[1]->value);
	printf("%d\n", n->childrenList[0]->childrenList.size());
	printf("%s\n", n->childrenList[0]->childrenList[0]->value);
	printf("%s\n", n->childrenList[0]->childrenList[1]->value);
	printf("%s\n", n->childrenList[0]->childrenList[0]->childrenList[0]->value);
	printf("%s\n", n->childrenList[0]->childrenList[0]->childrenList[1]->value);
}

main(int argc, char *argv[]){
	yyin = fopen(argv[1], "r");
	yyout = fopen(argv[2], "w");
	Node root = Node("program");
	Proot = &root;
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
	// Node* ptr2 = &n2;
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

	printf("---------\n");
	// printTree(Proot);
	printar(Proot);
	// printTree(Proot);
	// printf("%s\n", Proot->childrenList[0]->value);
	// printf("%s\n", Proot->childrenList[1]->value);
	// printf("%d\n", Proot->childrenList[0]->childrenList.size());
	// printf("%d\n", Proot->childrenList[1]->childrenList.size());
	// printf("%s\n", Proot->childrenList[0]->childrenList[2]->value);
	// printf("%s\n", Proot->childrenList[1]->childrenList[2]->value);
	// // printf("%s", Proot->value);

	// vector<int> v;
	// v.push_back(1);
	// v.push_back(2);
	// v.push_back(3);

	// printf("%d", v[0]);
	// printf("%d", v[1]);
	// printf("%d", v[2]);



	fclose(yyin);
	fclose(yyout);

	return 0;
}

