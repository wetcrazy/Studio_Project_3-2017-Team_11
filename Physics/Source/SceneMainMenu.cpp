#include "SceneMainMenu.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Mtx44.h"
#include "Application.h"
#include "Vertex.h"
#include "Utility.h"
#include "shader.hpp"
#include "LoadTGA.h"
#include "Camera.h"
#include "MeshBuilder.h"
#include "Mesh.h"

#include "SceneManager.h"
#include "SceneCollision.h"
#include "SceneHighScore.h"
#include "SceneSaveFile.h"

#include "../IK/irrKlang.h"
using namespace irrklang;
#pragma comment(lib,"irrKlang.lib")
ISoundEngine*Menu = createIrrKlangDevice();

SceneMainMenu::SceneMainMenu()
{
}

SceneMainMenu::~SceneMainMenu()
{
}

void SceneMainMenu::Init()
{
	SceneBase::Init();

	Math::InitRNG();

	arrows->type = GameObject::GO_ARROW;	//Arrow 
	arrows->active = true;
	arrows->pos.Set(-10, -10, 1);
	arrows->scale.Set(1.0f, 1.0f, 1.0f);

	menu->type = GameObject::GO_MAINMENU;	//Main Menu
	menu->active = true;
	menu->pos.Set(-10, -10, -5);
	menu->scale.Set(1.0f, 1.0f, 1.0f);

	Menu->play2D("PocketMorty.mp3", GL_TRUE);
}

GameObject* SceneMainMenu::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 40; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}
	for (unsigned i = 0; i < 40; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_CUBE);
		m_goList.push_back(go);
	}
	GameObject *go = m_goList.back();
	go->active = true;
	return go;
}

void SceneMainMenu::Update(double dt)
{
	SceneBase::Update(dt);

	pressDelay += (float)dt;
	int h_temp = 100;
	int w_temp = 100 * Application::GetWindowWidth() / Application::GetWindowHeight();

	//Position values (arrows)
	float posXDownArrow = 0.67f;
	float posYDownArrow_Play = 0.69f;
	float posYDownArrow_Levels = 0.84f;
	float posYDownArrow_Instructions = 1.07f;
	float posYDownArrow_Highscore = 1.45f;
	float posYDownArrow_Options = 2.25f;
	float posYDownArrow_Quit = 5.1f;

	//Scale values
	float scaleDown_Arrow = 20.f;
	float scaleXDown_Highlight = 2.4f;
	float scaleYDown_Highlight = 17.f;

	//UP, DOWN and ENTER controls
	{
		//Prevent pressDelay from exceeding 0.5f
		if (pressDelay > 0.5f)
			pressDelay = 0.5f;

		if ((Application::IsKeyPressed(VK_UP)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == QUIT)
				selectOptions = OPTIONS;

			else if (selectOptions == OPTIONS)
				selectOptions = HIGHSCORE;

			else if (selectOptions == HIGHSCORE)
				selectOptions = INSTRUCTIONS;

			else if (selectOptions == INSTRUCTIONS)
				selectOptions = LEVELS;

			else if (selectOptions == LEVELS)
				selectOptions = NEWGAME;

			else if (selectOptions == NEWGAME)
				selectOptions = QUIT;

			pressDelay = 0.f;
		}

		if ((Application::IsKeyPressed(VK_DOWN)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == QUIT)
				selectOptions = NEWGAME;

			else if (selectOptions == NEWGAME)
				selectOptions = LEVELS;

			else if (selectOptions == LEVELS)
				selectOptions = INSTRUCTIONS;

			else if (selectOptions == INSTRUCTIONS)
				selectOptions = HIGHSCORE;

			else if (selectOptions == HIGHSCORE)
				selectOptions = OPTIONS;

			else if (selectOptions == OPTIONS)
				selectOptions = QUIT;

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == NEWGAME)
				SceneManager::getInstance()->changeScene(new SceneSaveFile());

			else if (selectOptions == HIGHSCORE)
			SceneManager::getInstance()->changeScene(new SceneHighScore());

			else if (selectOptions == QUIT)
				exit(0);

			pressDelay = 0.f;
		}
	}

	switch (selectOptions)
	{
	case NEWGAME:
		//Arrow
		arrows->active = true;
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Play, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Menu
		menu->active = true;
		menu->pos.Set(w_temp / 2, h_temp / 2, -5);
		menu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case LEVELS:
		//Arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Levels, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Menu
		menu->pos.Set(w_temp / 2, h_temp / 2, -5);
		menu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case INSTRUCTIONS:
		//Arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Instructions, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Menu
		menu->pos.Set(w_temp / 2, h_temp / 2, -5);
		menu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case HIGHSCORE:
		//Arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Highscore, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Menu
		menu->pos.Set(w_temp / 2, h_temp / 2, -5);
		menu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case OPTIONS:
		//Arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Options, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Menu
		menu->pos.Set(w_temp / 2, h_temp / 2, -5);
		menu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case QUIT:
		//Arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Quit, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Menu
		menu->pos.Set(w_temp / 2, h_temp / 2, -5);
		menu->scale.Set(w_temp + 2, h_temp, 1);
		break;
	}
}

void SceneMainMenu::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_MAINMENU:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_MAINMENU], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARROW:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ARROW], true, go->Color);
		modelStack.PopMatrix();
		break;
	}
}

void SceneMainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	//RenderMesh(meshList[GEO_AXES], false);

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}
}

void SceneMainMenu::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneMainMenu::RenderMeshOnScreen(Mesh* mesh, float x, float y, int sizex, int sizey, int position)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	//to do: scale and translate accordingly
	modelStack.Scale(sizex, sizey, position);
	modelStack.Translate(x + 0.5f, y + 0.5f, 0);

	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void SceneMainMenu::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

	Menu->stopAllSounds();

}