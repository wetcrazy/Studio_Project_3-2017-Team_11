#ifndef CLASS_STORAGE_H
#define CLASS_STORAGE_H

#include <string>
#include <vector>
#include <ostream>

#include "Vector3.h"

using std::string;
using std::vector;
using std::ostream;

class HighScore
{
protected:
	// Struct Storage for Scores
	struct Score
	{
		string rank;
		string name;
		string score;
	}; 

public:
	// Constructor & Destructor
	HighScore();
	~HighScore();

	// Struct Storage for Scores
	Score Data;

	// Operator
	friend ostream &operator<<(ostream &os, HighScore &input);
};

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