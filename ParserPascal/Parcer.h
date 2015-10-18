#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Tree.h"
using namespace std;


class Parcer
{
	matrix m;
	const int alphabet = 26;
	bool isIndexValid(int index);
public:
	Parcer(string namefile); 
	bool isKeyword(string imput);
};

