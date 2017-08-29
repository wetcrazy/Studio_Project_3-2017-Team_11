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
		MAIN_MENU_PAGE1,
		NEXT_PAGE1,
		PREVIOUS_PAGE2,
		NEXT_PAGE2,
		PREVIOUS_PAGE3,
		NEXT_PAGE3,
		PREVIOUS_PAGE4,
		MAIN_MENU_PAGE4
	};

protected:
	//Physics
	std::vector<GameObject *> m_goList;

	std::string fps;
	float m_worldWidth;
	float m_worldHeight;

	//Default optoin
	INSTRUCTIONS_MENU selectOptions = MAIN_MENU_PAGE1;

	GameObject* FetchGO();

	//Insructions Menu
	GameObject *arrows = FetchGO();
	GameObject *pg1 = FetchGO();
	GameObject *pg2 = FetchGO();
	GameObject *pg3 = FetchGO();
	GameObject *pg4 = FetchGO();

	//Next page
	bool b_page1, b_page2, b_page3, b_page4;

	//Options
	float pressDelay = 0.f;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.2f;
};
#endif