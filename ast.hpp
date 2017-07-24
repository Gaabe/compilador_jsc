#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>

using namespace std;

//TODO DUVIDA AQUI
//Abstrações
class exp_node {
public:
    float num;
    virtual void print() = 0;
    
};

class operator_node : public exp_node {
public:
    exp_node *left;
    exp_node *right;

    operator_node(exp_node *L, exp_node *R);
};


//#################################################################################################


//UnOp        = '-'  | '!'
class unary_minus_node : public exp_node {
    //Acho que nao precisa
protected:
    exp_node *exp;
public:
    unary_minus_node(exp_node *exp);
    void print();
};


class unary_neg_node : public exp_node {
    //Acho que nao precisa
protected:
    exp_node *exp;
public:
    unary_neg_node(exp_node *exp);
    void print();
};


//#################################################################################################


//BinOp       = '+' | '-' | '*' | '/' | '<' | '<=' | '>' | '>=' | '==' | '!=' | '&&' | '||'
class plus_node : public operator_node {
public:
    plus_node(exp_node *L, exp_node *R);
    void print();
};


class minus_node : public operator_node {
public:
    minus_node(exp_node *L, exp_node *R);
    void print();
};


class times_node : public operator_node {
public:
    times_node(exp_node *L, exp_node *R);
    void print();
};


class divide_node : public operator_node {
public:
    divide_node(exp_node *L, exp_node *R);
    void print();
};

class menor_node : public operator_node {
public:
    menor_node(exp_node *L, exp_node *R);
    void print();
};

class menori_node : public operator_node {
public:
    menori_node(exp_node *L, exp_node *R);
    void print();
};

class maior_node : public operator_node {
public:
    maior_node(exp_node *L, exp_node *R);
    void print();
};

class maiori_node : public operator_node {
public:
    maiori_node(exp_node *L, exp_node *R);
    void print();
};

class iigual_node : public operator_node {
public:
    iigual_node(exp_node *L, exp_node *R);
    void print();
};

class dif_node : public operator_node {
public:
    dif_node(exp_node *L, exp_node *R);
    void print();
};

class e_node : public operator_node {
public:
    e_node(exp_node *L, exp_node *R);
    void print();
};

class ou_node : public operator_node {
public:
    ou_node(exp_node *L, exp_node *R);
    void print();
};


//#################################################################################################


//Expr        = Expr BinOp Expr | UnOp Expr | '(' Expr ')' | FuncCall | DEC | ID
class exp_paren_node : public exp_node {
protected:
    exp_node *expr;
public:
    exp_paren_node(exp_node *exp);
    void print();
};

class dec_node : public exp_node {
protected:
    float dec;
public:
    dec_node(float value);
    void print();
};

class id_node : public exp_node {
protected:
    string id;
public:
    id_node(string value);
    void print();
};



//#################################################################################################



//ArgList     = Expr (',' Expr)*
class arglist_node : public exp_node {
protected:
    list<exp_node *> *args;
public:
    arglist_node(list<exp_node *> *args);
    void print();
    
};


//#################################################################################################



//FuncCall    = ID '(' ArgList? ')'
class funccall_node : public exp_node {
protected:
    string id;
    arglist_node  *arg;
public:
    funccall_node(string name, arglist_node *argumentos);
    void print();
};


//#################################################################################################



//DecVar      = let ID ('=' Expr)? ';'
class decvarst {
public:
    virtual void print() {}
};

class decvarst_node : public decvarst {
protected:
    string id;
    exp_node *exp;
public:
    decvarst_node(string name, exp_node *expression);
    void print();
};

//#################################################################################################


class statement {
public:
    virtual void print() {}
};



//#################################################################################################


//Block       = '{' DecVar* Stmt* '}'
class block {
protected:
    list<decvarst_node *> *decvars;
    list<statement *> *stms;
    
public:
    block(list<decvarst_node *> *decvars, list<statement *> *stms);
    void print();
    
};

//Stmt        = Assign ';' | FuncCall ';' | if '(' Expr ')' Block (else Block)? | while '(' Expr ')' Block | return Expr? ';' | break ';' | continue ';'


class assignment_stmt : public statement {
protected:
    string id;
    exp_node *exp;
public:
    assignment_stmt(string id,exp_node *expression);
    void print();
};


class func_call_stmt : public statement {
protected:
    string id;
    exp_node *exp;
public:
    func_call_stmt(string id,exp_node *expression);
    void print();
};

//class funccall_stmt : public statement {
//protected:
//    exp_node *exp;
//public:
//    funccall_stmt(exp_node *expression);
//    void print();
//};

class if_else_stmt : public statement {
protected:
    exp_node *exp;
    block *_if;
    block *_else;
public:
    if_else_stmt(exp_node *exp, block *_if, block *_else);
    void print();
};

class while_stmt : public statement {
protected:
    exp_node *exp;
    block *while_block;
public:
    while_stmt(exp_node *exp, block *while_block);
    void print();
};

class return_stmt : public statement {
protected:
    exp_node *exp;
public:
    return_stmt(exp_node *expression);
    void print();
};

class break_stmt : public statement {
protected:
    string id;
    
public:
    break_stmt(string value);
    void print();
};


class continue_stmt : public statement {
protected:
    string id;
    
public:
    continue_stmt(string value);
    void print();
};


//#################################################################################################


//ParamList   = ID (',' ID)*
class paramlist {
protected:
    list<id_node *> *ids;
    
public:
    paramlist(list<id_node *> *ids);
    void print();
    
};


//#################################################################################################


//DecFunc     = def ID '(' ParamList? ')' Block
class decfunc : public decvarst {
protected:
    string id;
    paramlist *params;
    block *funcBlock;
    
public:
    decfunc(string id,paramlist *params,block *funcBlock);
    void print();
    
};


//#################################################################################################


//Program     = (DecVar | DecFunc)*
class pgm {
protected:
    list<decvarst *> *stmts;
public:
    pgm(list<decvarst *> *stmtlist);
    void print();
    
};


class print_stmt: public decvarst {
protected:
    string id;
public:
    print_stmt(string id);
};


extern map<string, float> idTable;
extern pgm *root;

