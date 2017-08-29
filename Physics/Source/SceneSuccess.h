#ifndef SCENESUCCESS_H
#define SCENESUCCESS_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "SceneUpgrade.h"
#include "ClassStorage.h"

class SceneSuccess : public SceneBase
{
public:
	SceneSuccess();
	~SceneSuccess();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	//virtual void RenderMenu();
	virtual void Exit();

	void RenderGO(GameObject *go);

	//Getter for score
	int GetScore();

	//Getter for save file
	int GetSaveFile();

	enum SUCCESS_MENU
	{
		MAIN_MENU = 0,
		NEXT
	};

protected:
	//Physics
	std::vector<GameObject *> m_goList;

	std::string fps;
	float m_worldWidth;
	float m_worldHeight;

	//Default optoin
	SUCCESS_MENU selectOptions = MAIN_MENU;

	GameObject* FetchGO();

	//UpgradesMenu
	GameObject *arrows = FetchGO();
	GameObject *successMenu = FetchGO();
	GameObject *star1 = FetchGO();
	GameObject *star2 = FetchGO();
	GameObject *star3 = FetchGO();	
	
	//Success Stuff
	std::vector<HighScore> highscore;
	const char *file_path;

	//Highscore
	HighScore curr_highscore;
	const char *file_ScoreDestination, *file_LevelDestination;

	//Score
	int i_score;
	int i_first, i_second;

	//Stars
	bool b_first, b_second, b_third;

	//savefile.txt
	int i_saveFile;

	//Options
	float pressDelay = 0.f;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.2f;
};

#endif 