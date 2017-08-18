#ifndef LOAD_CSV_H
#define LOAD_CSV_H

#include <vector>
#include <string>

class HighScore;
class Upgrade;

using std::string;

// Function to Load .CSV
// Returns vector<HighScore>
std::vector<HighScore> LoadCSV(const char *file_path);

// Function to Delete .CSV
// Returns vector<HighScore>
std::vector<HighScore> DeleteCSV(const char *file_path);

// Function to Write into .CSV
// Returns vectore<HighScore>
void WriteCSV(const char *file_path, std::vector<HighScore> input);

#endif