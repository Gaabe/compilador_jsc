#include <iostream>
#include <list>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;

class Node {
	public:
   		const char *value;
   		vector<Node*> childrenList;

   		Node(const char* value);
   		void addChildren(Node *n);
};

void importChildren(Node* mainNode, Node* nodeToImport, const char* name);

void importChildrenThenDeleteNode(Node* mainNode, Node* nodeToImport, const char* name);

void printTree(Node* n);