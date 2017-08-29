#ifndef LOAD_CSV_H
#define LOAD_CSV_H

#include <vector>
#include <string>

class HighScore;
class Profile;

using std::string;

// HighScore

// Function to Load .CSV
// Returns vector<HighScore>
std::vector<HighScore> LoadCSV(const char *file_path);

// Function to Delete .CSV
// Returns vector<HighScore>
std::vector<HighScore> DeleteCSV(const char *file_path);

// Function to Write into .CSV
// Returns vectore<HighScore>
void WriteCSV(const char *file_path, std::vector<HighScore> input);


// Profile

// Function to Load .CSV
// Returns Profile, temp is just there to overload function
std::vector<Profile> LoadCSV_2(string name);

// Function to Delete .CSV
// Returns Profile, temp is just there to overload function
void DeleteCSV_2(string name);

// Function to Write into .CSV
// Returns Profile
void WriteCSV_2(string name, std::vector<Profile> input);

#endif