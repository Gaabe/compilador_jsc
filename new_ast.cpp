#include <iostream>
#include <vector>
#include <string>
#include "new_ast.hpp"

using namespace std;

void Node::addChildren(Node n){
	this->childrenList.push_back(n);
}

void printTree(Node n){
	printf("[%s", n.value.c_str());
	for(int i = 0; i<n.childrenList.size(); i++){
		printTree(n.childrenList.at(i));
	}
	printf("]");
}

// int main(){
// 	Node n1;
// 	Node n2;
// 	Node n3;

// 	n1.value = "1";
// 	n2.value = "2";
// 	n3.value = "3";
// 	n1.addChildren(n2);
// 	n1.addChildren(n3);
// 	printTree(n1);
// 	return 0;
// }