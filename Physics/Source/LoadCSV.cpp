#include <fstream>
#include <iostream>
#include <string>

#include "LoadCSV.h"

using std::string;
using std::getline;

std::vector<Score> LoadCSV(const char *file_path)
{
	std::vector<Score> output;
	Score *Data;
	std::ifstream ip(file_path);
	if (!ip.good())
	{
		Data = new Score;
		std::cout << "ERROR " << file_path << " Not Found~" << std::endl;	
		Data->rank = "RANK", Data->name = "NAME", Data->score = "SCORE";
		output.push_back(*Data);
		delete &Data;
		Data = new Score;
		Data->rank = "ERROR", Data->name = "ERROR", Data->score = "ERROR";
		output.push_back(*Data);
		delete &Data;
		return output;
	}
	else
		std::cout << file_path << " File Opened~" << std::endl;
	
	while (ip.good())
	{
		Data = new Score;
		getline(ip, Data->rank, ',');
		getline(ip, Data->name, ',');
		getline(ip, Data->score, '\n');
		output.push_back(*Data);
		delete &Data;
	}
	
}