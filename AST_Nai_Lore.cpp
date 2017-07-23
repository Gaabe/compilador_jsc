#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "AST_Nai_Lore.hpp"

using namespace std;

// the constructor for node links the node to its children,
// and stores the character representation of the operator.
operator_node::operator_node(exp_node *L, exp_node *R) {
    left    = L;
    right   = R;
}

exp_paren_node::exp_paren_node(exp_node *exp) {
    expr = exp;
}

void exp_paren_node:: print() {
    expr->print();
}




number_node::number_node(float value) {
    num = value;
}

void number_node:: print() {
    cout << "[" << num << "]";
}



id_node::id_node(string value) : id(value) {}

void id_node:: print() {
    cout <<  "[" << id << "]";
}



// plus_node inherits the characteristics of node and adds its own evaluate function
// plus_node's constructor just uses node's constructor
plus_node::plus_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void plus_node:: print() {
    cout << "[+ ";
    left->print();
    right->print();
    cout << "]";
    
}




// minus_node inherits the characteristics of node and adds its own evaluate function
minus_node::minus_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void minus_node:: print() {
    cout << "[- ";
    left->print();
    right->print();
    cout << "]";
    
}




// times_node inherits the characteristics of node and adds its own evaluate function
times_node::times_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void times_node:: print() {
    cout << "[* ";
    left->print();
    right->print();
    cout << "]";
}




// divide_node inherits the characteristics of node and adds its own evaluate function

divide_node::divide_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void divide_node:: print() {
    cout << "[/ ";
    left->print();
    right->print();
    cout << "]";
}




menor_node::menor_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void menor_node:: print() {
    cout << "[< ";
    left->print();
    right->print();
    cout << "]";
}


menori_node::menori_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void menori_node:: print() {
    cout << "[<= ";
    left->print();
    right->print();
    cout << "]";
}



maior_node::maior_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void maior_node:: print() {
    cout << "[> ";
    left->print();
    right->print();
    cout << "]";
}


maiori_node::maiori_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void maiori_node:: print() {
    cout << "[>= ";
    left->print();
    right->print();
    cout << "]";
}



iigual_node::iigual_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void iigual_node:: print() {
    cout << "[== ";
    left->print();
    right->print();
    cout << "]";
}




dif_node::dif_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void dif_node:: print() {
    cout << "[!= ";
    left->print();
    right->print();
    cout << "]";
}


e_node::e_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void e_node:: print() {
    cout << "[&& ";
    left->print();
    right->print();
    cout << "]";
}



ou_node::ou_node(exp_node *L, exp_node *R) : operator_node(L,R) {
}

void ou_node:: print() {
    cout << "[|| ";
    left->print();
    right->print();
    cout << "]";
}




// unary_minus_node inherits the characteristics of node and adds its own evaluate function
unary_minus_node::unary_minus_node(exp_node *L) : exp(L) {}

void unary_minus_node:: print() {
    cout << "-";
    exp->print();
}





unary_neg_node::unary_neg_node(exp_node *L) : exp(L) {}

void unary_neg_node:: print() {
    cout << "!";
    exp->print();
}


decvarst_node::decvarst_node(string name, exp_node *expression)
: id(name), exp(expression) {}

void decvarst_node::print() {
    cout << "[decvar[" << id <<"]" ;
    if(exp != NULL){
    exp->print();
    }
    cout  <<"]";
}




arglist_node::arglist_node(list<exp_node *> *argumentos){
    args = argumentos;
}

void arglist_node::print() {
    list<exp_node *>::iterator argsIter;

    cout <<  "[arglist";
    
    for (argsIter = args->begin(); argsIter != args->end();
         argsIter++) {
        (*argsIter)->print();
    }
    
    
    cout <<"]\n";
}


block::block(list<decvarst_node *> *declarations, list<statement *> *statments)
{
    decvars =declarations;
    stms = statments;
}

void block::print() {
    list<decvarst_node *>::iterator decVarIter;
    cout << "[block" ;
    if(decvars!=NULL){
        for (decVarIter = decvars->begin(); decVarIter != decvars->end();
            decVarIter++) {
            (*decVarIter)->print();
        }
    }
    list<statement *>::iterator statmantsIter;
    
    if(stms!=NULL){
        for (statmantsIter = stms->begin(); statmantsIter != stms->end  ();
         statmantsIter++) {
            (*statmantsIter)->print();
        }
    }
    cout<<"]";
    
}

paramlist::paramlist(list<id_node *> *nodes_ids)
{
    ids =nodes_ids;
}

void paramlist::print() {
    list<id_node *>::iterator idsIter;
    cout << "[paramlist" ;
    if(ids!=NULL){
        for (idsIter = ids->begin(); idsIter != ids->end();
            idsIter++) {
            (*idsIter)->print();
        }
    }
    
    cout<<"]";
    
}




decfunc::decfunc(string identifier,paramlist *listParams,block *functionBlock)
{
    id=identifier;
    params= listParams;
    funcBlock=functionBlock;
}

void decfunc::print() {
    list<id_node *>::iterator idsIter;
    cout << "[decfunc[" << id << "]" ;
    params->print();
    funcBlock->print();
    cout<<"]";
    
}





func_call_stmt::func_call_stmt(string identificador, exp_node *expression)
:  exp(expression) {
    id=identificador;
}




funccall_node::funccall_node(string name, arglist_node *argumento)
: id(name), arg(argumento) {}

