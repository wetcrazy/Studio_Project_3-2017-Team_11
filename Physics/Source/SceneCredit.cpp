#include "SceneCredit.h"
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

#include "SceneMainMenu.h"

#include "../IK/irrKlang.h"
using namespace irrklang;
#pragma comment(lib,"irrKlang.lib")
ISoundEngine*Credit = createIrrKlangDevice();

SceneCredit::SceneCredit()
{
}

SceneCredit::~SceneCredit()
{
}

void SceneCredit::Init()
{
	SceneBase::Init();

	Math::InitRNG();

	arrows->type = GameObject::GO_ARROW;	//Arrow 
	arrows->active = true;
	arrows->pos.Set(-10, -10, 1);
	arrows->scale.Set(1.0f, 1.0f, 1.0f);

	credits->type = GameObject::GO_CREDITS;	//Main Menu
	credits->active = true;
	credits->pos.Set(-10, -10, -5);
	credits->scale.Set(1.0f, 1.0f, 1.0f);

	Credit->play2D("PocketMorty.mp3", GL_TRUE);
}

GameObject* SceneCredit::FetchGO()
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

void SceneCredit::Update(double dt)
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
			if (selectOptions == BACK)
				selectOptions = BACK;

			pressDelay = 0.f;
		}

		if ((Application::IsKeyPressed(VK_DOWN)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == BACK)
				selectOptions = BACK;

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == BACK)
				SceneManager::getInstance()->changeScene(new SceneMainMenu());
			else
				exit(0);

			pressDelay = 0.f;
		}
	}

	switch (selectOptions)
	{
	case BACK:
		//Arrow
		arrows->active = true;
		arrows->pos.Set((w_temp / 4.5) / posXDownArrow, (h_temp / 9.5) / posYDownArrow_Play, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Credit
		credits->active = true;
		credits->pos.Set(w_temp / 2, h_temp / 2, -5);
		credits->scale.Set(w_temp + 2, h_temp, 1);
		break;
	}
}

void SceneCredit::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_CREDITS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CREDITS], true, go->Color);
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

void SceneCredit::Render()
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

void SceneCredit::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneCredit::RenderMeshOnScreen(Mesh* mesh, float x, float y, int sizex, int sizey, int position)
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

void SceneCredit::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

	Credit->stopAllSounds();

}