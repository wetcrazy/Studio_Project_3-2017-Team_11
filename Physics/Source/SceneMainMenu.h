#ifndef SceneMainMenu_H
#define SceneMainMenu_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneMainMenu : public SceneBase
{
public:
	SceneMainMenu();
	~SceneMainMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	//virtual void RenderMenu();
	virtual void Exit();

	void RenderGO(GameObject *go);

	enum MENU
	{
		NEWGAME = 0,
		LEVELS,
		INSTRUCTIONS,
		HIGHSCORE,
		OPTIONS,
		QUIT,
	};

protected:
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, int sizex, int sizey, int position);

	//Physics
	std::vector<GameObject *> m_goList;

	std::string fps;
	float m_worldWidth;
	float m_worldHeight;

	//Default optoin
	MENU selectOptions = NEWGAME;

	GameObject* FetchGO();


	//MainMenu
	GameObject *arrows = FetchGO();
	GameObject *menu = FetchGO();

	//Options
	float pressDelay = 0.f;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.2f;
};

#endif 