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
%token <string> TIDENTIFIER TINTEGER TDOUBLE
%token <token> TCEQ TCNE T_MENOR T_MENOR_IGUAL T_MAIOR TCGE TEQUAL
%token  TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT
%token <token> T_ADICAO T_SUBTRACAO T_PRODUTO T_DIVISAO



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


%token <token> T_DEF T_IF T_ELSE T_WHILE T_RETURN T_BREAK T_CONTINUE T_INT T_VOID
%token <token> T_PARENTESE_ESQ T_CHAVE_ESQ T_COCHETE_ESQ T_COCHETE_DIR T_CHAVE_DIR T_PARENTESE_DIR T_VIRGULA T_PONTO_VIRGULA T_IGUAL T_ADICAO T_SUBTRACAO T_PRODUTO T_DIVISAO T_MENOR T_MAIOR T_MENOR_IGUAL T_MAIOR_IGUAL T_IGUAL_IGUAL T_DIFERENTE T_E T_OU T_ID T_DEC T_NAO





%left T_MENOR T_MAIOR T_MENOR_IGUAL T_MAIOR_IGUAL T_IGUAL_IGUAL T_DIFERENTE T_E T_OU
%left  T_ADICAO T_SUBTRACAO
%left  T_PRODUTO T_DIVISAO
%left  T_NAO

%start program


%%



program:      DecVar | D;


DecVar:       Type T_ID T_IGUAL Expr T_PONTO_VIRGULA;


DecFunc:      T_DEF Type T_ID T_PARENTESE_ESQ ParamList T_PARENTESE_DIR Block;

ParamList:    Type T_ID T_VIRGULA Type T_ID;


Block:        T_CHAVE_ESQ DecVar Stmt T_CHAVE_DIR;


Stmt:         Assign T_PONTO_VIRGULA |

              FuncCall T_PONTO_VIRGULA |

              T_IF T_PARENTESE_ESQ Expr T_PARENTESE_DIR Block T_ELSE Block |

              T_WHILE T_PARENTESE_ESQ Expr T_PARENTESE_DIR Block |

             T_RETURN Expr T_PONTO_VIRGULA |

             T_BREAK T_PONTO_VIRGULA |

              T_CONTINUE T_PONTO_VIRGULA;


Assign:       T_ID T_IGUAL Expr;

FuncCall:     T_ID T_PARENTESE_ESQ ArgList T_PARENTESE_DIR;

ArgList:      Expr T_VIRGULA Expr;



Expr: 
	//Expr BinOp Expr |
	Expr T_ADICAO Expr |
	Expr T_SUBTRACAO Expr |
	Expr T_PRODUTO Expr |
	Expr T_DIVISAO Expr |
        Expr T_MENOR Expr |
        Expr T_MAIOR Expr |
        Expr T_MENOR_IGUAL Expr |
        Expr T_MAIOR_IGUAL Expr |
        Expr T_IGUAL_IGUAL Expr |
        Expr T_DIFERENTE Expr |
        Expr T_E Expr |
        Expr T_OU Expr |

//	UnOp Expr |
        T_SUBTRACAO Expr |
        T_NAO Expr |
	T_COCHETE_ESQ Expr T_COCHETE_DIR |
	FuncCall |
	T_DEC;	

//BinOp:
//	T_ADICAO |
//	T_SUBTRACAO |
//	T_PRODUTO |
//	T_DIVISAO |
//	T_MENOR |
//	T_MAIOR |
//	T_MENOR_IGUAL |
//	T_MAIOR_IGUAL |
//	T_IGUAL_IGUAL |
//	T_DIFERENTE |
//	T_E |
//	T_OU;


//UnOp: 	T_SUBTRACAO |
//	        T_NAO;

Type:	T_INT |
	T_VOID;


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
