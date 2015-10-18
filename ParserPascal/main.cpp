#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

#include "Tree.h"
#include "Parcer.h"
using namespace std;

int main() {
	string stroka;
	cin >> stroka;
	Parcer p("Syntax.txt");
	cout<<p.isKeyword(stroka);

	system("pause");                             
	return 0;                                   
}