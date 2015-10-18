#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Tree.h"
using namespace std;

int main() {
	vector<string> keyword;
	ifstream file("Syntax.txt");
	while (!file.eof()){
		string helper;
		file >> helper;
		keyword.push_back(helper);
	}

	Tree a; 
	a.DoundloadKeywords(keyword);

	system("pause");
	return 0; 
}