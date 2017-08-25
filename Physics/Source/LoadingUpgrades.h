#ifndef LOADING_UPGRADES_H
#define LOADING_UPGRADES_H

#include <string>
#include <iostream>
#include <fstream>

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

class LoadingUpgrades
{
public:

	//Speed Upgrades
	int speed_upgrade;
	//Speed Multiplyer Upgrades
	int speed_multiplyer_upgrade;

	//Temp
	string tempString;

	void ReadFile(string fileName);
	void DeleteTextFile(string fileName);
	void WriteFile(string fileName, string type, int number);
};

#endif