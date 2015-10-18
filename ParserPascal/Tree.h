#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

typedef vector<int> row;
typedef vector<row> matrix;

class Tree
{
	struct Node;
	Node* Root;

	void addNode(string word, Node *subTree);
	void ParsingTree(matrix &m, Node* subTree, int NumberOfRow);
	int FindColumn(char symbol, matrix &m);
public:
	Tree();
	void addNode(string word);
	void DoundloadKeywords(vector<string> keywords);
	matrix CreateMatrix();
};

