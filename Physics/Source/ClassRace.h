#ifndef CLASS_RACE_H
#define CLASS_RACE_H
 // color ,scale ,pos , target, direction, velocity
#include "Vector3.h"

class Race
{
public:
	Race();
	~Race();
	// Initalize/Set all variables
	void Init(Vector3 color, Vector3 scale, Vector3 pos, Vector3 target, Vector3 dir, Vector3 vel);
	// Get variables
	Vector3 Getter(int input);
private:
	Vector3 color, scale, pos, target, dir, vel;
};

#endif