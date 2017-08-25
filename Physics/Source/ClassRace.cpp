#include "ClassRace.h"

Race::Race()
{
	this->tempRace_N.type = GameObject::GO_BALL;
	this->tempRace_N.active = false;
	this->tempRace_N.dir.SetZero();
	this->tempRace_N.pos.SetZero();
	this->tempRace_N.scale.Set(1, 1, 1);
	this->tempRace_N.Color.Set(0.5f, 0.5f, 0.5f);
	this->tempRace_N.mass = 1.0f;
	this->tempRace_N.momentOfInertia = 1.0f;
	this->tempRace_N.angularVeloctiy = 1.0f;

}

Race::~Race()
{
}

Race::Race(GameObject temp)
{
	this->tempRace_N = temp;
}

void Race::f_setDraft(GameObject *temp)
{
	this->tempRace_PTR = temp;
}

string Race::sf_CheckRace_N()
{
	if (this->tempRace_N.type == GameObject::GO_BALL)
		return "Ball is True!\n";
	else if (this->tempRace_N.type == GameObject::GO_CUBE)
		return "Cube is True!\n";
	else if (this->tempRace_N.type == GameObject::GO_HEXA)
		return "Hexa is True!\n";
	else
		return "sf_CheckRace_N() Error!\n";
}

string Race::sf_CheckRace_PTR()
{
	if (this->tempRace_PTR->type == GameObject::GO_BALL)
		return "BALL IS TRUE!\n";
	else if (this->tempRace_PTR->type == GameObject::GO_CUBE)
		return "CUBE IS TRUE!\n";
	else if (this->tempRace_PTR->type == GameObject::GO_HEXA)
		return "HEXA IS TRUE!\n";
	else
		return "sf_CheckRace_PTR() Error!\n";
}

bool Race::b_CheckRace_GENERAL()
{
	if (this->tempRace_PTR->type == GameObject::GO_BALL)
		return true;
	else if (this->tempRace_PTR->type == GameObject::GO_CUBE)
		return true;
	else if (this->tempRace_PTR->type == GameObject::GO_HEXA)
		return true;
	else
		return false;
}

Vector3 Race::vf_getVectAbility(int i_projectileType)
{
	if (i_projectileType == 1)
	{
		return Vector3(1, 1, 1);
	}
	else if (i_projectileType == 2)
	{
		return Vector3(5, 5, 5);
	}
	else
		return Vector3(1, 1, 1);
}


