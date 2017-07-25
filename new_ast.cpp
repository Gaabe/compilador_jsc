#include <iostream>
#include <list>
#include <string>
#include "new_ast.hpp"

using namespace std;

void Node::addChildren(Node *n){
	this->childrenList.push_back(n);
}

void Node::importChildren(Node* n, char* name){
	std::list<Node*>::const_iterator iterator = n->childrenList.begin();
	do{
		if((*iterator)->value != name){
			this->addChildren(*iterator);
		}else{
			iterator = (*iterator)->childrenList.begin();
		}
		++iterator;
	}while((*iterator)->value != name || *iterator != n->childrenList.end() > 0)
}

Node::Node(string value){
	value = value;
}

void printTree(Node* n){
	printf("[%s", n->value);
	std::list<Node*>::const_iterator iterator;
	for (iterator = n->childrenList.begin(); iterator !=  n->childrenList.end(); ++iterator) {
    	printTree((*iterator));
	}	
	printf("]");
}

// int main(){
// 	Node n1 = Node("1");
// 	Node n2 = Node("2");
// 	Node n3 = Node("3");

// 	n1.value = "1";
// 	n2.value = "2";
// 	n3.value = "3";
// 	n1.addChildren(n2);
// 	n1.addChildren(n3);
// 	printTree(n1);
// 	return 0;
// }