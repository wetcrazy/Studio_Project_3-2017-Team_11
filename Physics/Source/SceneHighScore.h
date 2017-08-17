#ifndef SCENE_HIGH_SCORE_H
#define SCENE_HIGH_SCORE_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "ClassHighScore.h"

class SceneHighScore : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	SceneHighScore();
	~SceneHighScore();

	virtual void Init();
	virtual void Update(double dt);
	virtual void CreateStuff();	//create game objects here
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
	float CheckCollision2(GameObject *go1, GameObject *go2);
	void CollisionResponse(GameObject *go1, GameObject *go2);

protected:

	//Physics
	std::vector<GameObject *> m_goList;
	
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
	GameObject *background;

	// HighScore Stuff
	vector<HighScore> highscore;
	const char *file_path;
};

#endif