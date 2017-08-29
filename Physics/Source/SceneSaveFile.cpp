#include "SceneSaveFile.h"

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

#include <sstream>

#include "SceneManager.h"
#include "SceneMainMenu.h"
#include "SceneCollision.h"

SceneSaveFile::SceneSaveFile()
{
}

SceneSaveFile::~SceneSaveFile()
{
}

void SceneSaveFile::Init()
{
	SceneBase::Init();

	Math::InitRNG();

	arrows->type = GameObject::GO_ARROW;	//Arrow 
	arrows->active = true;
	arrows->pos.Set(-10, -10, 1);
	arrows->scale.Set(1.0f, 1.0f, 1.0f);

	saveMenu->type = GameObject::GO_SAVEMENU;	//Save Menu
	saveMenu->active = true;
	saveMenu->pos.Set(-10, -10, -5);
	saveMenu->scale.Set(1.0f, 1.0f, 1.0f);

	save1->type = GameObject::GO_SAVE1;	//Save 1
	save1->active = true;
	save1->pos.Set(-10, -10, -5);
	save1->scale.Set(1.0f, 1.0f, 1.0f);

	save2->type = GameObject::GO_SAVE2;	//Save 2 
	save2->active = true;
	save2->pos.Set(-10, -10, -5);
	save2->scale.Set(1.0f, 1.0f, 1.0f);

	save3->type = GameObject::GO_SAVE3;	//Save 3
	save3->active = true;
	save3->pos.Set(-10, -10, -5);
	save3->scale.Set(1.0f, 1.0f, 1.0f);

	select1 = false;
	select2 = false;
	select3 = false;
	select_delete = false;
}

GameObject* SceneSaveFile::FetchGO()
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

