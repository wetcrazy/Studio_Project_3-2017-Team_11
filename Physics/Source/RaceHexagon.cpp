#include "RaceHexagon.h"

Hexagon::Hexagon()
{
	is_spilt = false;
}

Hexagon::~Hexagon()
{

}

void Hexagon::Abilites()
{
	this->Set_Spilt(true);
}

void Hexagon::Set_Spilt(bool is_spilt)
{
	this->is_spilt = is_spilt;
}

bool Hexagon::Get_Spilt()
{
	return this->is_spilt;
}