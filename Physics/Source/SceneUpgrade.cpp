#include "SceneUpgrade.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"

#include <sstream>

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

	b_next_page = false;
	b_previous_page = true;

	b_speed_upgrade_1 = false;
	b_speed_upgrade_2 = false;
	b_speed_multiplyer_upgrade_1 = false;
	b_speed_multiplyer_upgrade_2 = false;

	arrows->type = GameObject::GO_ARROW;	//Arrow 
	arrows->active = true;
	arrows->pos.Set(-10, -10, 1);
	arrows->scale.Set(1.0f, 1.0f, 1.0f);

	upgradesMenu->type = GameObject::GO_UPGRADESMENU;	//Upgrades Menu
	upgradesMenu->active = true;
	upgradesMenu->pos.Set(-10, -10, -5);
	upgradesMenu->scale.Set(1.0f, 1.0f, 1.0f);

	upgradesMenuNextPage->type = GameObject::GO_UPGRADESMENU_NEXTPAGE;	//Upgrades Menu
	upgradesMenuNextPage->active = true;
	upgradesMenuNextPage->pos.Set(-10, -10, -5);
	upgradesMenuNextPage->scale.Set(1.0f, 1.0f, 1.0f);

	speed_upgrade_1->type = GameObject::GO_SPEED_UPGRADE_1;	//Speed_Upgrade_1
	speed_upgrade_1->active = true;
	speed_upgrade_1->pos.Set(-10, -10, -5);
	speed_upgrade_1->scale.Set(1.0f, 1.0f, 1.0f);

	speed_upgrade_2->type = GameObject::GO_SPEED_UPGRADE_2;	//Speed_Upgrade_2
	speed_upgrade_2->active = true;
	speed_upgrade_2->pos.Set(-10, -10, -5);
	speed_upgrade_2->scale.Set(1.0f, 1.0f, 1.0f);

	speed_multiplyer_upgrade_1->type = GameObject::GO_SPEED_MULTIPLYER_UPGRADE_1;	//Speed_Upgrade_1
	speed_multiplyer_upgrade_1->active = true;
	speed_multiplyer_upgrade_1->pos.Set(-10, -10, -5);
	speed_multiplyer_upgrade_1->scale.Set(1.0f, 1.0f, 1.0f);

	speed_multiplyer_upgrade_2->type = GameObject::GO_SPEED_MULTIPLYER_UPGRADE_2;	//Speed_Upgrade_2
	speed_multiplyer_upgrade_2->active = true;
	speed_multiplyer_upgrade_2->pos.Set(-10, -10, -5);
	speed_multiplyer_upgrade_2->scale.Set(1.0f, 1.0f, 1.0f);

	i_currency = GetCurrency();

	//Cost of upgrades
	i_cost_speed_upgrade_1 = 5;
	i_cost_speed_upgrade_2 = 15;
	i_cost_speed_multiplyer_upgrade_1 = 100;
	i_cost_speed_multiplyer_upgrade_2 = 150;
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
	upgraded.ReadFile("Text//Speed_Multiplyer_Upgrade.txt");

	SetCurrency(i_currency);

	pressDelay += (float)dt;

	int h_temp = 100;
	int w_temp = 100 * Application::GetWindowWidth() / Application::GetWindowHeight();

	//Position values (for upgrade menu)
	float posXDownArrow = 0.7f;
	float posXDownArrow_NextLevel = 1.1f;
	float posXDownArrow_Page = 0.57f;
	float posYDownArrow_Upgrade1 = 0.82f;
	float posYDownArrow_Upgrade2 = 1.33f;
	float posYDownArrow_Multiplyer_Upgrade1 = 0.82f;
	float posYDownArrow_Multiplyer_Upgrade2 = 1.33f;
	float posYDownArrow_Page_Level = 3.35f;

	//Scale values (for upgrade menu)
	float scaleDown_Arrow = 20.f;

	//Press R to reset
	if (Application::IsKeyPressed('R'))
	{
		SceneManager::getInstance()->changeScene(new SceneMainMenu());
		SetCurrentLevel(1);
		SetTempScore(0);
		SetScore(0);
		SetCurrency(0);
		SetTempCurrency(0);
		upgraded.DeleteTextFile("Text//Speed_Upgrade.txt");
		upgraded.ReadFile("Text//Speed_Upgrade.txt");
		upgraded.DeleteTextFile("Text//Speed_Multiplyer_Upgrade.txt");
		upgraded.ReadFile("Text//Speed_Multiplyer_Upgrade.txt");
	}

	//UP, DOWN and ENTER controls
	{
		//Prevent pressDelay from exceeding 0.5f
		if (pressDelay > 0.5f)
			pressDelay = 0.5f;

		if ((Application::IsKeyPressed(VK_UP)) && pressDelay >= cooldownPressed)
		{
			//First Page
			if (selectOptions == NEXTPAGE)
				selectOptions = NEXTLEVEL_1;

			else if (selectOptions == NEXTLEVEL_1)
				selectOptions = SPEED_UPGRADE_2;

			else if (selectOptions == SPEED_UPGRADE_2)
				selectOptions = SPEED_UPGRADE_1;

			else if (selectOptions == SPEED_UPGRADE_1)
				selectOptions = NEXTPAGE;

			//Second Page
			else if (selectOptions == PREVIOUSPAGE)
				selectOptions = NEXTLEVEL_2;

			else if (selectOptions == NEXTLEVEL_2)
				selectOptions = SPEED_MULTIPLYER_2;

			else if (selectOptions == SPEED_MULTIPLYER_2)
				selectOptions = SPEED_MULTIPLYER_1;

			else if (selectOptions == SPEED_MULTIPLYER_1)
				selectOptions = PREVIOUSPAGE;

			pressDelay = 0.f;
		}

		if ((Application::IsKeyPressed(VK_DOWN)) && pressDelay >= cooldownPressed)
		{
			//First Page
			if (selectOptions == SPEED_UPGRADE_1)
				selectOptions = SPEED_UPGRADE_2;

			else if (selectOptions == SPEED_UPGRADE_2)
				selectOptions = NEXTLEVEL_1;

			else if (selectOptions == NEXTLEVEL_1)
				selectOptions = NEXTPAGE;

			else if (selectOptions == NEXTPAGE)
				selectOptions = SPEED_UPGRADE_1;

			//Second Page
			else if (selectOptions == SPEED_MULTIPLYER_1)
				selectOptions = SPEED_MULTIPLYER_2;

			else if (selectOptions == SPEED_MULTIPLYER_2)
				selectOptions = NEXTLEVEL_2;

			else if (selectOptions == NEXTLEVEL_2)
				selectOptions = PREVIOUSPAGE;

			else if (selectOptions == PREVIOUSPAGE)
				selectOptions = SPEED_MULTIPLYER_1;

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == NEXTLEVEL_1 || selectOptions == NEXTLEVEL_2)
				SceneManager::getInstance()->changeScene(new SceneCollision());

			else if (selectOptions == NEXTPAGE)
			{
				b_previous_page = false;
				b_next_page = true;
				selectOptions = SPEED_MULTIPLYER_1;
			}

			else if (selectOptions == PREVIOUSPAGE)
			{
				b_previous_page = true;
				b_next_page = false;
				selectOptions = SPEED_UPGRADE_1;
			}

			else if (selectOptions == SPEED_UPGRADE_1 && upgraded.speed_upgrade != 1 && upgraded.speed_upgrade != 2 && i_currency >= i_cost_speed_upgrade_1)
			{
				//Bought upgrade
				b_speed_upgrade_1 = true;

				//Deduct currency
				i_currency -= i_cost_speed_upgrade_1;
			}

			else if (selectOptions == SPEED_UPGRADE_2 && upgraded.speed_upgrade == 1 && i_currency >= i_cost_speed_upgrade_2)
			{
				//Bought upgrade
				b_speed_upgrade_2 = true;

				//Deduct currency
				i_currency -= i_cost_speed_upgrade_2;
			}

			else if (selectOptions == SPEED_MULTIPLYER_1 && upgraded.speed_multiplyer_upgrade != 1 && upgraded.speed_multiplyer_upgrade != 2 && i_currency >= i_cost_speed_multiplyer_upgrade_1)
			{
				//Bought upgrade
				b_speed_multiplyer_upgrade_1 = true;

				//Deduct currency
				i_currency -= i_cost_speed_multiplyer_upgrade_1;
			}

			else if (selectOptions == SPEED_MULTIPLYER_2 && upgraded.speed_multiplyer_upgrade == 1 && i_currency >= i_cost_speed_multiplyer_upgrade_2)
			{
				//Bought upgrade
				b_speed_multiplyer_upgrade_2 = true;

				//Deduct currency
				i_currency -= i_cost_speed_multiplyer_upgrade_2;
			}

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
		upgradesMenuNextPage->active = false;
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

	case SPEED_MULTIPLYER_1:
		//Render in arrow 
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Multiplyer_Upgrade1, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenu->active = false;
		upgradesMenuNextPage->active = true;
		upgradesMenuNextPage->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenuNextPage->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case SPEED_MULTIPLYER_2:
		//Render in arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Multiplyer_Upgrade2, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenuNextPage->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenuNextPage->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case NEXTLEVEL_1:
		//Render in arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_NextLevel, (h_temp / 2) / posYDownArrow_Page_Level, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case NEXTLEVEL_2:
		//Render in arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_NextLevel, (h_temp / 2) / posYDownArrow_Page_Level, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenuNextPage->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenuNextPage->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case NEXTPAGE:
		//Render in arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Page, (h_temp / 2) / posYDownArrow_Page_Level, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case PREVIOUSPAGE:
		//Render in arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Page, (h_temp / 2) / posYDownArrow_Page_Level, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render in upgrades menu
		upgradesMenuNextPage->pos.Set(w_temp / 2, h_temp / 2, -5);
		upgradesMenuNextPage->scale.Set(w_temp + 2, h_temp, 1);
		break;
	}


	//Going to previous page (page 1)
	if (b_previous_page)
	{
		//False Actives
		upgradesMenuNextPage->active = false;
		speed_multiplyer_upgrade_1->active = false;
		speed_multiplyer_upgrade_2->active = false;

		//Buying first speed upgrade
		if (b_speed_upgrade_1)
		{
			//Render in upgrade1
			upgradesMenu->active = false;
			speed_upgrade_2->active = false;
			speed_upgrade_1->active = true;
			speed_upgrade_1->pos.Set(w_temp / 2, h_temp / 2, -5);
			speed_upgrade_1->scale.Set(w_temp + 2, h_temp, 1);

			//.txt
			upgraded.WriteFile("Text//Speed_Upgrade.txt", "Speed", 1);
		}
		//Buying second speed upgrade
		if (b_speed_upgrade_2)
		{
			//Render in upgrade2
			upgradesMenu->active = false;
			speed_upgrade_1->active = false;
			speed_upgrade_2->active = true;
			speed_upgrade_2->pos.Set(w_temp / 2, h_temp / 2, -5);
			speed_upgrade_2->scale.Set(w_temp + 2, h_temp, 1);

			//.txt
			upgraded.WriteFile("Text//Speed_Upgrade.txt", "Speed", 2);
		}
		//Bought first speed upgrade
		if (upgraded.speed_upgrade == 1)
		{
			//Render in upgrade1
			upgradesMenu->active = false;
			speed_upgrade_2->active = false;
			speed_upgrade_1->active = true;
			speed_upgrade_1->pos.Set(w_temp / 2, h_temp / 2, -5);
			speed_upgrade_1->scale.Set(w_temp + 2, h_temp, 1);
		}
		//Bought second speed upgrade
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


	//Going to next page (page 2)
	if (b_next_page)
	{
		//False Actives
		upgradesMenu->active = false;
		speed_upgrade_1->active = false;
		speed_upgrade_2->active = false;

		//Buying first speed multiplyer upgrade
		if (b_speed_multiplyer_upgrade_1)
		{
			//Render in upgrade1
			upgradesMenuNextPage->active = false;
			speed_multiplyer_upgrade_2->active = false;
			speed_multiplyer_upgrade_1->active = true;
			speed_multiplyer_upgrade_1->pos.Set(w_temp / 2, h_temp / 2, -5);
			speed_multiplyer_upgrade_1->scale.Set(w_temp + 2, h_temp, 1);

			//.txt
			upgraded.WriteFile("Text//Speed_Multiplyer_Upgrade.txt", "Speed_Multiplyer", 1);
		}
		//Buying second speed multiplyer upgrade
		if (b_speed_multiplyer_upgrade_2)
		{
			//Render in upgrade2
			upgradesMenuNextPage->active = false;
			speed_multiplyer_upgrade_1->active = false;
			speed_multiplyer_upgrade_2->active = true;
			speed_multiplyer_upgrade_2->pos.Set(w_temp / 2, h_temp / 2, -5);
			speed_multiplyer_upgrade_2->scale.Set(w_temp + 2, h_temp, 1);

			//.txt
			upgraded.WriteFile("Text//Speed_Multiplyer_Upgrade.txt", "Speed_Multiplyer", 2);
		}
		//Bought first speed multiplyer upgrade
		if (upgraded.speed_multiplyer_upgrade == 1)
		{
			//Render in upgrade1
			upgradesMenuNextPage->active = false;
			speed_multiplyer_upgrade_2->active = false;
			speed_multiplyer_upgrade_1->active = true;
			speed_multiplyer_upgrade_1->pos.Set(w_temp / 2, h_temp / 2, -5);
			speed_multiplyer_upgrade_1->scale.Set(w_temp + 2, h_temp, 1);
		}
		//Bought second multiplyer upgrade
		if (upgraded.speed_multiplyer_upgrade == 2)
		{
			//Render in upgrade2
			upgradesMenuNextPage->active = false;
			speed_multiplyer_upgrade_1->active = false;
			speed_multiplyer_upgrade_2->active = true;
			speed_multiplyer_upgrade_2->pos.Set(w_temp / 2, h_temp / 2, -5);
			speed_multiplyer_upgrade_2->scale.Set(w_temp + 2, h_temp, 1);
		}
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

void SceneUpgrade::SetCurrency(int currency)
{
	ofstream myFile;
	myFile.open("Text//Currency.txt");
	myFile << currency << endl;
	myFile.close();
}

int SceneUpgrade::GetCurrency()
{
	int currency;
	ifstream myFile;
	myFile.open("Text//Currency.txt");
	myFile >> currency;
	myFile.close();

	return currency;
}

void SceneUpgrade::SetTempCurrency(int tempCurrency)
{
	ofstream myFile;
	myFile.open("Text//TempCurrency.txt");
	myFile << tempCurrency << endl;
	myFile.close();
}

int SceneUpgrade::GetTempCurrency()
{
	int tempCurrency;
	ifstream myFile;
	myFile.open("Text//TempCurrency.txt");
	myFile >> tempCurrency;
	myFile.close();

	return tempCurrency;
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
	case GameObject::GO_UPGRADESMENU_NEXTPAGE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_UPGRADESMENU_NEXTPAGE], true, go->Color);
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
	case GameObject::GO_SPEED_MULTIPLYER_UPGRADE_1:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SPEED_MULTIPLYER_UPGRADE_1], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SPEED_MULTIPLYER_UPGRADE_2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SPEED_MULTIPLYER_UPGRADE_2], true, go->Color);
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

	std::ostringstream ss;

	ss.str(std::string());
	ss.precision(5);
	ss << "Currency: " << GetCurrency();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);

	switch (selectOptions)
	{
	case SPEED_UPGRADE_1:
		ss.str(std::string());
		ss.precision(5);
		ss << "Cost: " << i_cost_speed_upgrade_1;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
		break;
	case SPEED_UPGRADE_2:
		ss.str(std::string());
		ss.precision(5);
		ss << "Cost: " << i_cost_speed_upgrade_2;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
		break;
	case SPEED_MULTIPLYER_1:
		ss.str(std::string());
		ss.precision(5);
		ss << "Cost: " << i_cost_speed_multiplyer_upgrade_1;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
		break;
	case SPEED_MULTIPLYER_2:
		ss.str(std::string());
		ss.precision(5);
		ss << "Cost: " << i_cost_speed_multiplyer_upgrade_2;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
		break;
	}
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