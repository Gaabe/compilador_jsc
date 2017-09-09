%{
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h> 
#include "new_ast.hpp"
#include "codegen.cpp"
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" FILE *yyout;

using namespace std;

void yyerror(const char *s);
bool isDeclared(vector<const char*> list, const char* name);
int getArgNumber(const char* name, vector<int> argnumberlist, vector<const char*> list);

Node* root;
bool problema_semantico;
vector<const char*> funcoes_declaradas;
vector<const char*> variaveis_declaradas;
vector<int> numero_argumentos_funcoes_declaradas;

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
%type <node> Term
%type <node> Factor
%type <node> AritExpr
%type <node> HigherExpr
%type <node> EvenHigherExpr
%type <node> ABitHigherExpr


%left T_LT T_GT T_LTE T_GTE T_EQUAL T_NOTEQUAL T_AND T_OR
%left  T_PLUS T_MINUS
%left  T_MUL T_DIV
%left  T_NOT
%left  op1


%%

Program:
  Program DecVar {root->add(*($2));}
  | Program DecFunc {root->add(*($2));}
  | %empty {}
;

DecVar:
  T_LET T_ID AssignExprOrNothing T_SEMICOL {Node *n = new Node("decvar");
										    Node *n_id = new Node($2);
											//if(isDeclared(variaveis_declaradas, $2)){
											// 	problema_semantico = true;
											// }else{
											// 	variaveis_declaradas.push_back($2);
											// }
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
															}else{
																funcoes_declaradas.push_back($2);
															}
															numero_argumentos_funcoes_declaradas.push_back($4->lista->size());
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
  NCommaIdOrNothing T_COMMA T_ID 	{Node *n = new Node("many_id");
  									Node *n2 = new Node($3);
  									if(($1)->value != "empty") {n->add(*($1));}
  									n->add(*n2);
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
												if($3->lista->size() != getArgNumber($1, numero_argumentos_funcoes_declaradas, funcoes_declaradas)){
													problema_semantico = true;
												}
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

AritExpr: AritExpr T_PLUS Term		{Node *n = new Node("+");
							n->add(*($1));
							n->add(*($3));
							$$ = n;}
     | AritExpr T_MINUS Term	{Node *n = new Node("-");
							n->add(*($1));
							n->add(*($3));
							$$ = n;}
     | Term		{$$=$1;}
     ;

Expr: 
     Expr T_OR ABitHigherExpr   {Node *n = new Node("||");
  					n->add(*($1));
  					n->add(*($3));
        $$ = n;}
    | ABitHigherExpr		{$$=$1;}
    ;

ABitHigherExpr:
	ABitHigherExpr T_AND HigherExpr  {Node *n = new Node("&&");
  					n->add(*($1));
  					n->add(*($3));
      $$ = n;}
    | HigherExpr		{$$=$1;}
    ;


HigherExpr:
     HigherExpr T_EQUAL EvenHigherExpr {Node *n = new Node("==");
  					n->add(*($1));
  					n->add(*($3));
      $$ = n;}
    | HigherExpr T_NOTEQUAL EvenHigherExpr   {Node *n = new Node("!=");
  					n->add(*($1));
  					n->add(*($3));
        $$ = n;}
    | EvenHigherExpr		{$$=$1;}
    ;

EvenHigherExpr:
	EvenHigherExpr T_LT AritExpr {Node *n = new Node("<");
  					n->add(*($1));
  					n->add(*($3));
      $$ = n;}
    | EvenHigherExpr T_LTE AritExpr  {Node *n = new Node("<=");
  					n->add(*($1));
  					n->add(*($3));
      $$ = n;}
    | EvenHigherExpr T_GT AritExpr   {Node *n = new Node(">");
  					n->add(*($1));
  					n->add(*($3));
        $$ = n;}
    | EvenHigherExpr T_GTE AritExpr  {Node *n = new Node(">=");
  					n->add(*($1));
  					n->add(*($3));
        $$ = n;}
    | AritExpr		{$$=$1;}
    ;



Term: Term T_MUL Factor	{Node *n = new Node("*");
							n->add(*($1));
							n->add(*($3));
							$$ = n;}
     | Term T_DIV Factor	{Node *n = new Node("/");
							n->add(*($1));
							n->add(*($3));
							$$ = n;}
     | Factor	{$$=$1;}
     ;

Factor: T_OPENPAR Expr T_CLOSEPAR	{$$=$2;}
       | T_MINUS Factor		{Node *n = new Node("-");
							n->add(*($2));
							$$ = n;}
       | T_NOT Factor		{Node *n = new Node("!");
							n->add(*($2));
							$$ = n;}
       | T_NUMBER			{Node *n = new Node($1);
    						$$ = n;}
       | T_ID 				{Node *n = new Node($1);
		  					$$ = n;}
		| FuncCall {$$ = $1;}
       ;

%%

int genCode(char *file) {

    
    struct Node2* root2 = (Node2*) calloc (1, sizeof(struct Node2));
    
    int tree2;
    
    if ((out=fopen(file,"r"))==NULL){
        fprintf(stderr, "Arquivo não pode ser aberto\n");
        exit(1);  
    }
        
    fseek(out, 0, SEEK_END);
    tree2=ftell(out);
    fseek(out, 0, SEEK_SET);
                
    char* tree = (char*)calloc(tree2+1, sizeof(char));
    fread(tree, 1, tree2, out);

    fclose(out);
    
    position = 0;
        
    genStruct (tree, &root2);
    fixTree(&root2);
        
    if ((out=fopen(file,"w"))==NULL){
         fprintf(stderr, "Arquivo não pode ser aberto\n");
         exit(1);  
    }   
    
    genFunc(root2);
    fclose(out);  

    return 0;
}

int main(int argc, char *argv[]) {
	problema_semantico = false;
	funcoes_declaradas.push_back("print");
	numero_argumentos_funcoes_declaradas.push_back(1);
	if(argc > 1)
		yyin = fopen(argv[1], "r");
	if(argc > 2)
		yyout = fopen(argv[2], "w");
	root = new Node("program");
  	do {
   		yyparse();
  	} while (!feof(yyin));

	if (!isDeclared(funcoes_declaradas, "main")){
		problema_semantico = true;
	}

	if(!problema_semantico){
  		printTree(yyout, *root);
	}

	if(argc > 1)
		fclose(yyin);
	if(argc > 2)
		fclose(yyout);
  genCode(argv[2]);

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

int getArgNumber(const char* name, vector<int> argnumberlist, vector<const char*> list){
	for (int i = 0; i <  int(list.size()); i++){
		if(strcmp(list.at(i), name) == 0){
			return argnumberlist.at(i);
		}
	}
}

void yyerror(const char *s) { printf("ERROR: %s\n", s); exit(2); }