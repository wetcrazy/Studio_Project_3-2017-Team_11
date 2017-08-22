#include "SceneUpgrade.h"
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
#include "SceneMainMenu.h"
#include "SceneCollision.h"

SceneUpgrade::SceneUpgrade()
{
}

SceneUpgrade::~SceneUpgrade()
{
}

void SceneUpgrade::Init()
{
	SceneBase::Init();

	Math::InitRNG();


	b_speed_upgrade_1 = false;
	b_speed_upgrade_2 = false;

	arrows->type = GameObject::GO_ARROW;	//Arrow 
	arrows->active = true;
	arrows->pos.Set(-10, -10, 1);
	arrows->scale.Set(1.0f, 1.0f, 1.0f);

	upgradesMenu->type = GameObject::GO_UPGRADESMENU;	//Upgrades Menu
	upgradesMenu->active = true;
	upgradesMenu->pos.Set(-10, -10, -5);
	upgradesMenu->scale.Set(1.0f, 1.0f, 1.0f);

	speed_upgrade_1->type = GameObject::GO_SPEED_UPGRADE_1;	//Speed_Upgrade_1
	speed_upgrade_1->active = true;
	speed_upgrade_1->pos.Set(-10, -10, -5);
	speed_upgrade_1->scale.Set(1.0f, 1.0f, 1.0f);

	speed_upgrade_2->type = GameObject::GO_SPEED_UPGRADE_2;	//Speed_Upgrade_2
	speed_upgrade_2->active = true;
	speed_upgrade_2->pos.Set(-10, -10, -5);
	speed_upgrade_2->scale.Set(1.0f, 1.0f, 1.0f);
}

GameObject* SceneUpgrade::FetchGO()
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

