#include <iostream>
#include <list>
#include <string>
#include "new_ast.hpp"
#include <stdio.h>
#include <vector>

using namespace std;

void Node::addChildren(Node *n){
	this->childrenList.push_back(n);
}

void importChildren(Node* mainNode, Node* nodeToImport, const char* name){
	for (int i = 0; i < nodeToImport->childrenList.size(); i++){
		if(nodeToImport->childrenList[i]->value != name){
			mainNode->addChildren(nodeToImport->childrenList[i]);
		}else{
			importChildren(mainNode, nodeToImport->childrenList[i], name);
		}
	}
}

void importChildrenThenDeleteNode(Node* mainNode, Node* nodeToImport, const char* name){
	importChildren(mainNode, nodeToImport, name);
	for( std::vector<Node*>::iterator iter = (mainNode->childrenList).begin(); iter != (mainNode->childrenList).end(); ++iter )
	{
	    if( *iter == nodeToImport )
	    {
	        (mainNode->childrenList).erase( iter );
	        break;
	    }
	}
}

Node::Node(const char* value){
	this->value = value;
}

void printTree(Node* n){
	printf("[%s", n->value);
	for (int i = 0; i < n->childrenList.size(); i++) {
    	printTree(n->childrenList[i]);
	}	
	printf("]");
}


// Main para testes na AST

// int main(){
// 	Node n1 = Node("paramlist");
// 	Node n2 = Node("id");
// 	Node n3 = Node("ncomma");
// 	Node n4 = Node("ncomma");
// 	Node n5 = Node("id");
// 	Node n6 = Node("ncomma");
// 	Node n7 = Node("id");

// 	Node* ptr1 = &n1;
// 	Node* ptr2 = &n2;
// 	Node* ptr3 = &n3;
// 	Node* ptr4 = &n4;
// 	Node* ptr5 = &n5;
// 	Node* ptr6 = &n6;
// 	Node* ptr7 = &n7;
// 	n1.addChildren(ptr2);
// 	n1.addChildren(ptr3);
// 	n3.addChildren(ptr4);
// 	n3.addChildren(ptr5);
// 	n4.addChildren(ptr6);
// 	n4.addChildren(ptr7);
// 	printTree(ptr1);
// 	printf("\n");
// 	importChildrenThenDeleteNode(ptr1, ptr3, "ncomma");
// 	printTree(ptr1);
// 	return 0;
// }