#include "LevelCreate.h"

LevelCreate::LevelCreate()
{
	i_level = 1;
	s_type = "";
	i_active = -1;
	ft_direction_X = 0;
	ft_direction_Y = 0;
	ft_direction_Z = 0;
	ft_position_X = 0;
	ft_position_Y = 0;
	ft_position_Z = 0;
	ft_scale_X = 0;
	ft_scale_Y = 0;
	ft_scale_Z = 0;
	ft_color_R = 0;
	ft_color_G = 0;
	ft_color_B = 0;
}

LevelCreate::~LevelCreate()
{
	i_level = 1;
	s_type = "";
	i_active = -1;
	ft_direction_X = 0;
	ft_direction_Y = 0;
	ft_direction_Z = 0;
	ft_position_X = 0;
	ft_position_Y = 0;
	ft_position_Z = 0;
	ft_scale_X = 0;
	ft_scale_Y = 0;
	ft_scale_Z = 0;
	ft_color_R = 0;
	ft_color_G = 0;
	ft_color_B = 0;
}

void LevelCreate::LevelSet(
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
	string colorB)
{
	this->i_level = std::stoi(level);
	this->s_type = type;
	this->i_active = std::stoi(active);
	this->ft_direction_X = std::stof(directionX);
	this->ft_direction_Y = std::stof(directionY);
	this->ft_direction_Z = std::stof(directionZ);
	this->ft_position_X = std::stof(positionX);
	this->ft_position_Y = std::stof(positionY);
	this->ft_position_Z = std::stof(positionZ);
	this->ft_scale_X = std::stof(scaleX);
	this->ft_scale_Y = std::stof(scaleY);
	this->ft_scale_Z = std::stof(scaleZ);
	this->ft_color_R = std::stof(colorR);
	this->ft_color_G = std::stof(colorG);
	this->ft_color_B = std::stof(colorB);
}

int LevelCreate::get_level()
{
	return this->i_level;
}

string LevelCreate::get_type()
{
	return this->s_type;
}

int LevelCreate::get_active()
{
	return this->i_active;
}

float LevelCreate::get_dirx()
{
	return this->ft_direction_X;
}

float LevelCreate::get_diry()
{
	return this->ft_direction_Y;
}

float LevelCreate::get_dirz()
{
	return this->ft_direction_Z;
}

float LevelCreate::get_posx()
{
	return this->ft_position_X;
}

float LevelCreate::get_posy()
{
	return this->ft_position_Y;
}

float LevelCreate::get_posz()
{
	return this->ft_position_Z;
}

float LevelCreate::get_scax()
{
	return this->ft_scale_X;
}

float LevelCreate::get_scay()
{
	return this->ft_scale_Y;
}

float LevelCreate::get_scaz()
{
	return this->ft_scale_Z;
}

float LevelCreate::get_colr()
{
	return this->ft_color_R;
}

float LevelCreate::get_colg()
{
	return this->ft_color_G;
}

float LevelCreate::get_colb()
{
	return this->ft_color_B;
}
