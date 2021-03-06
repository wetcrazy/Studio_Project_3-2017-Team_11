#ifndef SCENE_HIGH_SCORE_H
#define SCENE_HIGH_SCORE_H

#include <vector>

#include "SceneBase.h"
#include "GameObject.h"

class HighScore;

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

	enum UPGRADE_MENU
	{
		DELETE_HIGHSCORE = 0,
		EXIT_HIGHSCORE,
	};

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

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

	//Default option
	UPGRADE_MENU selectOptions = DELETE_HIGHSCORE;

	//Arrows
	GameObject *arrows = FetchGO();
	GameObject *arrowsRight = FetchGO();
	GameObject *highscoreMenu = FetchGO();

	//Options
	float pressDelay = 0.f;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.5f;

	// HighScore Stuff
	std::vector<HighScore> highscore;
	const char *file_path;

	// Profile
	std::vector<Profile> profile1;
	std::vector<Profile> profile2;
	std::vector<Profile> profile3;
};

#endif