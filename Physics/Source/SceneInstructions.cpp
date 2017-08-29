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

	b_page1 = true;
	b_page2 = false;
	b_page3 = false;
	b_page4 = false;

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

	pg4->type = GameObject::GO_INSTRUCTIONS4;	//Pg 4
	pg4->active = true;
	pg4->pos.Set(-10, -10, -5);
	pg4->scale.Set(1.0f, 1.0f, 1.0f);
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

void SceneInstructions::Update(double dt)
{
	SceneBase::Update(dt);

	pressDelay += (float)dt;

	int h_temp = 100;
	int w_temp = 100 * Application::GetWindowWidth() / Application::GetWindowHeight();

	//Position values (for instructions file)
	float posXArrow_MainMenu_Page1 = 1.12f;
	float posXArrow_Next_Page1 = 0.6f;
	float posXArrow_Previous_Page2 = 1.28f;
	float posXArrow_Next_Page2 = 0.6f;
	float posXArrow_Previous_Page3 = 1.28f;
	float posXArrow_Next_Page3 = 0.6f;
	float posXArrow_Previous_Page4 = 1.28f;
	float posXArrow_MainMenu_Page4 = 0.59f;
	float posYArrow = 3.6f;

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
			if (selectOptions == NEXT_PAGE1)
				selectOptions = MAIN_MENU_PAGE1;
			else if (selectOptions == MAIN_MENU_PAGE1)
				selectOptions = NEXT_PAGE1;

			//Second Page
			else if (selectOptions == PREVIOUS_PAGE2)
				selectOptions = NEXT_PAGE2;
			else if (selectOptions == NEXT_PAGE2)
				selectOptions = PREVIOUS_PAGE2;

			//Third Page
			else if (selectOptions == PREVIOUS_PAGE3)
				selectOptions = NEXT_PAGE3;
			else if (selectOptions == NEXT_PAGE3)
				selectOptions = PREVIOUS_PAGE3;

			//Fourth Page
			else if (selectOptions == PREVIOUS_PAGE4)
				selectOptions = MAIN_MENU_PAGE4;
			else if (selectOptions == MAIN_MENU_PAGE4)
				selectOptions = PREVIOUS_PAGE4;

			pressDelay = 0.f;
		}

		if ((Application::IsKeyPressed(VK_RIGHT)) && pressDelay >= cooldownPressed)
		{
			//First Page
			if (selectOptions == NEXT_PAGE1)
				selectOptions = MAIN_MENU_PAGE1;
			else if (selectOptions == MAIN_MENU_PAGE1)
				selectOptions = NEXT_PAGE1;

			//Second Page
			else if (selectOptions == PREVIOUS_PAGE2)
				selectOptions = NEXT_PAGE2;
			else if (selectOptions == NEXT_PAGE2)
				selectOptions = PREVIOUS_PAGE2;

			//Third Page
			else if (selectOptions == PREVIOUS_PAGE3)
				selectOptions = NEXT_PAGE3;
			else if (selectOptions == NEXT_PAGE3)
				selectOptions = PREVIOUS_PAGE3;

			//Fourth Page
			else if (selectOptions == PREVIOUS_PAGE4)
				selectOptions = MAIN_MENU_PAGE4;
			else if (selectOptions == MAIN_MENU_PAGE4)
				selectOptions = PREVIOUS_PAGE4;

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
		{
			if (selectOptions == MAIN_MENU_PAGE1 || selectOptions == MAIN_MENU_PAGE4)
				SceneManager::getInstance()->changeScene(new SceneMainMenu());

			else if (selectOptions == NEXT_PAGE1)
			{
				b_page2 = true;
				b_page1 = false;
				b_page3 = false;
				b_page4 = false;

				selectOptions = PREVIOUS_PAGE2; //default selection for next page
			}
			else if (selectOptions == NEXT_PAGE2)
			{
				b_page3 = true;
				b_page1 = false;
				b_page2 = false;
				b_page4 = false;

				selectOptions = PREVIOUS_PAGE3; //default selection for next page
			}
			else if (selectOptions == NEXT_PAGE3)
			{
				b_page4 = true;
				b_page1 = false;
				b_page2 = false;
				b_page3 = false;

				selectOptions = PREVIOUS_PAGE4; //default selection for next page
			}

			else if (selectOptions == PREVIOUS_PAGE2 && b_page2)
			{
				b_page1 = true;
				b_page2 = false;
				b_page3 = false;
				b_page4 = false;

				selectOptions = MAIN_MENU_PAGE1; //default selection for previous page
			}

			else if (selectOptions == PREVIOUS_PAGE3 && b_page3)
			{
				b_page2 = true;
				b_page1 = false;
				b_page3 = false;
				b_page4 = false;

				selectOptions = PREVIOUS_PAGE2; //default selection for previous page
			}

			else if (selectOptions == PREVIOUS_PAGE4 && b_page4)
			{
				b_page3 = true;
				b_page1 = false;
				b_page2 = false;
				b_page4 = false;

				selectOptions = PREVIOUS_PAGE3; //default selection for previous page
			}

			pressDelay = 0.f;
		}
	}

	switch (selectOptions)
	{
	case MAIN_MENU_PAGE1:
		//Render arrow
		arrows->active = true;
		arrows->pos.Set((w_temp / 2) / posXArrow_MainMenu_Page1, (h_temp / 2) / posYArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg2->active = false;
		pg3->active = false;
		pg4->active = false;
		pg1->active = true;
		pg1->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg1->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case NEXT_PAGE1:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXArrow_Next_Page1, (h_temp / 2) / posYArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg1->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg1->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case PREVIOUS_PAGE2:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXArrow_Previous_Page2, (h_temp / 2) / posYArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg1->active = false;
		pg3->active = false;
		pg4->active = false;
		pg2->active = true;
		pg2->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg2->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case NEXT_PAGE2:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXArrow_Next_Page2, (h_temp / 2) / posYArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg2->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg2->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case PREVIOUS_PAGE3:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXArrow_Previous_Page3, (h_temp / 2) / posYArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg1->active = false;
		pg3->active = false;
		pg4->active = false;
		pg3->active = true;
		pg3->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg3->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case NEXT_PAGE3:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXArrow_Next_Page3, (h_temp / 2) / posYArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg3->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg3->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case PREVIOUS_PAGE4:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXArrow_Previous_Page4, (h_temp / 2) / posYArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg1->active = false;
		pg3->active = false;
		pg3->active = false;
		pg4->active = true;
		pg4->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg4->scale.Set(w_temp + 2, h_temp, 1);
		break;

	case MAIN_MENU_PAGE4:
		//Render arrow
		arrows->pos.Set((w_temp / 2) / posXArrow_MainMenu_Page4, (h_temp / 2) / posYArrow, 1);
		arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

		//Render instructions menu
		pg4->pos.Set(w_temp / 2, h_temp / 2, -5);
		pg4->scale.Set(w_temp + 2, h_temp, 1);
		break;
	}

	//If at first page
	if (b_page1)
	{
		pg1->active = true;
		pg2->active = false;
		pg3->active = false;
		pg4->active = false;
	}
	//If at second page
	if (b_page2)
	{
		pg2->active = true;
		pg1->active = false;
		pg3->active = false;
		pg4->active = false;
	}
	//If at third page
	if (b_page3)
	{
		pg3->active = true;
		pg1->active = false;
		pg2->active = false;
		pg4->active = false;
	}
	//If at fourth page
	if (b_page4)
	{
		pg4->active = true;
		pg1->active = false;
		pg2->active = false;
		pg3->active = false;
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
	case GameObject::GO_INSTRUCTIONS4:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_INSTRUCTIONS4], true, go->Color);
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
