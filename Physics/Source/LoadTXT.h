#ifndef LOAD_TEXT_H_
#define LOAD_TEXT_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "LevelCreate.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::ifstream;

class LoadTXT
{
public:
	LoadTXT();
	~LoadTXT();
public:
	std::vector<LevelCreate> LoadTextFile();
};

#endif // !LOAD_TEXT_H_
