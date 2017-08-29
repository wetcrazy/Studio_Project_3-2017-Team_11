#include "SceneInstructions.h"

#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"

#include <sstream>

#include "SceneManager.h"
#include "SceneMainMenu.h"

SceneInstructions::SceneInstructions()
{
}

SceneInstructions::~SceneInstructions()
{
}

void SceneInstructions::Init()
{
	SceneBase::Init();

	Math::InitRNG();

	arrows->type = GameObject::GO_ARROW;	//Arrow 
	arrows->active = true;
	arrows->pos.Set(-10, -10, 1);
	arrows->scale.Set(1.0f, 1.0f, 1.0f);

	pg1->type = GameObject::GO_INSTRUCTIONS1;	//Pg 1
	pg1->active = true;
	pg1->pos.Set(-10, -10, -5);
	pg1->scale.Set(1.0f, 1.0f, 1.0f);

	pg2->type = GameObject::GO_INSTRUCTIONS2;	//Pg 2
	pg2->active = true;
	pg2->pos.Set(-10, -10, -5);
	pg2->scale.Set(1.0f, 1.0f, 1.0f);

	pg3->type = GameObject::GO_INSTRUCTIONS3;	//Pg 3
	pg3->active = true;
	pg3->pos.Set(-10, -10, -5);
	pg3->scale.Set(1.0f, 1.0f, 1.0f);

	next_page1 = false;
	next_page2 = false;
	previous_page1 = false;
	previous_page2 = false;
}

GameObject* SceneInstructions::FetchGO()
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
}

void SceneInstructions::Update(double dt)
{
	SceneBase::Update(dt);

	pressDelay += (float)dt;

	int h_temp = 100;
	int w_temp = 100 * Application::GetWindowWidth() / Application::GetWindowHeight();

	//Position values (for instructions file)
	float posXDownArrow_Saves = 0.72f;
	float posXDownArrow_Select = 1.55f;
	float posXDownArrow_Delete = 0.9f;
	float posXDownArrow_MainMenu = 0.57f;
	float posYDownArrow_Save1 = 0.765f;
	float posYDownArrow_Save2 = 1.1f;
	float posYDownArrow_Save3 = 1.89f;
	float posYDownArrow_Options = 5.f;

	//Scale values (for instructions file)
	float scaleDown_Arrow = 20.f;

	//UP, DOWN and ENTER controls
	{
		//Prevent pressDelay from exceeding 0.5f
		if (pressDelay > 0.5f)
			pressDelay = 0.5f;

		if ((Application::IsKeyPressed(VK_LEFT)) && pressDelay >= cooldownPressed)
		{
			//First Page
			if (selectOptions == NEXT_1)
				selectOptions = MAIN_MENU;
			else if (selectOptions == MAIN_MENU)
				selectOptions = NEXT_1;

			//Second Page
			else if (selectOptions == PREVIOUS_1)
				selectOptions = NEXT_2;
			else if (selectOptions == NEXT_2)
				selectOptions = PREVIOUS_1;

			//Third Page
			else if (selectOptions == PREVIOUS_2)
				selectOptions = MAIN_MENU;
			else if (selectOptions == MAIN_MENU)
				selectOptions = PREVIOUS_2;

			pressDelay = 0.f;
		}

		if ((Application::IsKeyPressed(VK_RIGHT)) && pressDelay >= cooldownPressed)
		{
			//First Page
			if (selectOptions == MAIN_MENU)
				selectOptions = NEXT_1;
			else if (selectOptions == NEXT_1)
				selectOptions = MAIN_MENU;

			//Second Page
			else if (selectOptions == NEXT_2)
				selectOptions = PREVIOUS_1;
			else if (selectOptions == PREVIOUS_1)
				selectOptions = NEXT_2;

			//Third Page
			else if (selectOptions == MAIN_MENU)
				selectOptions = PREVIOUS_2;
			else if (selectOptions == PREVIOUS_2)
				selectOptions = MAIN_MENU;

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == MAIN_MENU)
				SceneManager::getInstance()->changeScene(new SceneMainMenu());

			else if (selectOptions == NEXT_1)
			{
				next_page1 = true;
				next_page2 = false;
				previous_page1 = false;
				previous_page2 = false;

				selectOptions = NEXT_2; //default selection for next page
			}
			if (selectOptions == NEXT_2)
			{
				next_page1 = false;
				next_page2 = true;
				previous_page1 = false;
				previous_page2 = false;

				selectOptions = MAIN_MENU; //default selection for next page
			}

			else if (selectOptions == PREVIOUS_1)
			{
				next_page1 = false;
				next_page2 = false;
				previous_page1 = true;
				previous_page2 = false;

				selectOptions = MAIN_MENU; //default selection for next page
			}

			else if (selectOptions == PREVIOUS_2)
			{
				next_page1 = false;
				next_page2 = false;
				previous_page1 = false;
				previous_page2 = true;

				selectOptions = NEXT_1; //default selection for next page
			}

			pressDelay = 0.f;
		}
	}

	switch (selectOptions)
	{
	case NEXT_1:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Saves, (h_temp / 2) / posYDownArrow_Save2, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg2->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg2->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case NEXT_2:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Saves, (h_temp / 2) / posYDownArrow_Save3, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg3->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg3->scale.Set(w_temp + 2, h_temp, 1);
		break;
	case PREVIOUS_1:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Saves, (h_temp / 2) / posYDownArrow_Save2, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg1->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg1->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case PREVIOUS_2:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXDownArrow_Saves, (h_temp / 2) / posYDownArrow_Save3, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg2->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg2->scale.Set(w_temp + 2, h_temp, 1);
		break;
	}
}

void SceneInstructions::RenderGO(GameObject *go)
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
	case GameObject::GO_INSTRUCTIONS1:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_INSTRUCTIONS1], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_INSTRUCTIONS2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_INSTRUCTIONS2], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_INSTRUCTIONS3:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_INSTRUCTIONS3], true, go->Color);
		modelStack.PopMatrix();
		break;
	}
}

void SceneInstructions::Render()
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
}

void SceneInstructions::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
