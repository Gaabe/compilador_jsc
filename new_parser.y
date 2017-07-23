%{
#include <stdlib.h>
#include <stdio.h>
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
#include "node.h"
NBlock *programBlock; /* the top level root node of our final AST */

extern int yylex();
void yyerror(const char *s) { printf("ERROR: %sn", s); }


%}

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
%token T_COL;
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
%token;
%token T_BREAK;
%token T_CONTINUE;        

%token T_ID;
%token T_NUMBER;

%token T_ERROR;



/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <ident> ident
%type <expr> numeric expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl
%type <token> comparison





%left T_LT T_GT T_LTE T_GTE T_EQUAL T_NOTEQUAL T_AND T_OR
%left  T_PLUS T_MINUS
%left  T_MUL T_DIV
%left  T_NOT

%start program


%%

Program     = (DecVar | DecFunc)*

DecVar      = T_LET T_ID (T_ASSIGN Expr)? T_SEMICOL

DecFunc     = T_DEF T_ID T_OPENPAR ParamList? T_CLOSEPAR Block
ParamList   = T_ID (T_COMMA T_ID)*

Block       = T_OPENCURL DecVar* Stmt* T_CLOSECURL

Stmt        = Assign T_SEMICOL |
              FuncCall T_SEMICOL |
              T_IF T_OPENPAR Expr T_CLOSEPAR Block (T_ELSE Block)? |
              T_WHILE T_OPENPAR Expr T_CLOSEPAR Block |
              T_RETURN Expr? T_SEMICOL |
              T_BREAK T_SEMICOL |
              T_CONTINUE T_SEMICOL

Assign      = T_ID T_ASSIGN Expr
FuncCall    = T_ID T_OPENPAR ArgList? T_CLOSEPAR
ArgList     = Expr (T_COMMA Expr)*

Expr        = Expr BinOp Expr |
              UnOp Expr |
              T_OPENPAR Expr T_CLOSEPAR |
              FuncCall |
              T_NUMBER |
              T_ID

BinOp       = T_PLUS | T_MINUS | T_MUL | T_DIV | T_LT | T_LTE |
              T_GT | T_GTE | T_EQUAL | T_NOTEQUAL | T_AND | T_OR
UnOp        = T_MINUS  | T_NOT

%%
//extern FILE *yyin;
//
//main(){
//    do {
//	yyparse();
//	} while(!feof(yyin));
//}
//
//yyerror(char *s){
//}
