#include "ClassHighScore.h"

// HighScore Class
// ======================================================================================

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
	string temp;
	for (int check_mode = 0; check_mode < 3; ++check_mode)
	{
		// For Rank and Name (4 placments)
		if (check_mode != 2)
		{
			switch (check_mode)
			{
			case 0:
				temp = input.Data.rank;
				break;
			case 1:
				temp = input.Data.name;
				break;
			default:
				break;
			}
			switch (temp.size())
			{
			case 1:
				os << "   " << temp << ' ';
				break;
			case 2:
				os << "  " << temp << ' ';
				break;
			case 3:
				os << " " << temp << ' ';
				break;
			case 4:
				os << temp << ' ';
				break;

			}
		}
		// For Score (5 Placement)
		else if (check_mode == 2)
		{
			temp = input.Data.score;
			switch (temp.size())
			{
			case 1:
				os << "    " << temp;
				break;
			case 2:
				os << "   " << temp;
				break;
			case 3:
				os << "  " << temp;
				break;
			case 4:
				os << ' ' << temp;
				break;
			case 5:
				os << temp;
			}
		}
	}
	return os;
}
// ======================================================================================
