#include "ClassStorage.h"
#include "LoadTXT.h"

// HighScore Class
// ======================================================================================

// Constructor & Destructor
HighScore::HighScore()
{

}

HighScore::~HighScore()
{

}

// Functions
void HighScore::HighScore_Calculator(const char *file_ScoreDestination, const char *file_LevelDestination) 
{
	int curr_score, curr_level;
	ifstream curr_File;

	// Open score file
	curr_File.open(file_ScoreDestination);
	curr_File >> curr_score;
	curr_File.close();
	// Open level file
	curr_File.open(file_LevelDestination);
	curr_File >> curr_level;
	curr_File.close();

	// Return Highscore (Total_Score / Num_Levels Done)
	int level = curr_level - 1; //while in fail screen, the level switches to next level
	if (level < 1)
		level = 1;
	this->Data.score = std::to_string(curr_score / level);
}


// Operator
ostream &operator<<(ostream &os, HighScore &input)
{
	string temp;
	for (int check_mode = 0; check_mode < 4; ++check_mode)
	{
		// For Rank (4 placments)
		if (check_mode == 0)
		{
			temp = input.Data.rank;
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
		// For Rank
		else if (check_mode == 1)
		{
			temp = input.Data.level;
			switch (temp.size())
			{
			case 1:
				os << "  " << temp << "   ";
				break;
			case 5:
				os << temp << ' ';
				break;
			}		
		}
		// For Saved file number
		else if (check_mode == 2)
		{
			temp = input.Data.name;
			os << temp << ' ';
		}
		// For Score (5 Placement)
		else if (check_mode == 3)
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
				break;
			}
		}
	}
	return os;
}
// ======================================================================================

// Profile Class
// ======================================================================================

// Constructor & Destructor
Profile::Profile()
{

}

Profile::~Profile()
{

}
// ======================================================================================