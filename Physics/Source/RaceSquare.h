#ifndef RACE_SQUARE_H
#define RACE_SQUARE_H
 // color ,scale ,pos , target, direction, velocity
#include "ClassRace.h"

class Square : public Race
{
public:
	Square();
	~Square();

	// Ablities 
	void Abilites();

	// Getter
	bool Get_Piercing();
	// Setters
	void Set_Piercing(bool is_Piercing);
private:
	bool is_Piercing;
};

#endif