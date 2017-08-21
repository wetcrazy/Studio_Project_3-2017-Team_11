#include "ClassRace.h"

// ============================
// Construtor and Desconstrutor
Race::Race() // Set them to 0
{
	color.Set(0.0f, 0.0f, 0.0f);
	scale.Set(0.0f, 0.0f, 0.0f);
	pos.Set(0.0f, 0.0f, 0.0f);
	target.Set(0.0f, 0.0f, 0.0f);
	dir.Set(0.0f, 0.0f, 0.0f);
	vel.Set(0.0f, 0.0f, 0.0f);
}

Race::~Race()
{

}
// ============================

// Initalize/Set all variables
// Inputs = (1,Color)(2,Scale)(3,Pos)(4,Target)(5,Direction)(6,Velocity);
void Race::Init(Vector3 color, Vector3 scale, Vector3 pos, Vector3 target, Vector3 dir, Vector3 vel)
{
	this->color = color;
	this->scale = scale;
	this->pos = pos;
	this->target = target;
	this->dir = dir;
	this->vel = vel;
}

// Get variables
// Inputs = (1,Color)(2,Scale)(3,Pos)(4,Target)(5,Direction)(6,Velocity);
Vector3 Race::Getter(int input)
{
	Vector3 output;
	switch (input)
	{
	case 1:
		output = this->color;
		break;
	case 2:
		output = this->scale;
		break;
	case 3:
		output = this->pos;
		break;
	case 4:
		output = this->target;
		break;
	case 5:
		output = this->dir;
		break;
	case 6:
		output = this->vel;
		break;
	default:
		break;
	}
	return output;
}