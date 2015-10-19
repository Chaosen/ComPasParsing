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
	setlocale(LC_ALL, "Russian");
	Parcer p("Syntax.txt");
	
	bool exit = 0;
	while (exit)
	{
		cout << "Введите ключевое слово" << endl;
		cin >> stroka;
		if (p.isKeyword(stroka))
			cout << "Является ключевым словом" << endl;
		else
			cout << "Не является ключевым словом" << endl;
		cout << "Чтобы проверить следующее слово нажмите - 1 , Выйти - 0" << endl;
		bool ok = 1;
		cin >> ok;
		if (!ok)
			exit = 0;
	}
	
	system("pause");                             
	return 0;                                   
}