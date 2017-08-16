#ifndef CLASS_HIGH_SCORE_H
#define CLASS_HIGH_SCORE_H

#include <string>
#include <vector>
#include <ostream>

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

	// Struct Storage for Scores
	Score Data;

	// QuickSort Functions
	void QuickSort(vector<HighScore> *list, int first_index, int last_index);
	void Swap(vector<HighScore> *list, int first_index, int second_index);
	int Partition(vector<HighScore> *list, int first_index, int last_index);

	// Operator
	friend ostream &operator<<(ostream &os, HighScore &input);
};


#endif