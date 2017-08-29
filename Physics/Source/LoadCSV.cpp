#include <fstream>
#include <iostream>
#include <ostream>

#include "LoadCSV.h"
#include "ClassStorage.h"

using std::getline;

// =============== HighScore ===============

// Function to Load .CSV
// Returns vector<Score>
std::vector<HighScore> LoadCSV(const char *file_path)
{
	// Tempoary Variables
	std::vector<HighScore> output;
	HighScore *temp;
	
	// Open file to read
	std::ifstream ip(file_path);
	if (!ip.good()) // If cannnot read, return ERROR
	{
		temp = new HighScore;
		std::cout << "ERROR " << file_path << " Not Found~" << std::endl;	
		temp->Data.rank = "RANK", temp->Data.level = "LEVEL", temp->Data.name = "SAVED", temp->Data.score = "SCORE";
		output.push_back(*temp);
		temp->Data.rank = "ERROR", temp->Data.level = "ERROR", temp->Data.name = "ERROR", temp->Data.score = "ERROR";
		output.push_back(*temp);
		return output;
	}
	else
		std::cout << file_path << " File Opened~" << std::endl;
	
	while (ip.good()) // Returns the temp from .CSV
	{
		temp = new HighScore;
		getline(ip, temp->Data.rank, ',');
		getline(ip, temp->Data.level, ',');
		getline(ip, temp->Data.name, ',');
		getline(ip, temp->Data.score, '\n');
		output.push_back(*temp);
	}
	ip.close(); // Close the file
	return output; // Return vector<HighScore>
}

// Function to Delete .CSV
// Returns vector<HighScore>
std::vector<HighScore> DeleteCSV(const char *file_path)
{
	// Tempoary Variables
	std::vector<HighScore> output;

	// ReCreate file
	std::ofstream ip;
	ip.open(file_path);
	ip << "RANK,LEVEL,SAVED,SCORE";
	ip.close();
	std::cout << file_path << " File Deleted~" << std::endl;

	// Load file
	output = LoadCSV(file_path);
	return output;// Return vector<HighScore>
}

// Function to Write into .CSV
// Returns vectore<HighScore>
void WriteCSV(const char *file_path, std::vector<HighScore> input)
{
	// Recreate file
	std::ofstream ip;
	ip.open(file_path);
	for (int check_index = 0; check_index < input.size(); ++check_index) // Write down each vector into file
	{
		if (check_index == input.size() - 1) // Prevent addition of function endl at the end of the vector
		{
			ip << input[check_index].Data.rank << ',' << input[check_index].Data.level << ',' << input[check_index].Data.name << ',' << input[check_index].Data.score;
		}
		else
			ip << input[check_index].Data.rank << ',' << input[check_index].Data.level << ',' << input[check_index].Data.name << ',' << input[check_index].Data.score << std::endl;
	}
	ip.close();
	std::cout << file_path << " File OverWritten~" << std::endl;
}


// =============== Profile ===============

// Function to Load .CSV
// Returns Profile, temp is just there to overload function
vector<Profile> LoadCSV_2(size_t loadnum)
{
	// Tempoary Variables
	vector<Profile> output;
	Profile *temp1;
	const char *file_path;
	if (loadnum == 1)
	{
		file_path = "Save1//Profile.csv";
	}
	else if (loadnum == 2)
	{
		file_path = "Save2//Profile.csv";
	}
	else if (loadnum == 3)
	{
		file_path = "Save3//Profile.csv";
	}

	// Open file to read
	std::ifstream ip(file_path);
	std::cout << file_path << " File Opened~" << std::endl;

	while (ip.good())
	{
		temp1 = new Profile;
		getline(ip, temp1->Table.level, ',');
		getline(ip, temp1->Table.score, '\n');
		output.push_back(*temp1);
	}
	ip.close(); // Close the file
	return output; // Return Profile
}

// Function to Delete .CSV
// Returns Profile, temp is just there to overload function
void DeleteCSV_2(size_t loadnum)
{
	// Tempoary Variables
	const char *file_path;
	if (loadnum == 1)
	{
		file_path = "Save1//Profile.csv";
	}
	else if (loadnum == 2)
	{
		file_path = "Save2//Profile.csv";
	}
	else if (loadnum == 3)
	{
		file_path = "Save3//Profile.csv";
	}

	// ReCreate file
	std::ofstream ip;
	ip.open(file_path);
	ip.close();
	std::cout << file_path << " File Deleted~" << std::endl;
}

// Function to Write into .CSV
// Returns Profile
void WriteCSV_2(vector<Profile> input, size_t loadnum)
{
	// Tempoary Variables
	const char *file_path;
	if (loadnum == 1)
	{
		file_path = "Save1//Profile.csv";
	}
	else if (loadnum == 2)
	{
		file_path = "Save2//Profile.csv";
	}
	else if (loadnum == 3)
	{
		file_path = "Save3//Profile.csv";
	}

	// Recreate file
	std::ofstream ip;
	ip.open(file_path);
	for (int check_index = 0; check_index < input.size(); ++check_index) // Write down each vector into file
	{
		if (!input[check_index].Table.level.empty() && !input[check_index].Table.score.empty())
		{
			if (check_index == input.size() - 1) // Prevent addition of function endl at the end of the vector
			{
				ip << input[check_index].Table.level << ',' << input[check_index].Table.score;
			}
			else
				ip << input[check_index].Table.level << ',' << input[check_index].Table.score << std::endl;
		}
	}
	ip.close();
	std::cout << file_path << " File OverWritten~" << std::endl;
}