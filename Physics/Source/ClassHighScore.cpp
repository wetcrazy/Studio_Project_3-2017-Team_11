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
	os << input.Data.rank << ' ' << input.Data.name << ' ' << input.Data.score;
	return os;
}