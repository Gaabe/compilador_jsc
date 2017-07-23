#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>

using namespace std;

class exp_node {
public:
    float num;
    
    // print function for pretty printing an expression
    virtual void print() = 0;
    
    // evaluation function for a leaf, replaced for interior nodes
};

class operator_node : public exp_node {
public:
    exp_node *left;
    exp_node *right;
    
    // the constructor for node links the node to its children,
    // and stores the character representation of the operator.
    operator_node(exp_node *L, exp_node *R);
};

class number_node : public exp_node {
    
public:
    number_node(float value);
    void print();
};

class unary_minus_node : public exp_node {
protected:
    exp_node *exp;
public:
    unary_minus_node(exp_node *exp);
    void print();
};

class exp_paren_node : public exp_node {
protected:
    exp_node *expr;
public:
    exp_paren_node(exp_node *exp);
    void print();
};


class unary_neg_node : public exp_node {
protected:
    exp_node *exp;
public:
    unary_neg_node(exp_node *exp);
    void print();
};

class id_node : public exp_node {
protected:
    string id;
    
public:
    id_node(string value);
    void print();
};


// plus_node inherits the characteristics of node and adds its own evaluate function
class plus_node : public operator_node {
public:
    
    // plus_node's constructor just uses node's constructor
    plus_node(exp_node *L, exp_node *R);
    void print();
};


// minus_node inherits the characteristics of node and adds its own evaluate function
class minus_node : public operator_node {
public:
    
    minus_node(exp_node *L, exp_node *R);
    void print();
};


// times_node inherits the characteristics of node and adds its own evaluate function
class times_node : public operator_node {
public:
    
    times_node(exp_node *L, exp_node *R);
    void print();
};


// divide_node inherits the characteristics of node and adds its own evaluate function
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

class maiori_node : public operator_node {
public:
    
    maiori_node(exp_node *L, exp_node *R);
    void print();
};

class arglist_node : public exp_node {
protected:
    list<exp_node *> *args;
public:
    arglist_node(list<exp_node *> *args);
    void print();
    
};

class funccall_node : public exp_node {
protected:
    string id;
    arglist_node  *arg;
public:
    funccall_node(string name, arglist_node *argumentos);
    void print();
};


class maior_node : public operator_node {
public:
    
    maior_node(exp_node *L, exp_node *R);
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

class statement {
public:
    virtual void print() {}
};

class assignment_stmt : public statement {
protected:
    string id;
    exp_node *exp;
public:
    assignment_stmt(string id,exp_node *expression);
    void print();
};

class funccall_stmt : public statement {
protected:
    exp_node *exp;
public:
    funccall_stmt(exp_node *expression);
    void print();
};

class block {
protected:
    list<decvarst_node *> *decvars;
    list<statement *> *stms;
    
public:
    block(list<decvarst_node *> *decvars, list<statement *> *stms);
    void print();
    
};

class paramlist {
protected:
    list<id_node *> *ids;
    
public:
    paramlist(list<id_node *> *ids);
    void print();
    
};

class decfunc : public decvarst {
protected:
    string id;
    paramlist *params;
    block *funcBlock;
    
public:
    decfunc(string id,paramlist *params,block *funcBlock);
    void print();
    
};


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

class func_call_stmt : public statement {
protected:
    string id;
    exp_node *exp;
public:
    func_call_stmt(string id,exp_node *expression);
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

class return_stmt : public statement {
protected:
    exp_node *exp;
public:
    return_stmt(exp_node *expression);
    void print();
};


class print_stmt: public decvarst {
protected:
    string id;
public:
    print_stmt(string id);
};




class pgm {
protected:
    list<decvarst *> *stmts;
public:
    pgm(list<decvarst *> *stmtlist);
    void print();
    
};


// the object at the base of our tree
extern map<string, float> idTable;
extern pgm *root;
