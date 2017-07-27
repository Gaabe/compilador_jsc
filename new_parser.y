%{
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h> 
#include "new_ast.hpp"
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" FILE *yyout;

using namespace std;

void yyerror(const char *s);
bool isDeclared(vector<const char*> list, const char* name);

Node* root;
bool problema_semantico;
vector<const char*> funcoes_declaradas;
vector<const char*> variaveis_declaradas;

%}

%union {
	Node *node;
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

%%

Program:
  Program DecVar {root->add(*($2));}
  | Program DecFunc {root->add(*($2));}
  | %empty {}
;

DecVar:
  T_LET T_ID AssignExprOrNothing T_SEMICOL {Node *n = new Node("decvar");
										    Node *n_id = new Node($2);
										    if(isDeclared(variaveis_declaradas, $2)){
												problema_semantico = true;
											}
											variaveis_declaradas.push_back($2);
										    n->add(*n_id);
											if(($3)->value != "empty") {n->add(*($3));}
										    $$ = n;}
  ;

AssignExprOrNothing:
  T_ASSIGN Expr    {$$ = $2;}
  | %empty {Node *n = new Node("empty");
    				$$ = n;}
  ;

DecFunc:
  T_DEF T_ID T_OPENPAR ParamListOrNothing T_CLOSEPAR Block {Node *n = new Node("decfunc");
															Node *n_id = new Node($2);
															if(isDeclared(funcoes_declaradas, $2)){
																problema_semantico = true;
															}
															funcoes_declaradas.push_back($2);
															n->add(*n_id);
															n->add(*($4));
															n->add(*($6));
													    	$$ = n;}
  ;

ParamListOrNothing:
  ParamList {Node *n = new Node("paramlist");
  			importChildren(*n, *($1), "many_id");
  			$$ = n;}
  | %empty	{Node *n = new Node("paramlist");
			$$ = n;}
  ;

ParamList:
  T_ID NCommaIdOrNothing 	{Node *n = new Node("many_id");
  							Node *n2 = new Node($1);
  							n->add(*n2);
  							if(($2)->value != "empty") {n->add(*($2));}
							$$ = n;}
  ;

NCommaIdOrNothing:
  NCommaIdOrNothing T_COMMA T_ID 	{Node *n = new Node($3);
  									if(($1)->value != "empty") {n->add(*($1));}
									$$ = n;}
  | %empty 	{Node *n = new Node("empty");
			$$ = n;}
  ;

Block:
  T_OPENCURL NDecVarOrNothing NStmtOrNothing T_CLOSECURL 	{Node *n = new Node("block");
									  						if(($2)->value != "empty") {importChildren(*n, *($2), "many_decvar");}
									  						if(($3)->value != "empty") {importChildren(*n, *($3), "many_stmt");}
															$$ = n;}
  ;

NDecVarOrNothing:
  NDecVarOrNothing DecVar 	{Node *n = new Node("many_decvar");
  							if(($1)->value != "empty") {n->add(*($1));}
  							n->add(*($2));
					 		$$ = n;}
  | %empty 	{Node *n = new Node("empty");
			$$ = n;}
  ;

NStmtOrNothing:
  NStmtOrNothing Stmt	{Node *n = new Node("many_stmt");
  						if(($1)->value != "empty") {n->add(*($1));}
						n->add(*($2));
				 		$$ = n;}
  | %empty	{Node *n = new Node("empty");
			$$ = n;}
  ;

Stmt:
  Assign T_SEMICOL  {$$ = $1;}
  | FuncCall T_SEMICOL   {$$ = $1;}
	| T_IF T_OPENPAR Expr T_CLOSEPAR Block ElseBlockOrNothing  	{Node *n = new Node("if");
																n->add(*$3);
																n->add(*$5);
																if(($6)->value != "empty") {n->add(*($6));}
															  	$$ = n;}
  | T_WHILE T_OPENPAR Expr T_CLOSEPAR Block  	{Node *n = new Node("while");
												n->add(*$3);
												n->add(*$5);
											  	$$ = n;}
  | T_RETURN ExprOrNothing T_SEMICOL 	{Node *n = new Node("return");
										if(($2)->value != "empty") {n->add(*($2));}
										$$ = n;}
  | T_BREAK T_SEMICOL 	{Node *n = new Node("break");
						$$ = n;}
  | T_CONTINUE T_SEMICOL 	{Node *n = new Node("continue");
							$$ = n;}
  ;

ElseBlockOrNothing:
  T_ELSE Block 	{$$ = $2;}
  | %empty	{Node *n = new Node("empty");
			$$ = n;}
  ;

ExprOrNothing:
  Expr    {$$ = $1;}
  | %empty 	{Node *n = new Node("empty");
			$$ = n;}
  ;

Assign:
  T_ID T_ASSIGN Expr 	{Node *n = new Node("assign");
						Node *n_id = new Node($1);
						n->add(*n_id);
						n->add(*($3));
						$$ = n;}
  ;

FuncCall:
  T_ID T_OPENPAR ArgListOrNothing T_CLOSEPAR	{Node *n = new Node("funccall");
												Node *n_id = new Node($1);
												n->add(*n_id);
												n->add(*($3));
												$$ = n;}
  ;

ArgListOrNothing:
  ArgList 	{Node *n = new Node("arglist");
  			importChildren(*n, *($1), "many_args");
			$$ = n;}
  | %empty 	{Node *n = new Node("arglist");
    		$$ = n;}
  ;

ArgList:
  Expr NCommaExprOrNothing 	{Node *n = new Node("many_args");
  							n->add(*($1));
  							if(($2)->value != "empty") {n->add(*($2));}
							$$ = n;}
  ;

NCommaExprOrNothing:
  NCommaExprOrNothing T_COMMA Expr 	{Node *n = new Node("many_args");
  									if(($1)->value != "empty") {n->add(*($1));}
  									n->add(*($3));
									$$ = n;}
  | %empty 	{Node *n = new Node("empty");
    		$$ = n;}
  ;

Expr:
  Expr BinOp Expr 	{($2)->add(*($1));
					($2)->add(*($3));
					$$ = $2;}
  | UnOp Expr 	{($1)->add(*($2));
				$$ = $1;}
  | T_OPENPAR Expr T_CLOSEPAR {$$ = $2;}
  | FuncCall {$$ = $1;}
  | T_NUMBER 	{Node *n = new Node($1);
    			$$ = n;}
  | T_ID 	{Node *n = new Node($1);
		  	$$ = n;}
  ;

BinOp:
  T_PLUS 	{Node *n = new Node("+");
			$$ = n;}
  | T_MINUS {Node *n = new Node("-");
			$$ = n;}
  | T_MUL 	{Node *n = new Node("*");
			$$ = n;}
  | T_DIV 	{Node *n = new Node("/");
		 	$$ = n;}
  | T_LT 	{Node *n = new Node("<");
			$$ = n;}
  | T_LTE 	{Node *n = new Node("<=");
			$$ = n;}
  | T_GT 	{Node *n = new Node(">");
    		$$ = n;}
  | T_GTE 	{Node *n = new Node(">=");
    		$$ = n;}
  | T_EQUAL {Node *n = new Node("==");
			$$ = n;}
  | T_NOTEQUAL 	{Node *n = new Node("!=");
				$$ = n;}
  | T_AND 	{Node *n = new Node("&&");
			$$ = n;}
  | T_OR 	{Node *n = new Node("||");
    		$$ = n;}
  ;

UnOp:
  T_MINUS 	{Node *n = new Node("-");
			$$ = n;}
  | T_NOT 	{Node *n = new Node("!");
			$$ = n;}
  ;

%%

int main(int argc, char *argv[]) {
	problema_semantico = false;
	funcoes_declaradas.push_back("print");
	if(argc > 1)
		yyin = fopen(argv[1], "r");
	if(argc > 2)
		yyout = fopen(argv[2], "w");
	root = new Node("program");
  	do {
   		yyparse();
  	} while (!feof(yyin));

	if(!problema_semantico){
  		printTree(yyout, *root);
	}


	if(argc > 1)
		fclose(yyin);
	if(argc > 2)
		fclose(yyout);

	// printf("teste\n");
	// Node* n = new Node("block");
	// Node* n2 = new Node("many_decvar");
	// Node* n3 = new Node("many_decvar");
	// Node* n4 = new Node("many_decvar");
	// Node* n5 = new Node("decvar");
	// Node* n6 = new Node("decvar");
	// Node* n7 = new Node("decvar");

	// n->add(*n2);
	// n2->add(*n3);
	// n2->add(*n5);
	// n3->add(*n4);
	// n3->add(*n6);
	// n4->add(*n7);
	// printTree(*n);
	// printf("\n");
	// importChildren(*n, *n2, "many_decvar");
	// printTree(*n);
	// printf("\n");

  return 0;
}
bool isDeclared(vector<const char*> list, const char* name){
	for (int i = 0; i <  int(list.size()); i++){
		if(strcmp(list.at(i), name) == 0){
			return true;
		}
	}
	return false;
}

void yyerror(const char *s) { printf("ERROR: %s\n", s); exit(2); }