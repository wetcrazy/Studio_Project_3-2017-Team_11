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
	mass = 0.0f;
}

Race::~Race()
{

}
// ============================

// Get variables vec3
//Inputs = (1,Color)(2,Scale)(3,Pos)(4,Target)(5,Direction)(6,Velocity)(7,Mass)
Vector3 Race::Getter_Vec3(int input)
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

// Get variables float
// Inputs = (1, Mass)
float Race::Getter_float(int input)
{
	float output;
	switch (input)
	{
	case 1:
		output = this->mass;
		break;
	default:
		break;
	}
	return output;
}

// Set all variables
// Inputs = (1,Color)(2,Scale)(3,Pos)(4,Target)(5,Direction)(6,Velocity);
void Race::SetAll(Vector3 color, Vector3 scale, Vector3 pos, Vector3 target, Vector3 dir, Vector3 vel, float mass)
{
	this->color = color;
	this->scale = scale;
	this->pos = pos;
	this->target = target;
	this->dir = dir;
	this->vel = vel;
	this->mass = mass;
}

// Setters
void Race::Set_Color(Vector3 color)
{
	this->color = color;
}

void Race::Set_Scale(Vector3 scale)
{
	this->scale = scale;
}

void Race::Set_Pos(Vector3 pos)
{
	this->pos = pos;
}

void Race::Set_Target(Vector3 target)
{
	this->target = target;
}

void Race::Set_Dir(Vector3 dir)
{
	this->dir = dir;
}

void Race::Set_Vel(Vector3 vel)
{
	this->vel = vel;
}

void Race::Set_Mass(float mass)
{
	this->mass = mass;
}

// Reset to 0 for everything
void Race::ResetRace()
{
	this->color.Set(0.0f, 0.0f, 0.0f);
	this->scale.Set(0.0f, 0.0f, 0.0f);
	this->pos.Set(0.0f, 0.0f, 0.0f);
	this->target.Set(0.0f, 0.0f, 0.0f);
	this->dir.Set(0.0f, 0.0f, 0.0f);
	this->vel.Set(0.0f, 0.0f, 0.0f);
	this->mass = 0.0f;
}