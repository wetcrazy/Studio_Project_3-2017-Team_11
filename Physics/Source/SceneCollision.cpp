#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SceneManager.h"
// #include "LoadTXT.h"

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
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

	// Upgrades Menu
	b_upgradesMenu_IsOpen = false;
	b_upgrades1 = false;
	b_upgrades2 = false;

	b_isBulletAlive = false;
	ft_bulletAlive = 0;
	m_objRestrict = 18;

	//LoadTXT loadtxt;

	CreateStuff();
}

GameObject* SceneCollision::FetchGO()
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

bool SceneCollision::CheckCollision(GameObject *go1, GameObject *go2, float dt)
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
	case GameObject::GO_GROUND:
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
	case GameObject::GO_PLATFORM:
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
		float r1 = go1->scale.x * 0.9f; // raw inputs
		float r2 = go2->scale.x * 0.9f; // raw inputs

		float r3 = go2->scale.y * 0.5f; // raw inputs

		if (vel.Dot(dis) < 0 && dis.LengthSquared() < (r1 + r2) * (r1 + r3))
		{
			std::cout << "This is being collided with!" << std::endl;
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

float SceneCollision::CheckCollision2(GameObject *go1, GameObject *go2)
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

void SceneCollision::CollisionResponse(GameObject * go1, GameObject * go2)
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
	case GameObject::GO_CUBE:
	{
		float mag = go1->vel.Length();
		Vector3 vel = go1->vel;
		Vector3 N = go2->dir;
		go1->vel = vel - (2.f * vel.Dot(N)) * N;

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
	case GameObject::GO_GROUND:
	{
		float mag = go1->vel.Length();
		std::cout << mag << std::endl;
		Vector3 vel = go1->vel;
		Vector3 N = go2->dir;
		go1->vel = vel - (2.f * vel.Dot(N)) * N;

		break;
	}
	case GameObject::GO_PLATFORM:
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
		Vector3 u1 = go1->vel;
		Vector3 u2 = go2->vel;
		Vector3 N = (go2->pos - go1->pos).Normalize();
		Vector3 u1N = u1.Dot(N) * N;
		Vector3 u2N = u2.Dot(N) * N;
		go1->vel = u1 + ((2 * m2) / (m1 + m2))* (u2N - u1N);
		go2->vel = u2 + ((2 * m2) / (m1 + m2)) * (u1N - u2N);
		go1->active = false;
		go2->active = false;
		m_objectCount--;
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

void SceneCollision::Update(double dt)
{
	std::cout << "Object Count : " << m_objectCount << std::endl;
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

	//Background resizing according to window size
	background->pos.Set(w_temp / 2, h_temp / 2, -5);
	background->scale.Set(w_temp + 2, h_temp, 1);

	//Position values (for upgrade menu)
	float posXDownArrow = 0.7f;
	float posXDownArrow_NextLevel = 1.1f;
	float posXDownArrow_NextPage = 0.59f;
	float posYDownArrow_Upgrade1 = 0.86f;
	float posYDownArrow_Upgrade2 = 1.36f;
	float posYDownArrow_NextPage_NextLevel = 3.7f;

	//Scale values (for upgrade menu)
	float scaleDown_Arrow = 20.f;

	if (posY > cannon->pos.y)        // Cannon cannot move when cursor is below cannon	
	{
		if (!b_shootIsTrue)
		{
			aim.Set(posX, posY, 0);
			aim.Set(aim.x - platform->pos.x, aim.y - platform->pos.y, 0);
			cannon->dir = aim.Cross(Vector3(0, 0, 1));
			cannon->dir.Normalize();
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
			if (m_objectCount < m_objRestrict)
				go->active = true;
			else
			{
				go->active = false;
				m_objectCount--;
			}
			go->type = GameObject::GO_CUBE;
			go->pos = platform->pos;
			go->pos += aim.Normalized() * 0.5;
			go->vel = aim;

			if (go->vel.Length() > 50) // 50 is distance
			{
				go->vel.Normalize();

			if (!b_upgrades1 && !b_upgrades2)
			{
				go->vel *= 50;	// Default Speed of cannon shooting
			}
			if (b_upgrades1)
			{
				go->vel *= 60;	// Speed of cannon shooting (upgrade 1)
			}
			if (b_upgrades2)
			{
				go->vel *= 70;	// Speed of cannon shooting (upgrade 2)
			}
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
			if (go->type == GameObject::GO_BLOCKS)
			{
				go->pos += go->vel * static_cast<float>(dt);
				if (!go->vel.IsZero())
					go->vel += (Vector3(0, 0, 0) - go->vel) * dt;
			}
			for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject *go2 = static_cast<GameObject *>(*it2);
				if (!go2->active)
					continue;
				//if (go->type != GameObject::GO_BALL && go2->type != GameObject::GO_BALL)
				/*if (go->type != GameObject::GO_CUBE && go2->type != GameObject::GO_CUBE)
				continue;*/

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
					//v1 = goA->vel;
					//v2 = goB->vel;
					break;
				}
			}
		}
	}

	// Opening Upgrades Menu
	if (Application::IsKeyPressed('P'))
	{
		b_upgradesMenu_IsOpen = true;
	}

	// If upgrades menu is open
	if (b_upgradesMenu_IsOpen)
	{
		RenderUpgradesMenu(dt);

		platform->active = false;
		cannon->active = false;

		switch (selectOptions)
		{
		case GRADE_1:
			//Render in arrow 
			arrows->active = true;
			arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Upgrade1, 1);
			arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

			//Render in upgrades menu
			upgradesMenu->active = true;
			upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
			upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
			break;

		case GRADE_2:
			//Render in arrow
			arrows->pos.Set((w_temp / 2) / posXDownArrow, (h_temp / 2) / posYDownArrow_Upgrade2, 1);
			arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

			//Render in upgrades menu
			upgradesMenu->active = true;
			upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
			upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
			break;

		case NEXTLEVEL:
			//Render in arrow
			arrows->pos.Set((w_temp / 2) / posXDownArrow_NextLevel, (h_temp / 2) / posYDownArrow_NextPage_NextLevel, 1);
			arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

			//Render in upgrades menu
			upgradesMenu->active = true;
			upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
			upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
			break;

		case NEXTPAGE:
			//Render in arrow
			arrows->pos.Set((w_temp / 2) / posXDownArrow_NextPage, (h_temp / 2) / posYDownArrow_NextPage_NextLevel, 1);
			arrows->scale.Set((w_temp + 2) / scaleDown_Arrow, h_temp / scaleDown_Arrow, 1);

			//Render in upgrades menu
			upgradesMenu->active = true;
			upgradesMenu->pos.Set(w_temp / 2, h_temp / 2, -5);
			upgradesMenu->scale.Set(w_temp + 2, h_temp, 1);
			break;
		}

		//If player has bought the first upgrade
		if (b_upgrades1)
		{
			//Render in upgrade1
			upgradesMenu->active = false;
			highlight_grade2->active = false;
			highlight_grade1->active = true;
			highlight_grade1->pos.Set(w_temp / 2, h_temp / 2, -5);
			highlight_grade1->scale.Set(w_temp + 2, h_temp, 1);
		}
		//If player has bought the second upgrade
		if (b_upgrades2)
		{
			//Render in upgrade2
			upgradesMenu->active = false;
			highlight_grade1->active = false;
			highlight_grade2->active = true;
			highlight_grade2->pos.Set(w_temp / 2, h_temp / 2, -5);
			highlight_grade2->scale.Set(w_temp + 2, h_temp, 1);
		}
	}
	// If upgrades menu is not open
	if (!b_upgradesMenu_IsOpen)
	{
		platform->active = true;
		cannon->active = true;

		arrows->active = false;
		highlight_grade1->active = false;
		highlight_grade2->active = false;
		upgradesMenu->active = false;

		//Reset default option
		selectOptions = GRADE_1;
	}
}