void funccall_node::print() {
    list<arglist_node *>::iterator argsIter;

    cout << "[funccall[" << id <<"]" ;
    arg->print();
    

    cout  <<"]";
}


if_else_stmt::if_else_stmt(exp_node *expression, block *if_block, block *else_block) {
    exp = expression;
    _if = if_block;
    _else = else_block;
}

void if_else_stmt::print() {
    cout << "[if";
    exp->print();

    
    _if->print();
    _else->print();
}

while_stmt::while_stmt(exp_node *expression, block *whileBLK) {
    exp = expression;
    while_block = whileBLK;
}

void while_stmt::print() {
    cout << "[while";
    exp->print();
    
    
    while_block->print();
}


void func_call_stmt::print() {
    cout << "[funccall[" << id <<"]" ;
    exp->print();
    cout  <<"]";
}




funccall_stmt::funccall_stmt(exp_node *expression)
{
    exp = expression;
}

void funccall_stmt::print() {
    exp->print();
}

break_stmt::break_stmt(string value) : id(value) {}

void break_stmt:: print() {
    cout <<  "[" << id << "]";
}

continue_stmt::continue_stmt(string value) : id(value) {}

void continue_stmt:: print() {
    cout <<  "[" << id << "]";
}


return_stmt::return_stmt(exp_node *expression)
: exp(expression) {}

void return_stmt::print() {
    cout << "[return " ;
    exp->print();
    cout  <<"]";
}

assignment_stmt::assignment_stmt(string name, exp_node *expression)
: id(name), exp(expression) {}

void assignment_stmt::print() {
    cout << "[assign[" << id <<"]" ;
    exp->print();
    cout  <<"]";
}



print_stmt::print_stmt (string name) : id(name) {}





pgm::pgm(list<decvarst *> *stmtlist) {
    stmts = stmtlist;
}



void pgm::print() {
    list<decvarst *>::iterator stmtIter;

    cout <<"[program";
    if(stmts!=NULL){
        //Imprime todos os statments
        for (stmtIter = stmts->begin(); stmtIter != stmts->end();
             stmtIter++) {
            (*stmtIter)->print();
        }
    }
    

    
    cout <<"]\n";
    
}





map<string, float> idTable;


void test1(){
    exp_node *op1 = new id_node("a");
    exp_node *op2 = new number_node(2);
    list<exp_node *> *args = new list<exp_node *>;
    args->push_back(op1);
    args->push_back(op2);
    
    
    arglist_node *arglist = new arglist_node(args);
    
    funccall_node *func = new funccall_node("add", arglist);
    
    
    list<decvarst_node *> *decvars = new list<decvarst_node *>;
    exp_node *op3 = new plus_node(new number_node(3),new number_node(5));
    exp_node *op4 = new id_node("b");
    
    
    decvarst_node *var1 = new decvarst_node("a",op3);
    decvarst_node *var2 = new decvarst_node("b",NULL);
    
    decvars->push_back(var1);
    decvars->push_back(var2);
    
    list<statement *> *stmts = new list<statement *>;
    block *block1 = new block(decvars,stmts);
    list<id_node *> *idList = new list<id_node *>;
    
    
    paramlist *paramlist1 = new paramlist(idList);
    decfunc *funcao = new decfunc("add",paramlist1,block1);
    list<decvarst *> *decfuncs = new list<decvarst *>;
    decfuncs->push_back(funcao);
    //funcao->print();
    pgm *program = new pgm(decfuncs);
    program->print();

}

void test2(){
    exp_node *op1 = new id_node("a");
    exp_node *op2 = new number_node(2);
    list<exp_node *> *args = new list<exp_node *>;
    args->push_back(op1);
    args->push_back(op2);
    
    
    
    
    
    list<decvarst_node *> *decvars = new list<decvarst_node *>;
    
    list<statement *> *stmts = new list<statement *>;
    stmts->push_back(new return_stmt(new plus_node(new id_node("x"), new id_node("y"))));
    block *block1 = new block(decvars,stmts);
    list<id_node *> *idList = new list<id_node *>;
    idList->push_back(new id_node("x"));
    idList->push_back(new id_node("x"));

    arglist_node *arglist = new arglist_node(args);

    
    
    paramlist *paramlist1 = new paramlist(idList);
    paramlist *paramlist2 = new paramlist(NULL);

    list<decvarst_node *> *decvars2 = new list<decvarst_node *>;
    decvars2->push_back(new decvarst_node("a",NULL));
    list<statement *> *stmts2 = new list<statement *>;
    stmts2->push_back(new assignment_stmt("a", new number_node(3)));
    
    exp_node *op3 = new id_node("a");
    exp_node *op4 = new number_node(2);
    list<exp_node *> *args2 = new list<exp_node *>;
    args2->push_back(op3);
    args2->push_back(op4);
    
    funccall_node *func_node = new funccall_node("add", new arglist_node(args2));
    stmts2->push_back(new return_stmt(func_node));
    block *block2 = new block(decvars2,stmts2);

    decfunc *funcao = new decfunc("add",paramlist1,block1);
    decfunc *funcao2 = new decfunc("main",paramlist2,block2);

    
    
    
    
    
    
    list<decvarst *> *decfuncs = new list<decvarst *>;
    decfuncs->push_back(funcao);
    decfuncs->push_back(funcao2);

    //funcao->print();
    
    
    
    
    
    
    pgm *program = new pgm(decfuncs);
    program->print();
    
}

int main(){
    
    //test1();
    //test2();

    
}
