#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "LoadingUpgrades.h"
#include "LoadTXT.h"

class SceneCollision : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	SceneCollision();
	~SceneCollision();

	virtual void Init();
	virtual void Update(double dt);
	virtual void CreateStuff();	//create game objects here
	virtual void CreateLevel(int i_level);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
	float CheckCollision2(GameObject *go1, GameObject *go2);
	void CollisionResponse(GameObject *go1, GameObject *go2);

	//Getting and Setting current levels
	void SetCurrentLevel(int levelNo);
	int GetCurrentLevel();

	//Getting and Setting of score
	void SetScore(int score);
	int GetScore();

	//Getting and Setting of temporary score
	void SetTempScore(int score);
	int GetTempScore();

protected:

	//Physics
	std::vector<GameObject *> m_goList;

	GameObject *m_ghost;

	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	int fortCount;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
	Vector3 initialMomentum, finalMomentum;
	Vector3 gravity;
	float initialKE, finalKE;

	//Cannon
	GameObject *platform;
	GameObject *cannon;
	Vector3 aim;

	//Background
	GameObject *mountain;
	GameObject *background_fire;
	GameObject *foreground_fire;

	//Scrolling
	GameObject *projectile;
	float launched;
	float scrollOffset;

	// Power Bar
	GameObject *powerbar;
	GameObject *powerrange;
	GameObject *guidemarker;
	size_t NumMode_tiggered_powerbar;
	bool is_movement_powerbar;
	float original_position_powerbar;

	//Upgrades
	LoadingUpgrades upgraded;

	bool b_isBulletAlive;
	float ft_bulletAlive;

	int i_level;

	//CurrentLevel.txt
	int i_CurrentLevel;

	//TempScore.txt
	int i_tempScore;

	LoadTXT loadText;

	int i_projectileType;

};

#endif