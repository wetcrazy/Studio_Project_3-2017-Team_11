#ifndef CLASS_RACE_H
#define CLASS_RACE_H
 // color ,scale ,pos , target, direction, velocity
#include "Vector3.h"

class Race
{
public:
	Race();
	~Race();
	// Get variables vec3
	//Inputs = (1,Color)(2,Scale)(3,Pos)(4,Target)(5,Direction)(6,Velocity)
	Vector3 Getter_Vec3(int input);
	// Get variables float
	// Inputs = (1, Mass)
	float Getter_float(int input);
	// Setter
	void SetAll(Vector3 color, Vector3 scale, Vector3 pos, Vector3 target, Vector3 dir, Vector3 vel, float mass);
	void Set_Color(Vector3 color);
	void Set_Scale(Vector3 scale);
	void Set_Pos(Vector3 pos);
	void Set_Target(Vector3 target);
	void Set_Dir(Vector3 dir);
	void Set_Vel(Vector3 vel);
	void Set_Mass(float mass);
	// Reset everything to 0
	virtual void ResetRace();

private:
	// Private variables
	Vector3 color, scale, pos, target, dir, vel;
	float mass;
};

#endif