#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "LoadingUpgrades.h"
#include "LoadTXT.h"
#include "LoadCSV.h"
#include "ClassRace.h"

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
	virtual void CreateSplits(int pos, GameObject*base);
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
	void SetTotalScore(int score);
	int GetTotalScore();

	//Getting and Setting of temporary score
	void SetTempScore(int score);
	int GetTempScore();

	//Getting and Setting of level score
	void SetLevelScore(int score);

	//Getting and Setting of currency
	void SetCurrency(int currency);
	int GetCurrency();

	//Getting and Setting of temporary currency
	void SetTempCurrency(int currency);
	int GetTempCurrency();

	//Getting of save file
	int GetSaveFile();

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

	int i_projectileCount;

	bool b_splitDone;
	int i_despawnHexa;

	float launched;
	float scrollOffset;

	// Power Bar
	GameObject *powerbar;
	GameObject *powerrange;
	GameObject *guidemarker;
	GameObject *powerbg;
	size_t NumMode_tiggered_powerbar;
	bool is_movement_powerbar;
	float original_position_powerbar;
	int percentage_of_powerbar;

	//Upgrades
	LoadingUpgrades upgraded;

	bool b_isBulletAlive;
	float ft_bulletAlive;

	int i_level;

	//CurrentLevel.txt
	int i_CurrentLevel;

	//TempScore.txt
	int i_tempScore;

	//Level score
	int i_levelScore;

	//TempCurrency.txt
	int i_tempCurrency;

	//savefile.txt
	int i_saveFile;

	LoadTXT loadText;

	int i_projectileType;
	bool b_raceConfirmed;
	float rot;
	int speed;
	bool b_abilityUsed;

	float speed_multiplyer;

	Race raceTemp;

	// Indicator
	GameObject *indicator;

	// Profile
	std::vector<Profile> profile;
};

#endif