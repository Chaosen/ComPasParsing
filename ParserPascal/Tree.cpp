#include "Tree.h"


struct Tree::Node{
	char ch;
	vector<Node*> next;
	Node() { ch = '\0'; }
	Node(char newCh) { ch = newCh; }
};

Tree::Tree() {
	Root = new Node('\0');
}

void Tree::addNode(string word){
	addNode(word, Root);
}

void Tree::DoundloadKeywords(vector<string> keywords){
	for (auto i : keywords) addNode(i);
}

void Tree::addNode(string word, Node *subTree){
	char first = word[0];
	Node* next = nullptr;
	for (auto i : subTree->next) if (i->ch == first) next = i;
	if (next == nullptr){
		next = new Node(first);
		subTree->next.push_back(next);
	}
	if (first != '\0')
		addNode(word.substr(1), next);
}

matrix Tree::CreateMatrix(){
	matrix m(2);
	for (int i = 65; i <= 90; i++) m[0].push_back(i);
	m[0].push_back(int('\0'));
	m[1] = row(m[0].size(), 0);
	for (int i = 0; i < Root->next.size(); i++){
		ParsingTree(m, Root->next[i], 1);
	}
	m.erase(m.begin(), m.begin() + 1);
	for (int i = 0; i < m.size(); i++)
	{
		for (int j = 0; j < m[i].size(); j++)
		{
			if (m[i][j] != 0 && m[i][j] != -1)
			{
				m[i][j] -= 1;
			}
		}
	}
	return m;
}

void Tree::ParsingTree(matrix &m, Node* subTree, int NumberOfRow) {
	char symbol = subTree->ch;
	int NumberOfColumn = FindColumn(symbol, m);
	if (symbol != '\0')
	{
		if (m[NumberOfRow][NumberOfColumn] == 0)
		{
			m.push_back(row(m[0].size(), 0));
			m[NumberOfRow][NumberOfColumn] = m.size() - 1;
		}

		for (int i = 0; i < subTree->next.size(); i++)
			ParsingTree(m, subTree->next[i], m[NumberOfRow][NumberOfColumn]);
	}
	else
		m[NumberOfRow][NumberOfColumn] = -1;
}

int Tree::FindColumn(char symbol, matrix &m){
	for (int i = 0; i < m[0].size(); i++)
	{
		if (symbol == m[0][i])
			return i;
	}
}