#include <iostream>
#include <list>
#include <string>
#include <stdio.h>
#include "new_ast.hpp"

using namespace std;

void Node::add(Node n) {
  this->lista->push_back(n);
}

Node::Node(const char* value) {
  this->value = value;
  this->lista = new vector<Node>();
}

void printTree(FILE* yyout, Node n) {
  fprintf(yyout, "[%s", n.value);
  for (int i = 0; i <  int(n.lista->size()); i++) {
    printTree(yyout, n.lista->at(i));
  }
 fprintf(yyout, "]");
}

void importChildren(Node parent, Node child, const char *value){
	for (int i = 0; i <  int(child.lista->size()); i++) {
		if(child.lista->at(i).value == value){
			importChildren(parent, child.lista->at(i), value);
			// for (int j = 0; j <  int(parent.lista->size()); j++){
			// 	if(parent.lista->at(j).value == value){
			// 		parent.lista->erase(parent.lista->begin()+j);
			// 	}
			// }
		}else{
			parent.add(child.lista->at(i));
		}
  	}
}