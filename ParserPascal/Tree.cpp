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