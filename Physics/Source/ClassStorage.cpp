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

// Upgrade Class
// ======================================================================================

// Constructor & Destructor
Upgrade::Upgrade()
{
	gravity = (0.0f, 0.0f, 0.0f);
	max_shots = 0;
	speed = 0.0f;
}

Upgrade::~Upgrade()
{

}

// Setters
void Upgrade::set_Gravity(Vector3 input)
{
	this->gravity = input;
}

void Upgrade::set_MaxShots(int input)
{
	this->max_shots = input;
}

void Upgrade::set_Speed(float input)
{
	this->speed = input;
}

// Getters
Vector3 Upgrade::get_Gravity()
{
	return this->gravity;
}

int Upgrade::get_MaxShots()
{
	return this->max_shots;
}

float Upgrade::get_Speed()
{
	return this->speed;
}
// ======================================================================================