#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Tree
{
	struct Node;
	Node* Root;

	void addNode(string word, Node *subTree);
public:
	Tree();
	void addNode(string word);
	void DoundloadKeywords(vector<string> keywords);
};

