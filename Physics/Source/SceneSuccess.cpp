#include "SceneSuccess.h"
#include <sstream>

#include "GL\glew.h"
#include "Application.h"
#include "LoadCSV.h"
#include "LoadTXT.h"
#include "QuickSort.h"

#include "SceneManager.h"
#include "SceneMainMenu.h"
#include "SceneUpgrade.h"

SceneSuccess::SceneSuccess()
{
}

SceneSuccess::~SceneSuccess()
{
}

void SceneSuccess::Init()
{
	SceneBase::Init();

	Math::InitRNG();

	i_score = GetScore();

	i_saveFile = GetSaveFile();

	//Setting star requirements
	//If score is less than i_first, get 1 star
	//If score is between i_first and i_second, get 2 stars
	//If score is more than i_second, get 3 stars
	i_first = 60;
	i_second = 100;

	b_first = false;
	b_second = false;
	b_third = false;

	file_path = "CSV//highscore.csv";

	highscore = LoadCSV(file_path);
	QuickSort(&highscore, 1, highscore.size() - 1);
	WriteCSV(file_path, highscore);

	if (i_saveFile == 1)
	{
		file_ScoreDestination = "Save1//Score.txt";
		file_LevelDestination = "Save1//CurrentLevel.txt";
	}
	else if (i_saveFile == 2)
	{
		file_ScoreDestination = "Save2//Score.txt";
		file_LevelDestination = "Save2//CurrentLevel.txt";
	}
	else if (i_saveFile == 3)
	{
		file_ScoreDestination = "Save3//Score.txt";
		file_LevelDestination = "Save3//CurrentLevel.txt";
	}
	curr_highscore.HighScore_Calculator(file_ScoreDestination, file_LevelDestination);

	arrows->type = GameObject::GO_ARROW;	//Arrow 
	arrows->active = true;
	arrows->pos.Set(-10, -10, 1);
	arrows->scale.Set(1.0f, 1.0f, 1.0f);

	successMenu->type = GameObject::GO_SUCCESSMENU;	//Success Menu
	successMenu->active = true;
	successMenu->pos.Set(-10, -10, -5);
	successMenu->scale.Set(1.0f, 1.0f, 1.0f);

	star1->type = GameObject::GO_STAR;	//Star
	star1->active = true;
	star1->pos.Set(-10, -10, 1);
	star1->scale.Set(1.0f, 1.0f, 1.0f);

	star2->type = GameObject::GO_STAR;	//Star
	star2->active = true;
	star2->pos.Set(-10, -10, 1);
	star2->scale.Set(1.0f, 1.0f, 1.0f);

	star3->type = GameObject::GO_STAR;	//Star
	star3->active = true;
	star3->pos.Set(-10, -10, 1);
	star3->scale.Set(1.0f, 1.0f, 1.0f);
}

GameObject* SceneSuccess::FetchGO()
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

