#include <iostream>
#include <list>
#include <string>

using namespace std;

class Node {
	public:
   		char *value;
   		list<Node*> childrenList;

   		Node(string value);
   		void addChildren(Node *n);
   		void importChildren(Node* n, char* name);
};

void printTree(Node* n);