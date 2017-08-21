#ifndef LEVEL_CREATE_H_
#define LEVEL_CREATE_H_

#include <iostream>
#include <string>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;

class LevelCreate
{
public:
	LevelCreate();
	~LevelCreate();
public:
	void LevelSet(
		string level, 
		string type, 
		string active, 
		string directionX, 
		string directionY, 
		string directionZ, 
		string positionX, 
		string positionY, 
		string positionZ, 
		string scaleX, 
		string scaleY, 
		string scaleZ,
		string colorR,
		string colorG,
		string colorB);
public:
	int get_level();
	string get_type();
	int get_active();
	float get_dirx();
	float get_diry();
	float get_dirz();
	float get_posx();
	float get_posy();
	float get_posz();
	float get_scax();
	float get_scay();
	float get_scaz();
	float get_colr();
	float get_colg();
	float get_colb();
private:
	int i_level;
	string s_type;
	int i_active;
	float ft_direction_X;
	float ft_direction_Y;
	float ft_direction_Z;
	float ft_position_X;
	float ft_position_Y;
	float ft_position_Z;
	float ft_scale_X;
	float ft_scale_Y;
	float ft_scale_Z;
	float ft_color_R;
	float ft_color_G;
	float ft_color_B;
};

#endif // !LEVEL_CREATE_H_
