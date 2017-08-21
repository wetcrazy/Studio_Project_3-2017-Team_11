#ifndef CLASS_UPGRADE_H
#define CLASS_UPGRADE_H

//#include "Vector3.h"
//
//#include <string>
//#include <vector>
//#include <ostream>
//
//using std::string;
//using std::vector;
//using std::ostream;
#include "ClassStorage.h"

class Upgrade
{
public:
	// Constructor & Destructor
	Upgrade();
	~Upgrade();

	// Setter
	void set_Gravity(Vector3 input);
	void set_MaxShots(int input);
	void set_Speed(float input);
	// Getter
	Vector3 get_Gravity();
	int get_MaxShots();
	float get_Speed();

private:
	// Variables
	Vector3 gravity; // Barrel Upgrade
	int max_shots; // Max number of shots
	float speed; // Speed of shot
};

#endif