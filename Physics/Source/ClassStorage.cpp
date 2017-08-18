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