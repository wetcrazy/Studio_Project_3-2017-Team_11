#ifndef SCENEUPGRADE_H
#define SCENEUPGRADE_H
#include <string>
#include "Scene.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Camera.h"
#include "SceneBase.h"

class SceneUpgrade : public SceneBase
{
public:
	SceneUpgrade();
	~SceneUpgrade();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	enum MENU
	{
		GRADE_1 = 0,
		RETURN,
	};

private:
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, int sizex, int sizey, int position);

	std::string fps;

	MENU selectOptions = GRADE_1;

	//Options
	float pressDelay = 0.f;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.2f;
};

#endif