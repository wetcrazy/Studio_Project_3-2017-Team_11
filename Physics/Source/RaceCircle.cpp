#include "RaceCircle.h"

Circle::Circle()
{

}

Circle::~Circle()
{

}

void Circle::Abilites()
{
	this->Set_Scale(Vector3(5.0f, 5.0f, 5.0f)); // Incrase Scale size
	this->Set_Mass(10.0f); // Incrase mass
}