
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	momentOfInertia(1.f),
	angularVeloctiy(1.f),
	Color(1.f, 1.f, 1.f)
{
}

GameObject::~GameObject()
{
}