void SceneCollision::CreateStuff()
{
	float w = Application::GetWindowWidth();
	float h = Application::GetWindowHeight();

	float w_temp = 133;
	float h_temp = 100;

	GameObject *wall = FetchGO();
	wall->type = GameObject::GO_PLATFORM;	// Left Wall
	wall->active = true;
	wall->dir.Set(1, 0, 0);
	wall->pos.Set(wall->scale.x / 2, wall->scale.y, 0);
	wall->scale.Set(2, h / 27, 1);

	wall = FetchGO();
	wall->type = GameObject::GO_GROUND;	// Ground
	wall->active = true;
	wall->dir.Set(0, -1, 0);
	wall->pos.Set(133 / 2, wall->scale.y / 2, 0);
	wall->scale.Set(2, w, 1);

	wall = FetchGO();
	wall->type = GameObject::GO_PLATFORM;	// Elevated Ground
	wall->active = true;
	wall->dir.Set(0, -1, 0);
	wall->pos.Set(133 / 8, 100 / 9, 0);
	wall->scale.Set(2, 100 / 3, 1);

	wall = FetchGO();
	wall->type = GameObject::GO_PLATFORM;	// Slanted Ground
	wall->active = true;
	wall->dir.Set(-1, -1, 0);
	wall->pos.Set(133 / 5 + 11.5, 100 / 9 - 5.3, 0);
	wall->scale.Set(2, 100 / 6, 1);

	for (unsigned i = 0; i < 6; i++)
	{
		wall = FetchGO();
		wall->type = GameObject::GO_PLATFORM;	// More Elevated Grounds
		wall->active = true;
		wall->dir.Set(0, -1, 0);
		wall->pos.Set(133 / 8 + (1 * i), 100 / 9 - (2 * i), 0);
		wall->scale.Set(2, 100 / 3 + (1.5 * i), 1);
	}

	{	//Upgrades Menu
		arrows->type = GameObject::GO_ARROW;
		arrows->active = true;
		arrows->pos.Set(-10, -10, 1);
		arrows->scale.Set(1, 1, 1);

		highlight_grade1->type = GameObject::GO_GRADE1;
		highlight_grade1->active = true;
		highlight_grade1->pos.Set(-10, -10, 1);
		highlight_grade1->scale.Set(1, 1, 1);

		highlight_grade2->type = GameObject::GO_GRADE2;
		highlight_grade2->active = true;
		highlight_grade2->pos.Set(-10, -10, 1);
		highlight_grade2->scale.Set(1, 1, 1);

		upgradesMenu->type = GameObject::GO_UPGRADESMENU;
		upgradesMenu->active = true;
		upgradesMenu->pos.Set(-10, -10, -5);
		upgradesMenu->scale.Set(1, 1, 1);
	}


	background = new GameObject(GameObject::GO_BACKGROUND);	// Background
	background->active = true;
	background->pos.Set(w_temp / 2, h_temp / 2, -5);
	background->scale.Set(w_temp, h_temp, 1);
	m_goList.push_back(background);

	{
		//Cannon
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
	}

	{ // Testing Structure
		GameObject* blocks = FetchGO();
		blocks->type = GameObject::GO_BLOCKS;	// Vertical
		blocks->active = true;
		blocks->dir.Set(1, 0, 0);
		blocks->pos.Set(133 / 2 + 16, blocks->scale.y + 5, 0);
		blocks->scale.Set(2, 8, 1);
		blocks->Color.Set(0.8, 0.8, 0);

		blocks = FetchGO();
		blocks->type = GameObject::GO_BLOCKS;	// Vertical
		blocks->active = true;
		blocks->dir.Set(1, 0, 0);
		blocks->pos.Set(133 / 2 + 26, blocks->scale.y + 5, 0);
		blocks->scale.Set(2, 8, 1);
		blocks->Color.Set(0.8, 0.8, 0);

		blocks = FetchGO();
		blocks->type = GameObject::GO_BLOCKS;	// Horizontal
		blocks->active = true;
		blocks->dir.Set(0, 1, 0);
		blocks->pos.Set(133 / 2 + 21, blocks->scale.y + 7.5, 0);
		blocks->scale.Set(2, 8, 1);
		blocks->Color.Set(0.8, 0.8, 0);
		//
		blocks = FetchGO();
		blocks->type = GameObject::GO_BLOCKS;	// Vertical
		blocks->active = true;
		blocks->dir.Set(1, 0, 0);
		blocks->pos.Set(133 / 2 + 6, blocks->scale.y + 8, 0);
		blocks->scale.Set(2, 14, 1);
		blocks->Color.Set(0.8, 0.8, 0);

		blocks = FetchGO();
		blocks->type = GameObject::GO_BLOCKS;	// Vertical
		blocks->active = true;
		blocks->dir.Set(1, 0, 0);
		blocks->pos.Set(133 / 2 + 36, blocks->scale.y + 8, 0);
		blocks->scale.Set(2, 14, 1);
		blocks->Color.Set(0.8, 0.8, 0);

		blocks = FetchGO();
		blocks->type = GameObject::GO_BLOCKS;	// Horizontal
		blocks->active = true;
		blocks->dir.Set(0, 1, 0);
		blocks->pos.Set(133 / 2 + 21, blocks->scale.y + 16.5, 0);
		blocks->scale.Set(2, 31.2, 1);
		blocks->Color.Set(0.8, 0.8, 0);

		std::cout << "This is spawned!" << std::endl;
	}
}

