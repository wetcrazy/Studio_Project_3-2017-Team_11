#include "SceneHighScore.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadCSV.h"
#include "QuickSort.h"
#include <sstream>

SceneHighScore::SceneHighScore()
{
}

SceneHighScore::~SceneHighScore()
{
}

void SceneHighScore::Init()
{
	SceneBase::Init();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;
	initialKE = 0;
	finalKE = 0;

	m_ghost01 = new GameObject(GameObject::GO_BALL);
	m_ghost02 = new GameObject(GameObject::GO_CUBE);
	gravity.Set(0.0f, -9.8f, 0.0f);

	// Cannon stuff
	ft_shootTime = 0.f;
	ft_elapsedTime = 0.f;
	b_shootIsTrue = false;

	CreateStuff();
	highscore = LoadCSV("CSV//highscore.csv");
	QuickSort(&highscore, 1, highscore.size()-1);
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
	ft_elapsedTime += dt;

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
	background->pos.Set(w_temp / 2, h_temp / 2, -5);
	background->scale.Set(w_temp + 2, h_temp, 1);

	if (posY > cannon->pos.y)        // Cannon cannot move when cursor is below cannon	
	if (!b_shootIsTrue)
	{		
		{
			if (!b_shootIsTrue)
			{
				aim.Set(posX, posY, 0);	
				aim.Set(aim.x - platform->pos.x, Math::Clamp(aim.y, platform->pos.y, 90 - platform->pos.y), 0);
				cannon->dir = aim.Cross(Vector3(0, 0, 1));
				cannon->dir.Normalize();
			}
		}
	}
	//std::cout << platform->pos.y << std::endl;

	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		m_ghost01->pos.Set(posX, posY, 0); //IMPT
		m_ghost01->active = true;
		m_ghost01->active = false;
		float sc = 2;
		m_ghost01->scale.Set(sc, sc, sc);
	}

	else if (bLButtonState && !Application::IsMousePressed(0) && ft_elapsedTime > ft_shootTime)
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		if (posY > cannon->pos.y)
		{
		//spawn small GO_BALL
		GameObject *go = FetchGO();
		go->active = true;
		go->type = GameObject::GO_CUBE;
		go->pos = platform->pos;
		go->pos += aim.Normalized() * 0.5;
		go->vel = aim;

		if (go->vel.Length() > 50) // 50 is distance
		{
			go->vel.Normalize();
			go->vel *= 100;	// Speed of cannon shooting
		}
		if (go->vel.y < 0)
			go->vel.y *= -1;
		go->scale.Set(2, 2, 2);
		m_ghost01->active = false;

		// Limit spawn rate of cannon balls AND prevents movement of cannon immediately after shooting
		ft_shootTime = ft_elapsedTime + 0.25f;

		// Cannon ball has been shot
		b_shootIsTrue = true;

		// Randomize color of ball
		go->Color.Set(Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1));

		}
	}

	// Cannon ball has not been shot
	else if (ft_elapsedTime > ft_shootTime)
		b_shootIsTrue = false;

	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		m_ghost02->pos.Set(posX, posY, 0); //IMPT
		m_ghost02->active = true;
		float sc = 3;
		m_ghost02->scale.Set(sc, sc, sc);
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//spawn large GO_BALL
		GameObject *go = FetchGO();
		go->active = true;
		go->type = GameObject::GO_CUBE;

		go->pos = m_ghost02->pos;
		go->vel.Set(m_ghost02->pos.x - posX, m_ghost02->pos.y - posY, 0);
		m_ghost02->active = false;
		float sc = 3;
		go->scale.Set(sc, sc, sc);
		go->mass = sc * sc * sc;

		// Randomize color of ball
		go->Color.Set(Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1));
	}

	if (Application::IsKeyPressed(VK_SPACE))
	{
		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			//if (go->active && go->type == GameObject::GO_BALL)
			if (go->active && go->type == GameObject::GO_CUBE)
				go->active = false;

			m_objectCount = 0;
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
}