void SceneSaveFile::Update(double dt)
{
	SceneBase::Update(dt);

	pressDelay += (float)dt;

	int h_temp = 100;
	int w_temp = 100 * Application::GetWindowWidth() / Application::GetWindowHeight();

	//Position values (for save file)
	float posXDownArrow_Saves = 0.72f;
	float posXDownArrow_Select = 1.55f;
	float posXDownArrow_Delete = 0.9f;
	float posXDownArrow_MainMenu = 0.57f;
	float posYDownArrow_Save1 = 0.765f;
	float posYDownArrow_Save2 = 1.1f;
	float posYDownArrow_Save3 = 1.89f;
	float posYDownArrow_Options = 5.f;

	//Scale values (for save file)
	float scaleDown_Arrow = 20.f;

	//UP, DOWN and ENTER controls
	{
		//Prevent pressDelay from exceeding 0.5f
		if (pressDelay > 0.5f)
			pressDelay = 0.5f;

		if ((Application::IsKeyPressed(VK_UP)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == SAVE1)
				selectOptions = MAIN_MENU;

			else if (selectOptions == MAIN_MENU)
				selectOptions = DELETE_FILE;

			else if (selectOptions == DELETE_FILE)
				selectOptions = SELECT;

			else if (selectOptions == SELECT)
				selectOptions = SAVE3;

			else if (selectOptions == SAVE3)
				selectOptions = SAVE2;

			else if (selectOptions == SAVE2)
				selectOptions = SAVE1;

			pressDelay = 0.f;
		}

		if ((Application::IsKeyPressed(VK_DOWN)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == SAVE1)
				selectOptions = SAVE2;

			else if (selectOptions == SAVE2)
				selectOptions = SAVE3;

			else if (selectOptions == SAVE3)
				selectOptions = SELECT;

			else if (selectOptions == SELECT)
				selectOptions = DELETE_FILE;

			else if (selectOptions == DELETE_FILE)
				selectOptions = MAIN_MENU;

			else if (selectOptions == MAIN_MENU)
				selectOptions = SAVE1;

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == SAVE1)
			{
				select1 = true;
				select2 = false;
				select3 = false;
				select_delete = false;
			}
				
			else if (selectOptions == SAVE2)
			{
				select2 = true;
				select1 = false;
				select3 = false;
				select_delete = false;
			}
				
			else if (selectOptions == SAVE3)
			{
				select3 = true;
				select1 = false;
				select2 = false;
				select_delete = false;
			}				

			if (selectOptions == SELECT)
			{
				if (select1)
				{
					SetSaveFile(1);
					SceneManager::getInstance()->changeScene(new SceneCollision());
				}
				else if (select2)
				{
					SetSaveFile(2);
					SceneManager::getInstance()->changeScene(new SceneCollision());
				}
				else if (select3)
				{
					SetSaveFile(3);
					SceneManager::getInstance()->changeScene(new SceneCollision());
				}
			}

			else if (selectOptions == DELETE_FILE)
			{
				select_delete = true;
				DeleteSaveFile("savefile.txt");

				SetCurrentLevel(1);
				SetTempScore(0);
				SetTotalScore(0);
				SetCurrency(0);
				SetTempCurrency(0);
				if (select1)
				{
					upgraded.DeleteTextFile("Save1//Speed_Upgrade.txt");
					upgraded.ReadFile("Save1//Speed_Upgrade.txt");
				}
				else if (select2)
				{
					upgraded.DeleteTextFile("Save2//Speed_Upgrade.txt");
					upgraded.ReadFile("Save2//Speed_Upgrade.txt");
				}
				else if (select3)
				{
					upgraded.DeleteTextFile("Save3//Speed_Upgrade.txt");
					upgraded.ReadFile("Save3//Speed_Upgrade.txt");
				}

			}

			else if (selectOptions == MAIN_MENU)
				SceneManager::getInstance()->changeScene(new SceneMainMenu());

			pressDelay = 0.f;
		}
	}

	switch (selectOptions)
	{
	case SAVE1:
		//Render arrow 
		arrows->active = true;
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Saves, (h_temp / 2) / posYDownArrow_Save1, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render save menu
		saveMenu->active = true;
		saveMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		saveMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case SAVE2:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Saves, (h_temp / 2) / posYDownArrow_Save2, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render save menu
		saveMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		saveMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case SAVE3:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Saves, (h_temp / 2) / posYDownArrow_Save3, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render save menu
		saveMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		saveMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case SELECT:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Select, (h_temp / 2) / posYDownArrow_Options, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render save menu
		saveMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		saveMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case DELETE_FILE:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Delete, (h_temp / 2) / posYDownArrow_Options, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render save menu
		saveMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		saveMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case MAIN_MENU:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_MainMenu, (h_temp / 2) / posYDownArrow_Options, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render save menu
		saveMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		saveMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;
	}

	//If First Save File selected
	if (select1)
	{
		//False actives
		saveMenu->active = false;
		save2->active = false;
		save3->active = false;

		//Render in save1
		save1->active = true;
		save1->pos.Set(w_temp / 2, h_temp / 2, -5);
		save1->scale.Set(w_temp + 2, h_temp, 1);
	}
	//If Second Save File selected
	if (select2)
	{
		//False actives
		saveMenu->active = false;
		save1->active = false;
		save3->active = false;

		//Render in save2
		save2->active = true;
		save2->pos.Set(w_temp / 2, h_temp / 2, -5);
		save2->scale.Set(w_temp + 2, h_temp, 1);
	}
	//If Third Save File selected
	if (select3)
	{
		//False actives
		saveMenu->active = false;
		save1->active = false;
		save2->active = false;

		//Render in save3
		save3->active = true;
		save3->pos.Set(w_temp / 2, h_temp / 2, -5);
		save3->scale.Set(w_temp + 2, h_temp, 1);
	}
	//If deleted save file
	if (select_delete)
	{
		//False actives
		save1->active = false;
		save2->active = false;
		save3->active = false;

		//Render in saveMenu
		saveMenu->active = true;
		saveMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		saveMenu->scale.Set(w_temp + 2, h_temp, 1);
	}
}

