#include <iostream>
#include <vector>
#include <string>

#ifndef _AST_H
#define _AST_H

using namespace std;

class Node {
  public:
    const char* value;
    vector<Node> *lista;

    Node(const char* value);
    void add(Node n);
};

void printTree(Node n);

#endif