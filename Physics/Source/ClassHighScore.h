#ifndef CLASS_HIGH_SCORE_H
#define CLASS_HIGH_SCORE_H

//#include <string>
//#include <vector>
//#include <ostream>
//
//using std::string;
//using std::vector;
//using std::ostream;
#include "ClassStorage.h"

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

	// Struct Storage for Scores
	Score Data;

	// Operator
	friend ostream &operator<<(ostream &os, HighScore &input);
};


#endif