void SceneHighScore::CreateStuff()
{
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float w_temp = 133;
	float h_temp = 100;

	GameObject *wall = FetchGO();

	wall->type = GameObject::GO_WALL;	// Left Wall
	wall->active = true;
	wall->dir.Set(1, 0, 0);
	wall->pos.Set(wall->scale.x / 2, wall->scale.y, 0);
	wall->scale.Set(2, h / 27, 1);
	wall->Color.Set(0.486, 0.988, 0);

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;	// Ground
	wall->active = true;
	wall->dir.Set(0, -1, 0);
	wall->pos.Set(133 / 2, wall->scale.y / 2, 0);
	wall->scale.Set(2, w, 1);
	wall->Color.Set(0.486, 0.988, 0);

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;	// Elevated Ground
	wall->active = true;
	wall->dir.Set(0, -1, 0);
	wall->pos.Set(133 / 8, 100 / 9, 0);
	wall->scale.Set(2, 100 / 3, 1);
	wall->Color.Set(0.486, 0.988, 0);

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;	// Slanted Ground
	wall->active = true;
	wall->dir.Set(-1, -1, 0);
	wall->pos.Set(133 / 5 + 11.5, 100 / 9 - 5.3, 0);
	wall->scale.Set(2, 100 / 6, 1);
	wall->Color.Set(0.486, 0.988, 0);

	GameObject* pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;	// Pillar for Elevated Ground
	pillar->active = true;
	pillar->pos.Set(133 / 8 + 16, 100 / 9, 0);
	pillar->scale.Set(1.4, 1.4, 1.4);
	pillar->Color.Set(0.486, 0.988, 0);

	background = new GameObject(GameObject::GO_BACKGROUND);	// Background
	background->active = true;
	background->pos.Set(w_temp / 2, h_temp / 2, -5);
	background->scale.Set(w_temp, h_temp, 1);
	m_goList.push_back(background);
	
	platform = new GameObject(GameObject::GO_CANNON_PLATFORM);	// Platform for Cannon
	platform->active = true;
	platform->dir.Set(0, 1, 0);
	platform->pos.Set(133 / 8, 100 / 6.8, 0);
	platform->scale.Set(1, 3, 1);
	m_goList.push_back(platform);

	cannon = new GameObject(GameObject::GO_CANNON);	// Cannon
	cannon->active = true;
	cannon->pos = platform->pos;
	cannon->scale.Set(1, 2.5, 1);
	m_goList.push_back(cannon);

	{
		GameObject* pillar = FetchGO();
		pillar->type = GameObject::GO_BLOCKS;	// Pillar for Elevated Ground
		pillar->active = true;
		pillar->pos.Set(133 / 2 + 16, 100 / 2 + 16, 0);
		pillar->scale.Set(3, 3, 3);
		pillar->Color.Set(1, 1, 1);
		std::cout << "This is spawned!" << std::endl;
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
	case GameObject::GO_BACKGROUND:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BACKGROUND], true, go->Color);
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
	if (m_ghost01->active)
		RenderGO(m_ghost01);
	if (m_ghost02->active)
		RenderGO(m_ghost02);

	//On screen text
	float textsize = 3;
	std::ostringstream ss;
	for (int check_index = 0,sizer=0; check_index < highscore.size(); ++check_index)
	{
		ss << highscore[check_index];
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), textsize, 0, 57 - sizer);
		ss.str("");
		sizer+=3;
	}



	//if (Application::IsKeyPressed(VK_RETURN))
	//{
	//	//On screen text
	//	std::ostringstream ss;
	//	ss << "Object count: " << m_objectCount;
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	//	ss.str(std::string());
	//	ss.precision(5);
	//	ss << "Initial momentum: " << initialMomentum;
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

	//	ss.str(std::string());
	//	ss.precision(5);
	//	ss << "Final momentum: " << finalMomentum;
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);

	//	//Exercise 3: render initial and final kinetic energy
	//	ss.str(std::string());
	//	ss.precision(5);
	//	ss << "Initial KE: " << initialKE;
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 21);

	//	ss.str(std::string());
	//	ss.precision(5);
	//	ss << "Final KE: " << finalKE;
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 18);

	//	ss.str(std::string());
	//	ss.precision(3);
	//	ss << "Speed: " << m_speed;
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	//	ss.str(std::string());
	//	ss.precision(5);
	//	ss << "FPS: " << fps;
	//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	//	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
	//}
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
	if (m_ghost01)
	{
		delete m_ghost01;
		m_ghost01 = NULL;
	}
	if (m_ghost02)
	{
		delete m_ghost02;
		m_ghost02 = NULL;
	}
}
