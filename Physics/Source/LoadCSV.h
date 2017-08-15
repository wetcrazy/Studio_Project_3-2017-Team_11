#ifndef LOAD_CSV_H
#define LOAD_CSV_H

#include <vector>

struct Score
{
	string rank;
	string name;
	string score;
}; 

std::vector<Score> LoadCSV(const char *file_path);

#endif