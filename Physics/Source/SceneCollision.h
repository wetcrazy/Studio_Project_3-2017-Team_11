#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

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
	virtual void Render();
	virtual void RenderUpgradesMenu(double dt);	//upgrades are located here
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
	float CheckCollision2(GameObject *go1, GameObject *go2);
	void CollisionResponse(GameObject *go1, GameObject *go2);

protected:

	//Physics
	std::vector<GameObject *> m_goList;
	
	GameObject *m_ghost01;
	GameObject *m_ghost02;

	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;

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
	float ft_shootTime;
	float ft_elapsedTime;
	bool b_shootIsTrue;

	//Background
	GameObject *background;

	//Upgrades Menu
	GameObject *upgradesMenu;
	bool b_upgradesMenu_IsOpen;
	float pressDelay = 0.f;
	const float cooldownPressed = 0.2f;
	bool b_upgrades1;
};

#endif