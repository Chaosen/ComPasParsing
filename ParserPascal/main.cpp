#include <iostream>
#include <fstream>
#include <iomanip>
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

	ofstream file1("Matrix.txt");
	matrix m = a.CreateMatrix();
	for (int i = 0; i < m.size(); i++){
		for (int j = 0; j < m[i].size(); j++){
			cout << setw(2) << m[i][j] << ' ';
			file1 << m[i][j] << '\t';
		}
		cout << endl;
		file1 << endl;
	}
	system("pause");
	return 0; 
}