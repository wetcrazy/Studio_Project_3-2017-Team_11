#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

#include "SceneManager.h"
#include "SceneMainMenu.h"
#include "SceneUpgrade.h"

#include "SpriteAnimation.h"
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

	//Upgrades
	upgraded.ReadFile("Text//Speed_Upgrade.txt");

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;
	fortCount = 0;
	initialKE = 0;
	finalKE = 0;

	i_projectileType = 3;

	if (i_projectileType == 1)
		m_ghost = new GameObject(GameObject::GO_BALL);
	else if (i_projectileType == 2)
		m_ghost = new GameObject(GameObject::GO_CUBE);
	else if (i_projectileType == 3)
		m_ghost = new GameObject(GameObject::GO_HEXA);

	gravity.Set(0.0f, -9.8f, 0.0f);

	b_isBulletAlive = false;
	ft_bulletAlive = 0;

	//Scrolling
	projectile = FetchGO();
	projectile->active = false;
	launched = 0.f;
	scrollOffset = 0.f;

	// Power Bar
	NumMode_tiggered_powerbar = 1;
	original_position_powerbar = 0;
	is_movement_powerbar = true;

	//LoadTXT loadtxt;

	i_CurrentLevel = GetCurrentLevel();
	i_tempScore = GetScore();
	i_tempCurrency = GetCurrency();

	CreateStuff();
	CreateLevel(i_CurrentLevel);
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
	for (unsigned i = 0; i < 40; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_HEXA);
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
	case GameObject::GO_HEXA:
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
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;

		float r = go1->scale.x;
		float h = go2->scale.x;
		float l = go2->scale.y;

		Vector3 N = go2->dir;
		Vector3 NP = go2->dir.Cross(Vector3(0, 0, 1));

		Vector3 relativePos = b1 - w0;
		if (relativePos.Dot(N) > 0)
			N = -N;

		return (go1->vel.Dot(N) > 0 && 
			((abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l * 0.5f)
				||
			(abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l * 0.5f)));
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
	case GameObject::GO_HEXA:
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
		//std::cout << mag << std::endl;
		Vector3 vel = go1->vel;
		Vector3 N = go2->dir;
		go1->vel = vel - (2.f * vel.Dot(N)) * N;

		break;
	}
	case GameObject::GO_GROUND:
	{
		float mag = go1->vel.Length();
		//std::cout << mag << std::endl;
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
		fortCount--;
		i_tempScore += 50;
		i_tempCurrency += 50;
		NumMode_tiggered_powerbar = 1;
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
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	//std::cout << "Count : " << m_objectCount << " / " << m_objRestrict << std::endl;
	SceneBase::Update(dt);

	SetTempScore(i_tempScore);
	SetTempCurrency(i_tempCurrency);

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

	//Cannon follows cursor position
	if (posY > cannon->pos.y)        // Cannon cannot move when cursor is below cannon	
	{
		//if (!projectile->active)
		if (NumMode_tiggered_powerbar == 1)
		{
			//Cannon follow cursor
			aim.Set(posX + launched, posY, 0);
			aim.Set(aim.x - platform->pos.x, aim.y - platform->pos.y, 0);
			cannon->dir = aim.Cross(Vector3(0, 0, 1));
			cannon->dir.Normalize();

			powerbar->pos.x = original_position_powerbar; // Reset pos of powerbar

			guidemarker->scale.y = 1;
			guidemarker->dir = cannon->dir;
		}		
		// std::cout << guidemarker->pos << std::endl;
	}

	//if (!bLButtonState && Application::IsMousePressed(0) && !projectile->active)
	// powerbar->pos.x = original_position_powerbar; // Update the pos of power bars (do not need this cause i updating the pos of power bar directly)

	// Logic for the aim -> hold -> fire
	if (!projectile->active && NumMode_tiggered_powerbar == 3)
	{
		bLButtonState = true;
		std::cout << "LBUTTON UP" << std::endl;

		if (posY > cannon->pos.y)
		{
			projectile->active = true;
			//
			if (i_projectileType == 1)
				projectile->type = GameObject::GO_BALL;
			else if (i_projectileType == 2)
				projectile->type = GameObject::GO_CUBE;
			else if (i_projectileType == 3)
				projectile->type = GameObject::GO_HEXA;
			//

			//Where projectile will shoot from
			projectile->pos = platform->pos;
			projectile->pos += aim.Normalized() * 0.5;
			projectile->vel = aim;

			// Maths to caulate speed multiplyer
			float speed_multiplyer = (1 * (powerbar->pos.x / (powerrange->scale.x/2)));
			std::cout << speed_multiplyer << std::endl; // Debug info for speed_multiplyer
			if (projectile->vel.Length() > 10) // 10 is distance
			{
				int speed = 35;

				if (upgraded.speed_upgrade == 1)
				{
					speed = 45;
				}

				else if (upgraded.speed_upgrade == 2)
				{
					speed = 55;
				}

				projectile->vel.Normalize();
				projectile->vel *= speed * speed_multiplyer;
			}

			if (projectile->vel.y < 0)
				projectile->vel.y *= -1;

			m_ghost->active = false;
			projectile->scale.Set(2, 2, 2);

			// Randomize color of ball
			projectile->Color.Set(Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1));
		}
	}
	else if (!bLButtonState && Application::IsMousePressed(0) && NumMode_tiggered_powerbar == 1)
	{
		bLButtonState = true;
		std::cout << "LBUTTON UP" << std::endl;

		NumMode_tiggered_powerbar = 2;
	}
	else if (!bLButtonState && Application::IsMousePressed(0) && NumMode_tiggered_powerbar == 2)
	{
		bLButtonState = true;
		std::cout << "LBUTTON UP" << std::endl;

		//NumMode_tiggered_powerbar = 3;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON DOWN" << std::endl;
		NumMode_tiggered_powerbar = 3;
	}
	if (NumMode_tiggered_powerbar == 2)
	{
		if (!is_movement_powerbar)
		{
			powerbar->pos.x -= 0.5f;
			guidemarker->scale.y -= 0.25f;
		}
		else if (is_movement_powerbar)
		{
			powerbar->pos.x += 0.5f;
			guidemarker->scale.y += 0.25f;
		}
		if (powerbar->pos.x <= 0)
		{
			is_movement_powerbar = true;
		}
		else if (powerbar->pos.x >= powerrange->scale.x / 2)
		{
			is_movement_powerbar = false;
		}
	}

	// std::cout << original_position_powerbar << std::endl; // Debug info for power bar position.x
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		m_ghost->pos.Set(posX, posY, 0); //IMPT
		m_ghost->active = true;
		float sc = 3;
		m_ghost->scale.Set(sc, sc, sc);
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//spawn large GO_BALL
		GameObject *go = FetchGO();
		go->active = true;

		if (i_projectileType == 1)
			go->type = GameObject::GO_CUBE;
		else if (i_projectileType == 2)
			go->type = GameObject::GO_HEXA;
		else if (i_projectileType == 3)
			go->type = GameObject::GO_BALL;

		go->pos = m_ghost->pos;
		go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
		m_ghost->active = false;
		float sc = 3;
		go->scale.Set(sc, sc, sc);
		go->mass = sc * sc * sc;

		// Randomize color of ball
		go->Color.Set(Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(0, 1));
	}

	if (Application::IsKeyPressed(VK_SPACE))
	{
		if (projectile->active)
		{
			projectile->active = false;
			NumMode_tiggered_powerbar = 1;
			m_objectCount = 0;
		}
	}
	


	//Manual Scrolling=============================================//
	if (!projectile->active)
	{
		if (Application::IsKeyPressed(VK_RIGHT))	//Right scrolling 
		{
			if (scrollOffset >= 0 && scrollOffset <= m_worldWidth * 1.5)
			{
				scrollOffset += 60 * dt;
			}
		}
		else if (Application::IsKeyPressed(VK_LEFT))	//Left scrolling 
		{
			if (scrollOffset > 0 && scrollOffset <= m_worldWidth * 1.5)
			{
				scrollOffset -= 60 * dt;
			}
		}
	}
	if (scrollOffset < 0)
		scrollOffset = 0;
	//End of Manual Scrolling======================================//




	//Physics Simulation Section
	dt *= m_speed;


	//Projectile Scrolling========================================//
	camera.target.x = launched + scrollOffset;
	camera.position.x = launched + scrollOffset;

	if (projectile->pos.x > m_worldWidth / 2)	//projectile scrolling
	{
		launched = projectile->pos.x - m_worldWidth / 2;
	}

	if (launched > m_worldWidth)	//make camera.position.x stop moving
		launched = m_worldWidth;

	if (!projectile->active)	//reset camera.position.x to initial position
		launched = 0;
	//End of Scrolling============================================//





	//Background stuff============================================//
	//Mountain resizing
	mountain->pos.Set(m_worldWidth / 2 + launched + scrollOffset, m_worldHeight / 2, -5);
	mountain->scale.Set(m_worldWidth + 2, m_worldHeight, 1);
	//Background Fire resizing
	background_fire->pos.Set(m_worldWidth / 2 + launched + scrollOffset, m_worldHeight / 2, -7);
	background_fire->scale.Set(m_worldWidth + 2, m_worldHeight, 1);
	//Foreground Fire resizing
	foreground_fire->pos.Set(m_worldWidth / 2 + launched + scrollOffset, m_worldHeight / 2, -3);
	foreground_fire->scale.Set(m_worldWidth + 2, m_worldHeight, 1);

	//Update Sprite Animation
	SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_BACKGROUND_FIRE]);
	SpriteAnimation* sa1 = dynamic_cast<SpriteAnimation*>(meshList[GEO_FOREGROUND_FIRE]);
	if (sa && sa1)
	{
		sa->Update(dt);
		sa->m_anim->animActive = true;

		sa1->Update(dt);
		sa1->m_anim->animActive = true;
	}

	//End of Background stuff=====================================//





	if (Application::IsKeyPressed('R'))
	{
		SceneManager::getInstance()->changeScene(new SceneMainMenu());
		SetCurrentLevel(1);
		SetTempScore(0);
		SetScore(0);
		SetTempCurrency(0);
		SetCurrency(0);
		upgraded.ResetFile("Text//Speed_Upgrade.txt", "");
	}

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if ((go->type == GameObject::GO_BALL) || (go->type == GameObject::GO_CUBE) || (go->type == GameObject::GO_HEXA))
			{
				go->pos += go->vel * static_cast<float>(dt);
				go->vel += gravity * dt;
				if (go->pos.x > m_worldWidth * 2 + go->scale.x || go->pos.x < -go->scale.x || go->pos.y > m_worldHeight + go->scale.y || go->pos.y < -go->scale.y)
				{
					go->pos.SetZero();
					go->active = false;
					--m_objectCount;
					NumMode_tiggered_powerbar = 1;
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

				GameObject *goA, *goB;

				if ((go->type == GameObject::GO_BALL) || (go->type == GameObject::GO_CUBE) || (go->type == GameObject::GO_HEXA))
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

	if (fortCount == 0)
	{
		SceneManager::getInstance()->changeScene(new SceneUpgrade());
		SetTempScore(i_tempScore);
		SetScore(i_tempScore);
		SetTempCurrency(i_tempCurrency);
		SetCurrency(i_tempCurrency);
		i_CurrentLevel++;
		SetCurrentLevel(i_CurrentLevel);
	}
}

void SceneCollision::CreateStuff()
{
	float w = Application::GetWindowWidth();
	float h = Application::GetWindowHeight();

	float m_worldWidth = 133;
	float m_worldHeight = 100;

	//GameObject *hexagon = FetchGO();
	//hexagon->type = GameObject::GO_HEXA;	// Left Wall
	//hexagon->active = true;
	//hexagon->dir.Set(1, 0, 0);
	//hexagon->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0);
	//hexagon->scale.Set(2, 2, 1);

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

	background_fire = new GameObject(GameObject::GO_BACKGROUND_FIRE);	// Background Fire
	background_fire->active = true;
	background_fire->pos.Set(m_worldWidth / 2, m_worldHeight / 2, -7);
	background_fire->scale.Set(m_worldWidth, m_worldHeight, 1);
	m_goList.push_back(background_fire);

	mountain = new GameObject(GameObject::GO_MOUNTAIN);		// Background Mountain
	mountain->active = true;
	mountain->pos.Set(m_worldWidth / 2, m_worldHeight / 2, -5);
	mountain->scale.Set(m_worldWidth, m_worldHeight, 1);
	m_goList.push_back(mountain);

	foreground_fire = new GameObject(GameObject::GO_FOREGROUND_FIRE);		// Foreground Fire
	foreground_fire->active = true;
	foreground_fire->pos.Set(m_worldWidth / 2, m_worldHeight / 2, -3);
	foreground_fire->scale.Set(m_worldWidth, m_worldHeight, 1);
	m_goList.push_back(foreground_fire);
		
	{
		//Cannon
		platform = new GameObject(GameObject::GO_CANNON_PLATFORM);	// Platform for Cannon
		platform->active = true;
		platform->dir.Set(0, 1, 0);
		platform->pos.Set(133 / 8, 100 / 6.9, 0);
		platform->scale.Set(1, 3, 1);
		m_goList.push_back(platform);

		cannon = new GameObject(GameObject::GO_CANNON);	// Cannon
		cannon->active = true;
		cannon->pos = platform->pos;
		cannon->scale.Set(1, 2.5, 1);
		m_goList.push_back(cannon);
	}
	// Power bar
	{
		powerrange = new GameObject(GameObject::GO_POWERRANGE); // power range
		powerrange->active = true;
		powerrange->pos.Set(0, m_worldHeight - 2.5, 1);
		powerrange->scale.Set(50, 80, 1);
		m_goList.push_back(powerrange);

		powerbar = new GameObject(GameObject::GO_POWERBAR); // power bar
		powerbar->active = true;
		powerbar->pos.Set(original_position_powerbar, m_worldHeight - 2.5, 1);
		powerbar->scale.Set(1, 80, 1);
		m_goList.push_back(powerbar);

		guidemarker = new GameObject(GameObject::GO_GUIDEMARKER); // power range
		guidemarker->active = true;
		guidemarker->pos = cannon->pos;
		guidemarker->pos.z = -1.0f; // push the marker behind
		//guidemarker->pos.y = guidemarker->pos.y + 10.0f;
		guidemarker->scale.Set(1, 1, 1);
		m_goList.push_back(guidemarker);
	}

	//{ // Testing Structure
	//	GameObject* blocks = FetchGO();
	//	blocks->type = GameObject::GO_BLOCKS;	// Vertical
	//	blocks->active = true;
	//	blocks->dir.Set(1, 0, 0);
	//	blocks->pos.Set(133 / 2 + 16, blocks->scale.y + 5, 0);
	//	blocks->scale.Set(2, 8, 1);
	//	blocks->Color.Set(0.8, 0.8, 0);
	//	fortCount++;

	//	std::cout << "This is spawned!" << std::endl;
	//}
}

void SceneCollision::CreateLevel(int level)
{
	std::vector<LevelCreate> createLevel;
	createLevel = loadText.LoadTextFile();
	for (unsigned int i = 0; i < createLevel.size(); i++)
	{
		/*std::cout <<
		createLevel[i].get_level() << ", " <<
		createLevel[i].get_type() << ", " <<
		createLevel[i].get_active() << ", " <<
		createLevel[i].get_dirx() << ", " <<
		createLevel[i].get_diry() << ", " <<
		createLevel[i].get_dirz() << ", " <<
		createLevel[i].get_posx() << ", " <<
		createLevel[i].get_posy() << ", " <<
		createLevel[i].get_posz() << ", " <<
		createLevel[i].get_scax() << ", " <<
		createLevel[i].get_scay() << ", " <<
		createLevel[i].get_scaz() << ", " <<
		createLevel[i].get_colr() << ", " <<
		createLevel[i].get_colg() << ", " <<
		createLevel[i].get_colb() << std::endl;*/

		if (createLevel[i].get_level() == level)
		{
			GameObject* go = FetchGO();
			if (createLevel[i].get_type() == "GO_BLOCKS")
				go->type = GameObject::GO_BLOCKS;
			if (createLevel[i].get_active() == 1)
				go->active = true;
			go->dir.Set(createLevel[i].get_dirx(), createLevel[i].get_diry(), createLevel[i].get_dirz());
			go->pos.Set(createLevel[i].get_posx(), createLevel[i].get_posy(), createLevel[i].get_posz());
			go->scale.Set(createLevel[i].get_scax(), createLevel[i].get_scay(), createLevel[i].get_scaz());
			go->Color.Set(createLevel[i].get_colr(), createLevel[i].get_colg(), createLevel[i].get_colb());
			fortCount++;
		}

		//std::cout << createLevel.size() << std::endl;
	}
}

void SceneCollision::SetCurrentLevel(int levelNo)
{
	ofstream myFile;
	myFile.open("Text//CurrentLevel.txt");
	myFile << levelNo << endl;
	myFile.close();
}

int SceneCollision::GetCurrentLevel()
{
	int level;
	ifstream myFile;
	myFile.open("Text//CurrentLevel.txt");
	myFile >> level;
	myFile.close();

	return level;
}

void SceneCollision::SetScore(int score)
{
	ofstream myFile;
	myFile.open("Text//Score.txt");
	myFile << score << endl;
	myFile.close();
}

int SceneCollision::GetScore()
{
	int score;
	ifstream myFile;
	myFile.open("Text//Score.txt");
	myFile >> score;
	myFile.close();

	return score;
}

void SceneCollision::SetTempScore(int tempScore)
{
	ofstream myFile;
	myFile.open("Text//TempScore.txt");
	myFile << tempScore << endl;
	myFile.close();
}

int SceneCollision::GetTempScore()
{
	int tempScore;
	ifstream myFile;
	myFile.open("Text//TempScore.txt");
	myFile >> tempScore;
	myFile.close();

	return tempScore;
}

void SceneCollision::SetCurrency(int currency)
{
	ofstream myFile;
	myFile.open("Text//Currency.txt");
	myFile << currency << endl;
	myFile.close();
}

int SceneCollision::GetCurrency()
{
	int currency;
	ifstream myFile;
	myFile.open("Text//Currency.txt");
	myFile >> currency;
	myFile.close();

	return currency;
}

void SceneCollision::SetTempCurrency(int tempCurrency)
{
	ofstream myFile;
	myFile.open("Text//TempCurrency.txt");
	myFile << tempCurrency << endl;
	myFile.close();
}

int SceneCollision::GetTempCurrency()
{
	int tempCurrency;
	ifstream myFile;
	myFile.open("Text//TempCurrency.txt");
	myFile >> tempCurrency;
	myFile.close();

	return tempCurrency;
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
	case GameObject::GO_HEXA:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HEXA], true, go->Color);
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
	case GameObject::GO_MOUNTAIN:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_MOUNTAIN], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BACKGROUND_FIRE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BACKGROUND_FIRE], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FOREGROUND_FIRE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FOREGROUND_FIRE], true, go->Color);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERBAR:
		modelStack.PushMatrix();
		//modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//RenderMesh(meshList[GEO_POWERBAR], true, go->Color);
		RenderMeshOnScreen(meshList[GEO_POWERBAR], go->pos.x, go->pos.y, go->scale.x, go->scale.y, 1);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERRANGE:
		modelStack.PushMatrix();
		//modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//RenderMesh(meshList[GEO_POWERRANGE], true, go->Color);
		RenderMeshOnScreen(meshList[GEO_POWERRANGE], go->pos.x, go->pos.y, go->scale.x, go->scale.y, 1);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_GUIDEMARKER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0, 0, 1);
		modelStack.PushMatrix();
		modelStack.Translate(0, 10, 0); // pff set the guide line to the cannon tip
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_GUIDEMARKER], true, go->Color);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	}
}

void SceneCollision::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	//if (m_ghost->active)
	//	RenderGO(m_ghost);

	//if (Application::IsKeyPressed(VK_RETURN))
	//{
	//	//On screen text
	std::ostringstream ss;
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

	ss.str(std::string());
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);

	//	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
	//}

	ss.str(std::string());
	ss.precision(5);
	ss << "TempScore: " << i_tempScore;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
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
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