void SceneSuccess::Update(double dt)
{
	SceneBase::Update(dt);

	pressDelay += (float)dt;

	int h_temp = 100;
	int w_temp = 100 * Application::GetWindowWidth() / Application::GetWindowHeight();

	//Position values (for success menu)
	float posXDownArrow_MainMenu = 1.15f;
	float posXDownArrow_Next = 0.59f;
	float posXStar_First = 1.6f;
	float posXStar_Second = 0.987f;
	float posXStar_Third = 0.711f;
	float posYDownArrow = 3.37f;
	float posYStar = 1.0f;

	//Scale values (for success menu)
	float scaleDown_Arrow = 20.f;
	float scaleXDown_Star = 8.9f;
	float scaleYDown_Star = 6.3f;

	//UP, DOWN and ENTER controls
	{
		//Prevent pressDelay from exceeding 0.5f
		if (pressDelay > 0.5f)
			pressDelay = 0.5f;

		if ((Application::IsKeyPressed(VK_LEFT)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == MAIN_MENU)
				selectOptions = NEXT;

			else if (selectOptions == NEXT)
				selectOptions = MAIN_MENU;

			pressDelay = 0.f;
		}

		if ((Application::IsKeyPressed(VK_RIGHT)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == MAIN_MENU)
				selectOptions = NEXT;

			else if (selectOptions == NEXT)
				selectOptions = MAIN_MENU;

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == NEXT)
				SceneManager::getInstance()->changeScene(new SceneUpgrade());

			else if (selectOptions == MAIN_MENU)
				SceneManager::getInstance()->changeScene(new SceneMainMenu());

			pressDelay = 0.f;
		}
	}

	switch (selectOptions)
	{
	case NEXT:
		//Render in arrow 
		arrows->active = true;
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Next, (h_temp / 2) / posYDownArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in success menu
		successMenu->active = true;
		successMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		successMenu->scale.Set(w_temp + 2, h_temp, 1);

		//Render in first star
		if (i_score <= i_first)
			b_first = true;
		//Render in first and second star
		if (i_score > i_first && i_score <= i_second)
		{
			b_first = true;
			b_second = true;
		}
		//Render in first, second and third star
		if (i_score > i_second)
		{
			b_first = true;
			b_second = true;
			b_third = true;
		}

		break;

	case MAIN_MENU:
		//Render in arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_MainMenu, (h_temp / 2) / posYDownArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in success menu
		successMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		successMenu->scale.Set(w_temp + 2, h_temp, 1);

		//Render in first star
		if (i_score <= i_first)
			b_first = true;
		//Render in first and second star
		if (i_score > i_first && i_score <= i_second)
		{
			b_first = true;
			b_second = true;
		}
		//Render in first, second and third star
		if (i_score > i_second)
		{
			b_first = true;
			b_second = true;
			b_third = true;
		}

		break;
	}

	//Obtained first star
	if (b_first)
	{
		star1->active = true;
		star1->pos.Set((w_temp / 2) / posXStar_First, (h_temp / 2) / posYStar, 1);
		star1->scale.Set((w_temp + 2) / scaleXDown_Star, h_temp / scaleYDown_Star, 1);
	}
	//Obtained second star
	if (b_second)
	{
		star2->active = true;
		star2->pos.Set((w_temp / 2) / posXStar_Second, (h_temp / 2) / posYStar, 1);
		star2->scale.Set((w_temp + 2) / scaleXDown_Star, h_temp / scaleYDown_Star, 1);
	}
	//Obtained third star
	if (b_third)
	{
		star3->active = true;
		star3->pos.Set((w_temp / 2) / posXStar_Third, (h_temp / 2) / posYStar, 1);
		star3->scale.Set((w_temp + 2) / scaleXDown_Star, h_temp / scaleYDown_Star, 1);
	}
}

int SceneSuccess::GetScore()
{
	int score;
	ifstream myFile;
	if (i_saveFile == 1)
		myFile.open("Save1//Score.txt");
	else if (i_saveFile == 2)
		myFile.open("Save2//Score.txt");
	else if (i_saveFile == 3)
		myFile.open("Save3//Score.txt");
	myFile >> score;
	myFile.close();

	return score;
}

int SceneSuccess::GetSaveFile()
{
	int saveFile;
	ifstream myFile;
	myFile.open("savefile.txt");
	myFile >> saveFile;
	myFile.close();

	return saveFile;
}

void SceneSuccess::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_SUCCESSMENU:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SUCCESSMENU], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARROW:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ARROW], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_STAR:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_STAR], true, go->Color);
		modelStack.PopMatrix();
		break;
	}
}

void SceneSuccess::Render()
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

	ss.str(std::string());
	ss.precision(5);
	ss << i_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 37, 40);

	ss.str(std::string());
	ss.precision(5);
	ss << curr_highscore.Data.score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 37, 17.5);
}

void SceneSuccess::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}