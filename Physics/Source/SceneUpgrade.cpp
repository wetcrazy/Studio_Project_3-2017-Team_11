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

#include "SceneManager.h"
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

	//Main Menu
	meshList[GEO_UPGRADEMENU] = MeshBuilder::GenerateQuad("mainmenu", Color(1, 1, 1));
	meshList[GEO_UPGRADEMENU]->textureID = LoadTGA("Image//Menu/Upgrades.tga");

	//Arrow
	meshList[GEO_ARROW] = MeshBuilder::GenerateQuad("arrow", Color(1, 1, 1));
	meshList[GEO_ARROW]->textureID = LoadTGA("Image//Menu/Arrow.tga");

	//Text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	/*---------------------------Initialising Variables---------------------------------*/
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);
	/*-----------------------------------------------------------------------------*/
}

void SceneUpgrade::Update(double dt)
{
	int framespersec = 1 / dt;
	//elapsedTime += dt;
	camera.Update(dt);

	fps = "FPS:" + std::to_string(framespersec);

	pressDelay += (float)dt;

	//Prevent pressDelay from exceeding 0.5f
	if (pressDelay > 0.5f)
		pressDelay = 0.5f;

	if ((Application::IsKeyPressed(VK_UP)) && pressDelay >= cooldownPressed)
	{
		if (selectOptions == RETURN)
			selectOptions = GRADE_1;

		else if (selectOptions == GRADE_1)
			selectOptions = RETURN;

		pressDelay = 0.f;
	}

	if ((Application::IsKeyPressed(VK_DOWN)) && pressDelay >= cooldownPressed)
	{
		if (selectOptions == RETURN)
			selectOptions = GRADE_1;

		else if (selectOptions == GRADE_1)
			selectOptions = RETURN;

		pressDelay = 0.f;
	}

	if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
	{
		if (selectOptions == RETURN)
			SceneManager::getInstance()->changeScene(new SceneCollision());

		pressDelay = 0.f;
	}
}

void SceneUpgrade::Render()
{
	// Render VBO here
	Mtx44 MVP;

	// Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

	modelStack.LoadIdentity();

	RenderMeshOnScreen(meshList[GEO_UPGRADEMENU], 0, 0, 80, 60, 0);

	// Positioning of arrows
	switch (selectOptions)
	{
	case GRADE_1:
		RenderMeshOnScreen(meshList[GEO_ARROW], 7, 6.2, 3, 3, 0);
		break;
	case RETURN:
		RenderMeshOnScreen(meshList[GEO_ARROW], 7, 4.2, 3, 3, 0);
		break;
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