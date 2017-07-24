#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Node {
	public:
   		string value;
   		vector<Node> childrenList;

   		void addChildren(Node n);
};

void printTree(Node n);