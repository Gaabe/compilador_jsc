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

void printTree(Node n) {
  printf("[%s", n.value);
  for (int i = 0; i <  int(n.lista->size()); i++) {
    printTree(n.lista->at(i));
  }
  printf("]");
}