#ifndef SceneCredit_H
#define SceneCredit_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneCredit : public SceneBase
{
public:
	SceneCredit();
	~SceneCredit();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	//virtual void RenderMenu();
	virtual void Exit();

	void RenderGO(GameObject *go);

	enum MENU
	{
		BACK = 0
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
	MENU selectOptions = BACK;

	GameObject* FetchGO();


	//MainMenu
	GameObject *arrows = FetchGO();
	GameObject *credits = FetchGO();

	//Options
	float pressDelay = 0.f;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.2f;
};

#endif 