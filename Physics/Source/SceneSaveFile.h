#ifndef SAVE_FILE_H
#define SAVE_FILE_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "LoadingUpgrades.h"

class SceneSaveFile : public SceneBase
{
public:
	SceneSaveFile();
	~SceneSaveFile();

	virtual void Init();
	virtual void Update(double dt);
	virtual void DeleteSaveFile(string fileName);
	virtual void Render();
	virtual void Exit();

	//Setters
	void SetSaveFile(int selected_file);
	void SetCurrentLevel(int levelNo);
	void SetScore(int score);
	void SetTempScore(int score);

	//Setting of currency
	void SetCurrency(int currency);

	//Setting of temporary currency
	void SetTempCurrency(int currency);

	void RenderGO(GameObject *go);

	enum SAVE_MENU
	{
		SAVE1,
		SAVE2,
		SAVE3,
		SELECT,
		DELETE_FILE,
		MAIN_MENU
	};

protected:
	//Physics
	std::vector<GameObject *> m_goList;

	std::string fps;
	float m_worldWidth;
	float m_worldHeight;

	//Default optoin
	SAVE_MENU selectOptions = SAVE1;

	GameObject* FetchGO();

	//Save Menu
	GameObject *arrows = FetchGO();
	GameObject *save1 = FetchGO();
	GameObject *save2 = FetchGO();
	GameObject *save3 = FetchGO();

	//.txt
	LoadingUpgrades upgraded;

	//Options
	float pressDelay = 0.f;

	bool select1, select2, select3, select_delete; //if save files are selected

												   //Cooldown for Button Pressed
	const float cooldownPressed = 0.2f;
};
#endif