void SceneCollision::CreateLevel()
{

}

void SceneCollision::RenderUpgradesMenu(double dt)
{
	pressDelay += (float)dt;

	//Prevent pressDelay from exceeding 0.5f
	if (pressDelay > 0.5f)
		pressDelay = 0.5f;

	if ((Application::IsKeyPressed(VK_UP)) && pressDelay >= cooldownPressed)
	{
		if (selectOptions == NEXTPAGE)
			selectOptions = NEXTLEVEL;

		else if (selectOptions == NEXTLEVEL)
			selectOptions = GRADE_2;

		else if (selectOptions == GRADE_2)
			selectOptions = GRADE_1;

		else if (selectOptions == GRADE_1)
			selectOptions = NEXTPAGE;

		pressDelay = 0.f;
	}

	if ((Application::IsKeyPressed(VK_DOWN)) && pressDelay >= cooldownPressed)
	{
		if (selectOptions == GRADE_1)
			selectOptions = GRADE_2;

		else if (selectOptions == GRADE_2)
			selectOptions = NEXTLEVEL;

		else if (selectOptions == NEXTLEVEL)
			selectOptions = NEXTPAGE;

		else if (selectOptions == NEXTPAGE)
			selectOptions = GRADE_1;

		pressDelay = 0.f;
	}

	if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= cooldownPressed)
	{
		switch (selectOptions)
		{
		case GRADE_1:
		{
			b_upgrades1 = true;
			break;
		}
		case GRADE_2:
		{
			b_upgrades2 = true;
			break;
		}
		case NEXTPAGE:
		{
			upgradesMenu->active = false;
			b_upgradesMenu_IsOpen = false;
			break;
		}
		case NEXTLEVEL:
		{
			upgradesMenu->active = false;
			b_upgradesMenu_IsOpen = false;
			break;
		}
		}

		pressDelay = 0.f;
	}
}

void SceneCollision::RenderGO(GameObject *go)
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
		angle = Math::RadianToDegree(atan2(go->dir.y, go->dir.x));
		modelStack.Rotate(angle, 0, 0, 1);
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
	case GameObject::GO_GROUND:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		angle = Math::RadianToDegree(atan2(go->dir.y, go->dir.x));
		modelStack.Rotate(angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_GROUND], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PLATFORM:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		angle = Math::RadianToDegree(atan2(go->dir.y, go->dir.x));
		modelStack.Rotate(angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PLATFORM], true, go->Color);
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
	case GameObject::GO_UPGRADESMENU:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_UPGRADEMENU], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARROW:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ARROW], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_GRADE1:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_GRADE_1], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_GRADE2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_GRADE_2], true, go->Color);
		modelStack.PopMatrix();
		break;
	}
}

void SceneCollision::Render()
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

void SceneCollision::Exit()
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
