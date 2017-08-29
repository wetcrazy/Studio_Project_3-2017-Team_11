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
		GO_HEXA,
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

		//Background
		GO_MOUNTAIN,
		GO_BACKGROUND_FIRE,
		GO_FOREGROUND_FIRE,

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
		GO_UPGRADESMENU_NEXTPAGE,
		GO_SPEED_UPGRADE_1,
		GO_SPEED_UPGRADE_2,
		GO_SPEED_MULTIPLYER_UPGRADE_1,
		GO_SPEED_MULTIPLYER_UPGRADE_2,

		//Highscore Menu
		GO_HIGHSCOREMENU,

		//Save File Menu
		GO_SAVE1,
		GO_SAVE2,
		GO_SAVE3,

		// Power Bar
		GO_POWERBAR,
		GO_POWERRANGE,
		GO_GUIDEMARKER,
		GO_POWER_BG,
		GO_POWERRANGE_FIRED,

		// Fail Menu
		GO_FAILMENU,

		//Success Menu
		GO_SUCCESSMENU,

		//Star
		GO_STAR,

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