void SceneSaveFile::SetSaveFile(int selected_file)
{
	ofstream myFile;
	myFile.open("savefile.txt");
	myFile << selected_file << endl;
	myFile.close();
}

void SceneSaveFile::SetCurrentLevel(int levelNo)
{
	ofstream myFile;
	if (select1)
		myFile.open("Save1//CurrentLevel.txt");
	else if (select2)
		myFile.open("Save2//CurrentLevel.txt");
	else if (select3)
		myFile.open("Save3//CurrentLevel.txt");
	myFile << levelNo << endl;
	myFile.close();
}

void SceneSaveFile::SetTotalScore(int score)
{
	ofstream myFile;
	if (select1)
		myFile.open("Save1//TotalScore.txt");
	else if (select2)
		myFile.open("Save2//TotalScore.txt");
	else if (select3)
		myFile.open("Save3//TotalScore.txt");
	myFile << score << endl;
	myFile.close();
}

void SceneSaveFile::SetTempScore(int tempScore)
{
	ofstream myFile;
	if (select1)
		myFile.open("Save1//TempScore.txt");
	else if (select2)
		myFile.open("Save2//TempScore.txt");
	else if (select3)
		myFile.open("Save3//TempScore.txt");
	myFile << tempScore << endl;
	myFile.close();
}

void SceneSaveFile::SetCurrency(int currency)
{
	ofstream myFile;
	if (select1)
		myFile.open("Save1//Currency.txt");
	else if (select2)
		myFile.open("Save2//Currency.txt");
	else if (select3)
		myFile.open("Save3//Currency.txt");
	myFile << currency << endl;
	myFile.close();
}

void SceneSaveFile::SetTempCurrency(int tempCurrency)
{
	ofstream myFile;
	if (select1)
		myFile.open("Save1//TempCurrency.txt");
	else if (select2)
		myFile.open("Save2//TempCurrency.txt");
	else if (select3)
		myFile.open("Save3//TempCurrency.txt");
	myFile << tempCurrency << endl;
	myFile.close();
}

int SceneSaveFile::GetCurrentLevel(string fileName)
{
	int level;
	ifstream myFile;
	myFile.open(fileName);
	myFile >> level;
	myFile.close();

	return level;
}

void SceneSaveFile::DeleteSaveFile(string fileName)
{
	ofstream output;
	output.open(fileName);

	//Replace text with blank space
	output << endl;

	output.close();
}

void SceneSaveFile::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_ARROW:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ARROW], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SAVEMENU:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SAVEMENU], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SAVE1:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SAVE1], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SAVE2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SAVE2], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SAVE3:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SAVE3], true, go->Color);
		modelStack.PopMatrix();
		break;
	}
}

void SceneSaveFile::Render()
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

	std::ostringstream ss;

	//Show current level of save file 1
	int i_currentLevel_1 = GetCurrentLevel("Save1//CurrentLevel.txt");
	ss.str(std::string());
	ss.precision(5);
	if (i_currentLevel_1 == 10)
	{
		ss << "max" << endl;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2.3f, 45, 36.7);
	}
	else
	{
		ss << i_currentLevel_1;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 45, 36.5);
	}

	//Show current level of save file 2
	int i_currentLevel_2 = GetCurrentLevel("Save2//CurrentLevel.txt");
	ss.str(std::string());
	ss.precision(5);
	if (i_currentLevel_2 == 10)
	{
		ss << "max" << endl;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2.3f, 45, 24.7);
	}
	else
	{
		ss << i_currentLevel_2;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 45, 24.5);
	}

	//Show current level of save file 3
	int i_currentLevel_3 = GetCurrentLevel("Save3//CurrentLevel.txt");
	ss.str(std::string());
	ss.precision(5);
	if (i_currentLevel_3 == 10)
	{
		ss << "max" << endl;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2.3f, 45, 12.7);
	}
	else
	{
		ss << i_currentLevel_3;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 45, 12.5);
	}
}

void SceneSaveFile::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
