#include "ClassStorage.h"

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
				os << "    " << temp << ' ';
				break;
			case 2:
				os << "   " << temp << ' ';
				break;
			case 3:
				os << "  " << temp << ' ';
				break;
			case 4:
				os << ' ' << temp << ' ';
				break;
			case 5:
				os << temp << ' ';
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