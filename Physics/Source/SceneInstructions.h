#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneInstructions : public SceneBase
{
public:
	SceneInstructions();
	~SceneInstructions();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	enum INSTRUCTIONS_MENU
	{
		MAIN_MENU,
		NEXT_1,
		NEXT_2,
		PREVIOUS_1,
		PREVIOUS_2
	};

protected:
	//Physics
	std::vector<GameObject *> m_goList;

	std::string fps;
	float m_worldWidth;
	float m_worldHeight;

	//Default optoin
	INSTRUCTIONS_MENU selectOptions = MAIN_MENU;

	GameObject* FetchGO();

	//Insructions Menu
	GameObject *arrows = FetchGO();
	GameObject *pg1 = FetchGO();
	GameObject *pg2 = FetchGO();
	GameObject *pg3 = FetchGO();

	//Next page
	bool next_page1;
	bool next_page2;
	//Previous page
	bool previous_page1;
	bool previous_page2;

	//Options
	float pressDelay = 0.f;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.2f;
};
#endif