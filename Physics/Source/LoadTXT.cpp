#include "LoadTXT.h"

LoadTXT::LoadTXT()
{
}

LoadTXT::~LoadTXT()
{
}

std::vector<LevelCreate> LoadTXT::LoadTextFile()
{
	std::vector<LevelCreate> levelList;
	LevelCreate tempLevel;

	string st_level;
	string st_type;
	string st_active;
	string st_direction_X;
	string st_direction_Y;
	string st_direction_Z;
	string st_position_X;
	string st_position_Y;
	string st_position_Z;
	string st_scale_X;
	string st_scale_Y;
	string st_scale_Z;
	string st_color_r;
	string st_color_g;
	string st_color_b;

	ifstream file("level.txt");
	if (!file.is_open())
		cout << "File Error F0001!\n";
	while (file.good())
	{
		getline(file, st_level, ',');
		getline(file, st_type, ',');
		getline(file, st_active, ',');
		getline(file, st_direction_X, ',');
		getline(file, st_direction_Y, ',');
		getline(file, st_direction_Z, ',');
		getline(file, st_position_X, ',');
		getline(file, st_position_Y, ',');
		getline(file, st_position_Z, ',');
		getline(file, st_scale_X, ',');
		getline(file, st_scale_Y, ',');
		getline(file, st_scale_Z, ',');
		getline(file, st_color_r, ',');
		getline(file, st_color_g, ',');
		getline(file, st_color_b, '\n');

		tempLevel.LevelSet(st_level, st_type, st_active, st_direction_X, st_direction_Y, st_direction_Z, st_position_X, st_position_Y, st_position_Z, st_scale_X, st_scale_Y, st_scale_Z, st_color_r, st_color_g, st_color_b);

		levelList.push_back(tempLevel);
	}
	return levelList;
}
