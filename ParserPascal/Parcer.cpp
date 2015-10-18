#include "Parcer.h"

Parcer::Parcer(string namefile)
{
	vector<string> keyword;
	ifstream file(namefile);                                   
	while (!file.eof()) {                                          
		string helper;                                              
		file >> helper;                                          
		keyword.push_back(helper);                                 
	}
	file.close();

	Tree a;                                        
	a.DoundloadKeywords(keyword);
	m = a.CreateMatrix();
}

bool Parcer::isKeyword(string imput)
{
	int state = 0;
	int index = 0;
	do{
		int column_index = int(imput[index]) - int('A');

		if (isIndexValid(column_index)){
			if (imput[index] == '\0')
				state = m[state][m[0].size() - 1];
			else
				state = 0;
		}
		else
			state = m[state][column_index];
		index++;
	} while (state > 0);
	return state == -1;
}

bool Parcer::isIndexValid(int index)
{
	return ((index < 0) || (index > alphabet));
}
