#ifndef CLASS_RACE_H_
#define CLASS_RACE_H_

#include <iostream>
#include <string>
#include <vector>
#include "GameObject.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

class Race
{
public:
	Race();
	~Race();
public:
	Race(GameObject temp);
	void f_setDraft(GameObject *temp);
	string sf_CheckRace_N();
	string sf_CheckRace_PTR();
	bool b_CheckRace_GENERAL();
public:
	Vector3 vf_getVectAbility(int i_projectileType);
	GameObject *aSplit;
	GameObject *bSplit;
private:
	GameObject tempRace_N;
	GameObject *tempRace_PTR;
};

#endif // !CLASS_RACE_H_
