#ifndef SCENEUPGRADE_H
#define SCENEUPGRADE_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "LoadingUpgrades.h"

class SceneUpgrade : public SceneBase
{
public:
	SceneUpgrade();
	~SceneUpgrade();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	//virtual void RenderMenu();
	virtual void Exit();

	//Setters
	void SetCurrentLevel(int levelNo);
	void SetScore(int score);
	void SetTempScore(int score);

	//Getting and Setting of currency
	void SetCurrency(int currency);
	int GetCurrency();

	//Getting and Setting of temporary currency
	void SetTempCurrency(int currency);
	int GetTempCurrency();

	void RenderGO(GameObject *go);

	enum UPGRADES_MENU
	{
		SPEED_UPGRADE_1 = 0,
		SPEED_UPGRADE_2,
		NEXTLEVEL,
		NEXTPAGE,
	};

protected:
	//Physics
	std::vector<GameObject *> m_goList;

	std::string fps;
	float m_worldWidth;
	float m_worldHeight;

	//Default optoin
	UPGRADES_MENU selectOptions = SPEED_UPGRADE_1;

	GameObject* FetchGO();

	//UpgradesMenu
	GameObject *arrows = FetchGO();
	GameObject *upgradesMenu = FetchGO();
	GameObject *speed_upgrade_1 = FetchGO();
	GameObject *speed_upgrade_2 = FetchGO();

	bool b_speed_upgrade_1, b_speed_upgrade_2;

	//.txt
	LoadingUpgrades upgraded;

	//Currency.txt
	int i_currency;

	//Cost of upgrade
	int i_cost_speed_upgrade_1;
	int i_cost_speed_upgrade_2;

	//Options
	float pressDelay = 0.f;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.2f;
};

#endif 