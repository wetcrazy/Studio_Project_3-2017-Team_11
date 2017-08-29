#include <sstream>

#include "GL\glew.h"
#include "Application.h"
#include "LoadCSV.h"
#include "QuickSort.h"
#include "SceneManager.h"
#include "SceneHighScore.h"
#include "SceneMainMenu.h"
#include "ClassStorage.h"

SceneHighScore::SceneHighScore()
{
}

SceneHighScore::~SceneHighScore()
{
}

void SceneHighScore::Init()
{
	SceneBase::Init();
	Math::InitRNG();

	//Physics code here
	m_speed = 1.f;
	m_objectCount = 0;
	initialKE = 0;
	finalKE = 0;
	gravity.Set(0.0f, -9.8f, 0.0f);
	file_path = "CSV//highscore.csv";

	CreateStuff();
	highscore = LoadCSV(file_path);
	QuickSort(&highscore, 1, highscore.size() - 1);
	WriteCSV(file_path, highscore);
}

void SceneHighScore::CreateStuff()
{
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float w_temp = 133;
	float h_temp = 100;

	highscoreMenu->type = GameObject::GO_HIGHSCOREMENU;	// Background
	highscoreMenu->active = true;
	highscoreMenu->pos.Set(-10, -10, 1);
	highscoreMenu->scale.Set(1, 1, 1);

	arrows->type = GameObject::GO_ARROW;	// Arrow
	arrows->active = true;
	arrows->pos.Set(-10, -10, 1);
	arrows->scale.Set(1, 1, 1);

}

GameObject* SceneHighScore::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			++m_objectCount;
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
	++m_objectCount;
	return go;
}

void SceneHighScore::Update(double dt)
{
	SceneBase::Update(dt);
	pressDelay += (float)dt;

	// Mouse Variables
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x) / w * m_worldWidth;
	float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

	// Window Variables
	int h_temp = 100;
	int w_temp = 100 * Application::GetWindowWidth() / Application::GetWindowHeight();

	//Position values (for arrows)
	float posXArrow_Delete = 1.35f;
	float posXArrow_Exit = 0.6f;
	float posYArrow = 4.9f;

	//Scale values (for upgrade menu)
	float scaleDown_Arrow = 20.f;

	//Inserting and Deleting Data (Debugging)
	//{	

	//	//Prevent pressDelay from exceeding 0.5f
	//	if (pressDelay > 0.5f)
	//		pressDelay = 0.5f;

	//	if (Application::IsKeyPressed('Z') && pressDelay >= cooldownPressed)
	//	{
	//		highscore = DeleteCSV(file_path);

	//		pressDelay = 0.f;
	//	}

	//	if (Application::IsKeyPressed(VK_SPACE) && pressDelay >= cooldownPressed)
	//	{
	//		//highscore = DeleteCSV(file_path);
	//		HighScore temp;
	//		temp.Data.rank = "4";
	//		temp.Data.name = "GAYY";
	//		temp.Data.score = "1234";
	//		highscore.push_back(temp);
	//		QuickSort(&highscore, 1, highscore.size() - 1);
	//		WriteCSV(file_path, highscore);

	//		pressDelay = 0.f;
	//	}
	//}

	//UP, DOWN and ENTER controls
	{
		//Prevent pressDelay from exceeding 0.5f
		if (pressDelay > 0.5f)
			pressDelay = 0.5f;

		if ((Application::IsKeyPressed(VK_LEFT)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == EXIT_HIGHSCORE)
				selectOptions = DELETE_HIGHSCORE;

			else if (selectOptions == DELETE_HIGHSCORE)
				selectOptions = EXIT_HIGHSCORE;

			pressDelay = 0.f;
		}

		if ((Application::IsKeyPressed(VK_RIGHT)) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == EXIT_HIGHSCORE)
				selectOptions = DELETE_HIGHSCORE;

			else if (selectOptions == DELETE_HIGHSCORE)
				selectOptions = EXIT_HIGHSCORE;

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == DELETE_HIGHSCORE)
				highscore = DeleteCSV(file_path);

			if (selectOptions == EXIT_HIGHSCORE)
				SceneManager::getInstance()->changeScene(new SceneMainMenu());

			pressDelay = 0.f;
		}
	}

	//Arrows
	{
		switch (selectOptions)
		{
		case DELETE_HIGHSCORE:
			//Render in arrow
			arrows->active = true;
			arrows->pos.Set((w_temp / 2) / posXArrow_Delete, (h_temp / 2) / posYArrow, 1);
			arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

			//Render in highscore menu
			highscoreMenu->active = true;
			highscoreMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
			highscoreMenu->scale.Set(w_temp + 2, h_temp, 1);
			break;

		case EXIT_HIGHSCORE:
			//Render in arrow
			arrows->pos.Set((w_temp / 2) / posXArrow_Exit, (h_temp / 2) / posYArrow, 1);
			arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

			//Render in highscore menu
			highscoreMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
			highscoreMenu->scale.Set(w_temp + 2, h_temp, 1);
			break;
		}
	}
}

void SceneHighScore::RenderGO(GameObject *go)
{
	float angle;

	switch (go->type)
	{
	case GameObject::GO_HIGHSCOREMENU:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HIGHSCOREMENU], true, go->Color);
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

void SceneHighScore::Render()
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

	RenderMesh(meshList[GEO_AXES], false);

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text
	float textsize = 3.0f;
	std::ostringstream ss;
	ss << highscore[0];
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), textsize, 18, 43);
	ss.str("");
	// HighScore Table (Prints out only 10)
	if (highscore.size() > 11) // Force set print less than 11
	{
		for (int check_index = 1, sizer = 0; check_index < 11; ++check_index)
		{
			ss << highscore[check_index];
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), textsize, 18, 40 - sizer);
			ss.str("");
			sizer += 3;
		}
	}
	else // Just print (Prints out 10 or less) prevents the framework from crash when there is nothing to print
		for (int check_index = 1, sizer = 0; check_index < highscore.size(); ++check_index)
		{
			ss << highscore[check_index];
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), textsize, 18, 40 - sizer);
			ss.str("");
			sizer += 3;
		}
	// ===================================
}

void SceneHighScore::Exit() // Kills this scene
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}
