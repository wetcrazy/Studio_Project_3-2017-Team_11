#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Material.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_WALL,
		GO_PILLAR,
		GO_BLOCKS,
		GO_ASTEROID, //asteroid
		GO_SHIP, //player ship
		GO_BULLET, //player bullet
		GO_ENEMY,
		GO_ENEMY_BULLET,
		GO_POWERUP,

		GO_BACKGROUND,
		GO_GROUND,
		GO_PLATFORM,

		GO_CANNON_PLATFORM,
		GO_CANNON,

		//Arrow for Menus
		GO_ARROW,

		//Main Menu
		GO_MAINMENU,

		//Upgrades Menu
		GO_UPGRADESMENU,
		GO_GRADE1,
		GO_GRADE2,
		GO_NEXTLEVEL,
		GO_NEXTPAGE,

		//Highscore Menu
		GO_HIGHSCOREMENU,
		
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 dir;
	Vector3 Color;
	float momentOfInertia;
	float angularVeloctiy;
	bool active;
	float mass;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif