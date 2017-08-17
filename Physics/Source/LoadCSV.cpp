#include <fstream>
#include <iostream>

#include "LoadCSV.h"

using std::getline;

// Function to Load .CSV
// Returns vector<Score>
std::vector<HighScore> LoadCSV(const char *file_path)
{
	std::vector<HighScore> output;
	HighScore *temp;
	
	std::ifstream ip(file_path);
	if (!ip.good()) // If cannnot read, return ERROR
	{
		temp = new HighScore;
		std::cout << "ERROR " << file_path << " Not Found~" << std::endl;	
		temp->Data.rank = "RANK", temp->Data.name = "NAME", temp->Data.score = "SCORE";
		output.push_back(*temp);
		temp->Data.rank = "ERROR", temp->Data.name = "ERROR", temp->Data.score = "ERROR";
		output.push_back(*temp);
		return output;
	}
	else
		std::cout << file_path << " File Opened~" << std::endl;
	
	while (ip.good()) // Returns the temp from .CSV
	{
		temp = new HighScore;
		getline(ip, temp->Data.rank, ',');
		getline(ip, temp->Data.name, ',');
		getline(ip, temp->Data.score, '\n');
		output.push_back(*temp);
	}
	ip.close();
	return output;
}

// Function to Delete .CSV
// Returns vector<HighScore>
std::vector<HighScore> DeleteCSV(const char *file_path)
{
	std::vector<HighScore> output;

	// ReCreate file
	std::ofstream ip;
	ip.open(file_path);
	ip << "RANK,NAME,SCORE";
	ip.close();
	std::cout << file_path << " File Deleted~" << std::endl;

	// Load file
	output = LoadCSV(file_path);
	return output;
}

// Function to Write into .CSV
// Returns vectore<HighScore>
std::vector<HighScore> WriteCSV(const char *file_path)
{
	std::vector<HighScore> input;
	std::vector<HighScore> output;

	input = LoadCSV(file_path);
	return output;
}