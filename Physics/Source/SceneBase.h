#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include <vector>

class SceneBase : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_BALL,
		GEO_CUBE,
		GEO_ASTEROID,
		GEO_SHIP,

		GEO_BACKGROUND,
		GEO_GROUND,
		GEO_PLATFORM, 

		GEO_CANNON,
		GEO_CANNON_PLATFORM,

		// Arrow for menus
		GEO_ARROW,

		// Main Menu
		GEO_MAINMENU,

		// Upgrades Menu
		GEO_UPGRADESMENU,
		GEO_SPEED_UPGRADE_1,
		GEO_SPEED_UPGRADE_2,

		// HighScore Menu
		GEO_HIGHSCOREMENU,

		NUM_GEOMETRY,
	};
	enum UPGRADES_MENU
	{
		GRADE_1 = 0,
		GRADE_2,
		NEXTLEVEL,
		NEXTPAGE,
	};
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMesh(Mesh *mesh, bool enableLight, Vector3 color);
	void RenderMeshOnScreen(Mesh *mesh, float x, float y, float sizex, float sizey, float rotate);
	void RenderGO(GameObject *go);

	GameObject* FetchGO();
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;

	//Default option
	UPGRADES_MENU selectOptions = GRADE_1;

	float fps;
};

#endif