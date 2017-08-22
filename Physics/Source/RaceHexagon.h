#ifndef RACE_HEXAGON_H
#define RACE_HEXAGON_H
 // color ,scale ,pos , target, direction, velocity
#include "ClassRace.h"

class Hexagon :public Race
{
public:
	Hexagon();
	~Hexagon();

	// Abilites
	void Abilites();
	// Setter
	void Set_Spilt(bool if_spilt);
	// Getter
	bool Get_Spilt();
private:
	bool is_spilt;
};

#endif