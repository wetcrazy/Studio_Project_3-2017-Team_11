#include "RaceSquare.h"

Square::Square()
{
	is_Piercing = false;
}

Square::~Square()
{

}

void Square::Abilites()
{
	this->Set_Piercing(true);
}

bool Square::Get_Piercing()
{
	return this->is_Piercing;
}

void Square::Set_Piercing(bool is_Piercing)
{
	this->is_Piercing = is_Piercing;
}