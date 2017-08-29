#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include "SpriteAnimation.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//comg.vertexshader", "Shader//comg.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 1, 1), 10, 10, 1.f);
	meshList[GEO_HEXA] = MeshBuilder::GenerateHexa("hexa", Color(1, 1, 1), 3, 2, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1.f);
	meshList[GEO_SHIP] = MeshBuilder::GenerateQuad("ship", Color(1, 1, 1), 5.f);
	meshList[GEO_SHIP]->textureID = LoadTGA("Image//ship.tga");
	meshList[GEO_ASTEROID] = MeshBuilder::GenerateQuad("asteroid", Color(1, 1, 1), 3.f);
	meshList[GEO_ASTEROID]->textureID = LoadTGA("Image//asteroid.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	//Mountain Background
	meshList[GEO_MOUNTAIN] = MeshBuilder::GenerateQuad("static mountain", Color(1, 1, 1), 1.f);
	meshList[GEO_MOUNTAIN]->textureID = LoadTGA("Image//Background/mountain.tga");

	//Background Fire
	meshList[GEO_BACKGROUND_FIRE] = MeshBuilder::GenerateSpriteAnimation("sprite background fire", 1, 5, 1.0f);
	meshList[GEO_BACKGROUND_FIRE]->textureID = LoadTGA("Image//Background/sprite_background_fire.tga");
	//Sprite Animation for Background Fire
	SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_BACKGROUND_FIRE]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 4, 0, 0.85f, true);
	}

	//Foreground Fire
	meshList[GEO_FOREGROUND_FIRE] = MeshBuilder::GenerateSpriteAnimation("sprite foreground fire", 1, 5, 1.0f);
	meshList[GEO_FOREGROUND_FIRE]->textureID = LoadTGA("Image//Background/sprite_foreground_fire.tga");
	//Sprite Animation for Foreground Fire
	SpriteAnimation* sa1 = dynamic_cast<SpriteAnimation*>(meshList[GEO_FOREGROUND_FIRE]);
	if (sa1)
	{
		sa1->m_anim = new Animation();
		sa1->m_anim->Set(0, 4, 0, 0.75f, true);
	}

	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 1.f);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//ground.tga");
	meshList[GEO_PLATFORM] = MeshBuilder::GenerateQuad("platform", Color(1, 1, 1), 1.f);
	meshList[GEO_PLATFORM]->textureID = LoadTGA("Image//tile.tga");

	meshList[GEO_CANNON] = MeshBuilder::GenerateQuad("cannon", Color(1, 1, 1), 5.f);
	meshList[GEO_CANNON]->textureID = LoadTGA("Image//cannon.tga");
	meshList[GEO_CANNON_PLATFORM] = MeshBuilder::GenerateQuad("cannon platform", Color(1, 1, 1), 5.f);
	meshList[GEO_CANNON_PLATFORM]->textureID = LoadTGA("Image//cannon_platform.tga");

	//Arrow for menus
	meshList[GEO_ARROW] = MeshBuilder::GenerateQuad("arrow", Color(1, 1, 1), 1);
	meshList[GEO_ARROW]->textureID = LoadTGA("Image//Menu/Arrow.tga");

	//Main Menu
	meshList[GEO_MAINMENU] = MeshBuilder::GenerateQuad("mainmenu", Color(1, 1, 1), 1);
	meshList[GEO_MAINMENU]->textureID = LoadTGA("Image//Menu/main_menu.tga");

	//Upgrades Menu
	meshList[GEO_UPGRADESMENU] = MeshBuilder::GenerateQuad("upgradesmenu", Color(1, 1, 1), 1);
	meshList[GEO_UPGRADESMENU]->textureID = LoadTGA("Image//Menu/Upgrades/upgrades.tga");
	meshList[GEO_UPGRADESMENU_NEXTPAGE] = MeshBuilder::GenerateQuad("upgradesmenu_nextpage", Color(1, 1, 1), 1);
	meshList[GEO_UPGRADESMENU_NEXTPAGE]->textureID = LoadTGA("Image//Menu/Upgrades/upgrades2.tga");
	meshList[GEO_SPEED_UPGRADE_1] = MeshBuilder::GenerateQuad("upgradesmenu_g1", Color(1, 1, 1), 1);
	meshList[GEO_SPEED_UPGRADE_1]->textureID = LoadTGA("Image//Menu/Upgrades/upgrades_g1.tga");
	meshList[GEO_SPEED_UPGRADE_2] = MeshBuilder::GenerateQuad("upgradesmenu_g2", Color(1, 1, 1), 1);
	meshList[GEO_SPEED_UPGRADE_2]->textureID = LoadTGA("Image//Menu/Upgrades/upgrades_g2.tga");
	meshList[GEO_SPEED_MULTIPLYER_UPGRADE_1] = MeshBuilder::GenerateQuad("upgradesmenu_m1", Color(1, 1, 1), 1);
	meshList[GEO_SPEED_MULTIPLYER_UPGRADE_1]->textureID = LoadTGA("Image//Menu/Upgrades/upgrades2_m1.tga");
	meshList[GEO_SPEED_MULTIPLYER_UPGRADE_2] = MeshBuilder::GenerateQuad("upgradesmenu_m2", Color(1, 1, 1), 1);
	meshList[GEO_SPEED_MULTIPLYER_UPGRADE_2]->textureID = LoadTGA("Image//Menu/Upgrades/upgrades2_m2.tga");

	// HighScore Menu
	meshList[GEO_HIGHSCOREMENU] = MeshBuilder::GenerateQuad("highscore", Color(1, 1, 1));
	meshList[GEO_HIGHSCOREMENU]->textureID = LoadTGA("Image//Menu/HighScore Menu//highscore.tga");

	//Save File Menu
	meshList[GEO_SAVE1] = MeshBuilder::GenerateQuad("save1", Color(1, 1, 1));
	meshList[GEO_SAVE1]->textureID = LoadTGA("Image//Menu/Save File//save1.tga");
	meshList[GEO_SAVE2] = MeshBuilder::GenerateQuad("save2", Color(1, 1, 1));
	meshList[GEO_SAVE2]->textureID = LoadTGA("Image//Menu/Save File//save2.tga");
	meshList[GEO_SAVE3] = MeshBuilder::GenerateQuad("save3", Color(1, 1, 1));
	meshList[GEO_SAVE3]->textureID = LoadTGA("Image//Menu/Save File//save3.tga");

	// Power Bar
	meshList[GEO_POWERRANGE] = MeshBuilder::GenerateQuad("POWERBAR", Color(1, 0, 1), 1);
	meshList[GEO_POWERRANGE]->textureID = LoadTGA("Image//Powerbar/power_bar.tga");

	meshList[GEO_POWERRANGE_FIRED] = MeshBuilder::GenerateQuad("POWERBAR_FIRED", Color(1, 0, 1), 1);
	meshList[GEO_POWERRANGE_FIRED]->textureID = LoadTGA("Image//Powerbar/power_bar_blast.tga");

	meshList[GEO_POWERBAR] = MeshBuilder::GenerateQuad("POWERBAR", Color(0, 0, 0), 1);

	meshList[GEO_GUIDEMARKER] = MeshBuilder::GenerateQuad("POWERBAR", Color(1, 1, 1), 1);

	meshList[GEO_POWER_BG] = MeshBuilder::GenerateQuad("POWERBAR_BG", Color(1, 1, 1), 1);
	meshList[GEO_POWER_BG]->textureID = LoadTGA("Image//Powerbar/marker.tga");

	// FAIL Menu
	meshList[GEO_FAILMENU] = MeshBuilder::GenerateQuad("FAIL", Color(1, 1, 1));
	meshList[GEO_FAILMENU]->textureID = LoadTGA("Image//Menu/fail.tga");

	//Success Menu
	meshList[GEO_SUCCESSMENU] = MeshBuilder::GenerateQuad("SUCCESS", Color(1, 1, 1));
	meshList[GEO_SUCCESSMENU]->textureID = LoadTGA("Image//Menu/success.tga");

	// Star
	meshList[GEO_STAR] = MeshBuilder::GenerateQuad("STAR", Color(1, 1, 1));
	meshList[GEO_STAR]->textureID = LoadTGA("Image//Menu/star.tga");

	bLightEnabled = true;
}

void SceneBase::Update(double dt)
{
	//Keyboard Section
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	fps = (float)(1.f / dt);
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size_x, float size_y, float x, float y, float spacing)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size_x, size_y, 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * spacing + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneBase::RenderMesh(Mesh * mesh, bool enableLight, Vector3 color)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		mesh->material.kAmbient.r = color.x;
		mesh->material.kAmbient.g = color.y;
		mesh->material.kAmbient.b = color.z;

		mesh->material.kDiffuse.r = color.x;
		mesh->material.kDiffuse.g = color.y;
		mesh->material.kDiffuse.b = color.z;

		mesh->material.kSpecular.r = color.x;
		mesh->material.kSpecular.g = color.y;
		mesh->material.kSpecular.b = color.z;

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);

		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneBase::RenderMeshOnScreen(Mesh * mesh, float x, float y, float sizex, float sizey, float rotate)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(x, y, 0.1f);
	//modelStack.Rotate(rotate, 0, 0, 1);
	modelStack.Scale(sizex, sizey, 1.f);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
