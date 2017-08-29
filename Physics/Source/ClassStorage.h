#ifndef CLASS_STORAGE_H
#define CLASS_STORAGE_H

#include <string>
#include <vector>
#include <ostream>

#include "Vector3.h"

using std::string;
using std::vector;
using std::ostream;

class HighScore
{
protected:
	// Struct Storage for Scores
	struct Score
	{
		string rank;
		string name;
		string score;
	}; 

public:
	// Constructor & Destructor
	HighScore();
	~HighScore();

	// Functions
	// Function input = (&const char)
	void HighScore_Calculator(const char *file_ScoreDestination, const char *file_LevelDestination); // returns highscore for that instance

	// Struct Storage for Scores
	Score Data;

	// Operator
	friend ostream &operator<<(ostream &os, HighScore &input);
};
#endif