void SceneUpgrade::Update(double dt)
{
	SceneBase::Update(dt);

	upgraded.ReadFile("Text//Speed_Upgrade.txt");

	pressDelay += (float)dt;
	int h_temp = 100;
	int w_temp = 100 * Application::GetWindowWidth() / Application::GetWindowHeight();

	//Position values (for upgrade menu)
	float posXDownArrow = 0.7f;
	float posXDownArrow_NextLevel = 1.1f;
	float posXDownArrow_NextPage = 0.57f;
	float posYDownArrow_Upgrade1 = 0.82f;
	float posYDownArrow_Upgrade2 = 1.33f;
	float posYDownArrow_NextPage_NextLevel = 3.35f;

	//Scale values (for upgrade menu)
	float scaleDown_Arrow = 20.f;

	//Press R to reset
	if (Application::IsKeyPressed('R'))
	{
		SceneManager::getInstance()->changeScene(new SceneMainMenu());
		SetCurrentLevel(1);
		SetTempScore(0);
		SetScore(0);
		upgraded.ResetFile("Text//Speed_Upgrade.txt", "");
		upgraded.ReadFile("Text//Speed_Upgrade.txt");
	}

	//UP, DOWN and ENTER controls
	{
		//Prevent pressDelay from exceeding 0.5f
		if (pressDelay > 0.5f)
			pressDelay = 0.5f;

		if ((Application::IsKeyPressed(VK_UP)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == NEXTPAGE)
				selectOptions = NEXTLEVEL;

			else if (selectOptions == NEXTLEVEL)
				selectOptions = SPEED_UPGRADE_2;

			else if (selectOptions == SPEED_UPGRADE_2)
				selectOptions = SPEED_UPGRADE_1;

			else if (selectOptions == SPEED_UPGRADE_1)
				selectOptions = NEXTPAGE;

			pressDelay = 0.f;
		}

		if ((Application::IsKeyPressed(VK_DOWN)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == SPEED_UPGRADE_1)
				selectOptions = SPEED_UPGRADE_2;

			else if (selectOptions == SPEED_UPGRADE_2)
				selectOptions = NEXTLEVEL;

			else if (selectOptions == NEXTLEVEL)
				selectOptions = NEXTPAGE;

			else if (selectOptions == NEXTPAGE)
				selectOptions = SPEED_UPGRADE_1;

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == NEXTLEVEL)
				SceneManager::getInstance()->changeScene(new SceneCollision());

			else if (selectOptions == NEXTPAGE)
				SceneManager::getInstance()->changeScene(new SceneCollision());

			else if (selectOptions == SPEED_UPGRADE_1 && upgraded.speed_upgrade != 1 && upgraded.speed_upgrade != 2)
				b_speed_upgrade_1 = true;

			else if (selectOptions == SPEED_UPGRADE_2 && upgraded.speed_upgrade == 1 && upgraded.speed_upgrade != 2)
				b_speed_upgrade_2 = true;

			pressDelay = 0.f;
		}
	}

	switch (selectOptions)
	{
	case SPEED_UPGRADE_1:
		//Render in arrow 
		arrows->active = true;
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Upgrade1, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenu->active = true;
		upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case SPEED_UPGRADE_2:
		//Render in arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Upgrade2, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case NEXTLEVEL:
		//Render in arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_NextLevel, (h_temp / 2) / posYDownArrow_NextPage_NextLevel, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case NEXTPAGE:
		//Render in arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_NextPage, (h_temp / 2) / posYDownArrow_NextPage_NextLevel, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;
	}

	//Buying first upgrade
	if (b_speed_upgrade_1)
	{
		//Render in upgrade1
		upgradesMenu->active = false;
		speed_upgrade_1->active = true;
		speed_upgrade_1->pos.Set(w_temp / 2, h_temp / 2, -5);
		speed_upgrade_1->scale.Set(w_temp + 2, h_temp, 1);

		//.txt
		upgraded.WriteFile("Text//Speed_Upgrade.txt", "Speed", 1);
	}
	//Buying second upgrade
	if (b_speed_upgrade_2)
	{
		//Render in upgrade2
		speed_upgrade_1->active = false;
		speed_upgrade_2->active = true;
		speed_upgrade_2->pos.Set(w_temp / 2, h_temp / 2, -5);
		speed_upgrade_2->scale.Set(w_temp + 2, h_temp, 1);

		//.txt
		upgraded.WriteFile("Text//Speed_Upgrade.txt", "Speed", 2);
	}

	//Bought first upgrade
	if (upgraded.speed_upgrade == 1)
	{
		//Render in upgrade1
		upgradesMenu->active = false;
		speed_upgrade_2->active = false;
		speed_upgrade_1->active = true;
		speed_upgrade_1->pos.Set(w_temp / 2, h_temp / 2, -5);
		speed_upgrade_1->scale.Set(w_temp + 2, h_temp, 1);
	}

	//Bought second upgrade
	if (upgraded.speed_upgrade == 2)
	{
		//Render in upgrade2
		upgradesMenu->active = false;
		speed_upgrade_1->active = false;
		speed_upgrade_2->active = true;
		speed_upgrade_2->pos.Set(w_temp / 2, h_temp / 2, -5);
		speed_upgrade_2->scale.Set(w_temp + 2, h_temp, 1);
	}
}

void SceneUpgrade::SetCurrentLevel(int levelNo)
{
	ofstream myFile;
	myFile.open("Text//CurrentLevel.txt");
	myFile << levelNo << endl;
	myFile.close();
}

void SceneUpgrade::SetScore(int score)
{
	ofstream myFile;
	myFile.open("Text//Score.txt");
	myFile << score << endl;
	myFile.close();
}

void SceneUpgrade::SetTempScore(int tempScore)
{
	ofstream myFile;
	myFile.open("Text//TempScore.txt");
	myFile << tempScore << endl;
	myFile.close();
}

void SceneUpgrade::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_UPGRADESMENU:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_UPGRADESMENU], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARROW:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ARROW], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SPEED_UPGRADE_1:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SPEED_UPGRADE_1], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SPEED_UPGRADE_2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SPEED_UPGRADE_2], true, go->Color);
		modelStack.PopMatrix();
		break;
	}
}

void SceneUpgrade::Render()
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

void SceneUpgrade::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneUpgrade::RenderMeshOnScreen(Mesh* mesh, float x, float y, int sizex, int sizey, int position)
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

void SceneUpgrade::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}