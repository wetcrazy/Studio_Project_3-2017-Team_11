#include "ClassHighScore.h"

// Constructor & Destructor
HighScore::HighScore()
{

}

HighScore::~HighScore()
{

}

// Operator
ostream &operator<<(ostream &os, HighScore &input)
{
	if (input.Data.rank == "RANK")
	{
		os << input.Data.rank << ' ' << input.Data.name << ' ' << input.Data.score;
	}
	else
	{
		os << "   " << input.Data.rank << " " << input.Data.name << " " << input.Data.score;
	}
	
	return os;
}