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
	highscore = LoadCSV("CSV//highscore.csv");
	QuickSort(&highscore, 1, highscore.size() - 1);
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

bool SceneHighScore::CheckCollision(GameObject *go1, GameObject *go2, float dt)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 vel = go1->vel - go2->vel;
		float r1 = go1->scale.x;
		float r2 = go2->scale.x;

		return vel.Dot(dis) < 0 &&
			dis.LengthSquared() < (r1 + r2) * (r1 + r2);
	}
	case GameObject::GO_WALL:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->dir;
		Vector3 NP = N.Cross(Vector3(0, 0, 1));
		float l = go2->scale.y * 0.7;
		float r = go1->scale.x * 0.7;
		float h = go2->scale.x * 0.7;
		if ((w0 - b1).Dot(N) < 0)
			N = -N;

		return go1->vel.Dot(N) > 0 && (abs((w0 - b1).Dot(N)) < r + h * 0.5f) &&
			(abs((w0 - b1).Dot(NP)) < r + l * 0.5f);
	}
	case GameObject::GO_BLOCKS:
	{
		Vector3 dis = go1->pos - go2->pos;
		Vector3 vel = go1->vel - go2->vel;
		float r1 = go1->scale.x * 0.65;
		float r2 = go2->scale.x * 0.65;

		if (vel.Dot(dis) < 0 && dis.LengthSquared() < (r1 + r2) * (r1 + r2))
		{
			std::cout << "THis is being collided with!" << std::endl;
			return true;
		}
		return false;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 p1 = go1->pos;
		Vector3 p2 = go2->pos;
		float r1 = go1->scale.x;
		float r2 = go2->scale.x;

		return ((p2 - p1).LengthSquared() < (r1 + r1) * (r1 + r2)) &&
			((p2 - p1).LengthSquared() > 0.0f) &&
			((p2 - p1).Dot(go1->vel) > 0.0f);
	}
	case GameObject::GO_CANNON_PLATFORM:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->dir;
		Vector3 NP = N.Cross(Vector3(0, 0, 1));
		float l = go2->scale.y;
		float r = go1->scale.x;
		float h = go2->scale.x;
		if ((w0 - b1).Dot(N) < 0)
			N = -N;

		return go1->vel.Dot(N) > 0 && (abs((w0 - b1).Dot(N)) < r + h * 0.5f) &&
			(abs((w0 - b1).Dot(NP)) < r + l * 0.5f);
	}
	}
}

float SceneHighScore::CheckCollision2(GameObject *go1, GameObject *go2)
{
	Vector3 rel = go1->vel - go2->vel;
	Vector3 dir = go1->pos - go2->pos;
	float r = go1->scale.x + go2->scale.x;
	if (rel.Dot(dir) > 0)
		return -1;

	float a = rel.Dot(rel);
	float b = 2 * rel.Dot(dir);
	float c = dir.Dot(dir) - r * r;
	float d = b * b - 4 * a * c;
	if (d < 0)
		return -1;
	float t = (-b - sqrt(d)) / (2 * a);
	if (t < 0)
		t = (-b - sqrt(d)) / (2 * a);
	return 0;
}

void SceneHighScore::CollisionResponse(GameObject * go1, GameObject * go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 u1 = go1->vel;
		Vector3 u2 = go2->vel;
		Vector3 N = (go2->pos - go1->pos).Normalize();
		Vector3 u1N = u1.Dot(N) * N;
		Vector3 u2N = u2.Dot(N) * N;
		go1->vel = u1 + ((2 * m2) / (m1 + m2))* (u2N - u1N);
		go2->vel = u2 + ((2 * m2) / (m1 + m2)) * (u1N - u2N);
		go1->vel *= 0.85;
		go2->vel *= 0.85;

		break;
	}
	case GameObject::GO_WALL:
	{
		float mag = go1->vel.Length();
		std::cout << mag << std::endl;
		Vector3 vel = go1->vel;
		Vector3 N = go2->dir;
		go1->vel = vel - (2.f * vel.Dot(N)) * N;

		break;
	}
	case GameObject::GO_BLOCKS:
	{
		float mag = go1->vel.Length();
		std::cout << mag << std::endl;
		Vector3 vel = go1->vel;
		Vector3 N = go2->dir;

		float closex, closey;
		int side = 0;
		Vector3 norm = (0, 0, 0);
		closex = go1->pos.x;
		closey = go1->pos.y;
		if (closex < go2->pos.x) { closex = go2->pos.x; side = 1; }
		if (closex > go2->pos.x + go2->scale.x * 1) { closex = go2->pos.x + go2->scale.x * 1; side = 2; }

		if (closey < go2->pos.y) { closey = go2->pos.y; side = 3; }
		if (closey > go2->pos.y + go2->scale.y * 1) { closey = go2->pos.y + go2->scale.y * 1; side = 4; }

		if (side == 1)
		{
			norm = (-1, 0, 0);
			std::cout << "Left!" << std::endl;
		}
		else if (side == 2)
		{
			std::cout << "Right!" << std::endl;
			norm = (1, 0, 0);
		}
		else if (side == 3)
		{
			std::cout << "Bottom!" << std::endl;
			norm = (0, 1, 0);
		}
		else
		{
			std::cout << "Top!" << std::endl;
			norm = (0, -1, 0);
		}
		go1->vel = (vel - (2.f * vel.Dot(N)) * N);
		if (side == 1 || side == 2)
			go1->vel.x = -go1->vel.x;
		if (side == 3 || side == 4)
			go1->vel.y = -go1->vel.y;
		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 vel = go1->vel;
		Vector3 N = (go2->pos - go1->pos).Normalize();
		go1->vel = vel - (2.f * vel.Dot(N)) * N;
		go1->vel *= 0.85;

		break;
	}
	case GameObject::GO_CANNON_PLATFORM:
	{
		Vector3 vel = go1->vel;
		Vector3 N = go2->dir;
		go1->vel = vel - (2.f * vel.Dot(N)) * N;
		break;
	}
	}
}

