#ifndef LOAD_CSV_H
#define LOAD_CSV_H

#include <vector>
#include <string>

#include "ClassHighScore.h"

using std::string;

// Function to Load .CSV
// Returns vector<Score>
std::vector<HighScore> LoadCSV(const char *file_path);


#endif