void SceneHighScore::Update(double dt)
{
	SceneBase::Update(dt);
	pressDelay += (float)dt;

	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	//Mouse Section
	static bool bLButtonState = false;

	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x) / w * m_worldWidth;
	float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

	int h_temp = 100;
	int w_temp = 100 * Application::GetWindowWidth() / Application::GetWindowHeight();

	//Position values (for arrows)
	float posXArrow_Delete = 1.35f;
	float posXArrow_Exit = 0.575f;
	float posYArrow = 5.1f;

	//Scale values (for upgrade menu)
	float scaleDown_Arrow = 20.f;

	if (!bLButtonState && Application::IsMousePressed(0))
	{
	}

	else if (bLButtonState && !Application::IsMousePressed(0))
	{
	}

	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
	}

	{	//Inserting and Deleting Data

		//Prevent pressDelay from exceeding 0.5f
		if (pressDelay > 0.5f)
			pressDelay = 0.5f;

		if (Application::IsKeyPressed('Z') && pressDelay >= cooldownPressed)
		{
			highscore = DeleteCSV(file_path);

			pressDelay = 0.f;
		}

		if (Application::IsKeyPressed(VK_SPACE) && pressDelay >= cooldownPressed)
		{
			//highscore = DeleteCSV(file_path);
			HighScore temp;
			temp.Data.rank = "4";
			temp.Data.name = "GAYY";
			temp.Data.score = "1234";
			highscore.push_back(temp);
			QuickSort(&highscore, 1, highscore.size() - 1);
			WriteCSV(file_path, highscore);

			pressDelay = 0.f;
		}
	}
	//Physics Simulation Section
	dt *= m_speed;

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			//if (go->type == GameObject::GO_BALL)
			if (go->type == GameObject::GO_CUBE)
			{
				go->pos += go->vel * static_cast<float>(dt);
				go->vel += gravity * dt;
				if (go->pos.x > m_worldWidth + go->scale.x || go->pos.x < -go->scale.x || go->pos.y > m_worldHeight + go->scale.y || go->pos.y < -go->scale.y)
				{
					go->active = false;
					--m_objectCount;
				}
			}

			for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject *go2 = static_cast<GameObject *>(*it2);
				if (!go2->active)
					continue;
				//if (go->type != GameObject::GO_BALL && go2->type != GameObject::GO_BALL)
				if (go->type != GameObject::GO_CUBE && go2->type != GameObject::GO_CUBE)
					continue;

				GameObject *goA, *goB;

				//if(go->type == GameObject::GO_BALL)
				if (go->type == GameObject::GO_CUBE)
				{
					goA = go;
					goB = go2;
				}
				else
				{
					goA = go2;
					goB = go;
				}

				if (CheckCollision(goA, goB, dt))
				{
					m1 = goA->mass;
					m2 = goB->mass;
					u1 = goA->vel;
					u2 = goB->vel;

					CollisionResponse(goA, goB);

					v1 = goA->vel;
					v2 = goB->vel;

					break;
				}
			}
		}
	}

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
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CUBE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BLOCKS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PILLAR:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		angle = Math::RadianToDegree(atan2(go->dir.y, go->dir.x));
		modelStack.Rotate(angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CANNON:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.PushMatrix();
		modelStack.Translate(0, 2, 2);
		RenderMesh(meshList[GEO_CANNON], true, go->Color);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CANNON_PLATFORM:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CANNON_PLATFORM], true, go->Color);
		modelStack.PopMatrix();
		break;
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
	for (int check_index = 1, sizer = 0; check_index < highscore.size(); ++check_index)
	{
		ss << highscore[check_index];
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), textsize, 18, 40 - sizer);
		ss.str("");
		sizer += 3;
	}

}

void SceneHighScore::Exit()
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
