#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAsteroid::SceneAsteroid()
{
}

SceneAsteroid::~SceneAsteroid()
{
}

void SceneAsteroid::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (unsigned i = 0; i < 100; i++)
	{
		GameObject* go = new GameObject(GameObject::GO_ASTEROID);
		m_goList.push_back(go);
	}
	//Exercise 2b: Initialize m_lives and m_score
	m_lives = 10;
	m_score = 0;
	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	m_ship = new GameObject(GameObject::GO_SHIP);
	m_ship->active = true;
	m_ship->scale.Set(1, 1, 1);
	m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0);
	m_ship->momentOfInertia = 1.0f;
	m_ship->angularVeloctiy = 0.0f;
	m_ship->dir.Set(0, 0, 0);

	bulletTime = 0;
	elapsedTime = 0;
}

GameObject* SceneAsteroid::FetchGO()
{
	//Exercise 3a: Fetch a game object from m_goList and return it
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		for (unsigned i = 0; i < 10; i++)
		{
			GameObject* go = (GameObject *)* it;
			if (!go->active)
			{
				go->active = true;
				return m_goList[m_goList.size() - 1];
			}
		}
	}
	return NULL;
}

void SceneAsteroid::Update(double dt)
{
	SceneBase::Update(dt);
	elapsedTime += dt;
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}
	m_force.SetZero();
	m_torque.SetZero();
	//Exercise 6: set m_force values based on WASD
	if (Application::IsKeyPressed('W'))
	{
		m_force += 100.0f * m_ship->dir;
	}
	if (Application::IsKeyPressed('A'))
	{
		m_force += 5 * m_ship->dir * dt;
		Vector3 r(1, -1, 0);
		Vector3 F(0, 5, 0);
		m_torque += r.Cross(F);
	}
	if (Application::IsKeyPressed('S'))
	{
		m_force -= 100.0f * m_ship->dir;
	}
	if (Application::IsKeyPressed('D'))
	{
		m_force += 5 * m_ship->dir * dt;
		Vector3 r(-1, -1, 0);
		Vector3 F(0, 5, 0);
		m_torque += r.Cross(F);
	}
	//Exercise 8: use 2 keys to increase and decrease mass of ship
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_ship->mass = Math::Min(m_ship->mass + 1.f, 100.f);
	}
	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_ship->mass = Math::Max(m_ship->mass - 1.f, 1.0f);
	}
	int counter = 0;
	//Exercise 11: use a key to spawn some asteroids
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject *)* it;
		if (Application::IsKeyPressed('V'))
		{
			if (counter < 25)
			{
				if (!go->active)
				{
					go->active = true;
					go->type = GameObject::GO_ASTEROID;
					go->scale.Set(1, 1, 1);
					go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 0);
					go->vel.Set(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), 0);
					break;
				}
				counter++;
			}
		}
		//if ((go->pos - m_ship->pos).Length() < 30)
		//	go->vel = m_ship->pos - go->pos;
		go->pos += go->vel * dt * m_speed;
		//Exercise 13: asteroids should wrap around the screen like the ship
		if (go->type != GameObject::GO_BULLET)
		{
			if (go->pos.x < 0)
				go->pos.x += m_worldWidth;
			else if (go->pos.x > m_worldWidth)
				go->pos.x -= m_worldWidth;
			if (go->pos.y < 0)
				go->pos.y += m_worldHeight;
			else if (go->pos.y > m_worldHeight)
				go->pos.y -= m_worldHeight;
		}
		else
		{
			//Exercise 16: unspawn bullets when they leave screen
			if (go->pos.x < 0 || go->pos.x > m_worldWidth ||
				go->pos.y < 0 || go->pos.y > m_worldHeight)
				go->active = false;
		}
	}
	//Exercise 14: use a key to spawn a bullet
	if (Application::IsKeyPressed(VK_SPACE) && elapsedTime > bulletTime)
	{
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject *)* it;
			if (!go->active)
			{
				go->active = true;
				go->type = GameObject::GO_BULLET;
				go->scale.Set(1, 1, 1);
				go->pos = m_ship->pos;
				if (m_ship->vel.IsZero())
					go->vel.Set(BULLET_SPEED, 0, 0);
				else
					go->vel = m_ship->dir.Normalized() * BULLET_SPEED;
				break;
			}
		}

		//Exercise 15: limit the spawn rate of bullets
		bulletTime = elapsedTime + 0.25;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Physics Simulation Section

	//Exercise 7: Update ship's velocity based on m_force
	m_ship->vel += (m_force * (1 / m_ship->mass)) * dt * m_speed;
	if (m_ship->vel.Length() > MAX_SPEED)
	{
		m_ship->vel.Normalize();
		m_ship->vel *= MAX_SPEED;
	}
	m_ship->pos += m_ship->vel * dt * m_speed;

	m_ship->momentOfInertia = m_ship->mass * 1 * 1;
	float angularAcceleration = m_torque.z * (1 / m_ship->momentOfInertia); 
	m_ship->angularVeloctiy += angularAcceleration * dt * m_speed;          
	float theta = atan2(m_ship->dir.y, m_ship->dir.x);						
	theta += m_ship->angularVeloctiy * dt * m_speed;						
	m_ship->dir.Set(cos(theta), sin(theta), 0);								
	//Exercise 9: wrap ship position if it leaves screen
	if (m_ship->pos.x < 0)
		m_ship->pos.x += m_worldWidth;
	else if (m_ship->pos.x > m_worldWidth)
		m_ship->pos.x -= m_worldWidth;
	if (m_ship->pos.y < 0)
		m_ship->pos.y += m_worldHeight;
	else if (m_ship->pos.y > m_worldHeight)
		m_ship->pos.y -= m_worldHeight;
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			if (go->type == GameObject::GO_ASTEROID)
			{
				//Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check
				if (m_ship && m_ship->active)
				{
					if ((go->pos - m_ship->pos).LengthSquared() < (go->scale.x + m_ship->scale.x) * (go->scale.x * m_ship->scale.x))
					{
						go->active = false;
						counter--;
						m_lives--;
						m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0);
						m_ship->vel.Set(0, 0, 0);
						break;
					}
					//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
					for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
					{

						GameObject *go2 = (GameObject *)*it2;
						if (go2->active)
						{
							if (go2->type == GameObject::GO_BULLET)
							{
								if ((go->pos - go2->pos).Length() < (go->scale.x + sqrt(((go2->scale.x / 5) * (go2->scale.x / 5)) + ((go2->scale.y / 5) *(go2->scale.y / 5)))))
								{
									go->active = false;
									go2->active = false;
									m_score += 2;
									break;
								}

							}
						}
					}
				}

			}
			

		}
	}

	if (m_lives == 0)
	{
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject* go = (GameObject *)* it;
				if (go->active)
					go->active = false;
			}
		m_lives = 10;
		m_score = 0;
		m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0);
		m_ship->vel.Set(0, 0, 0);
	}
		
}


void SceneAsteroid::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_SHIP:
		//Exercise 4a: render a sphere with radius 1
		//Exercise 17a: render a ship texture or 3D ship model
		//Exercise 17b:	re-orientate the ship with velocity
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		modelStack.Rotate(Math::RadianToDegree(atan2(m_ship->dir.y, m_ship->dir.x)) - 90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SHIP], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ASTEROID:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ASTEROID], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		modelStack.Scale(go->scale.x / 5, go->scale.y / 5, go->scale.z / 5);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAsteroid::Render()
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

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}
	if (m_ship->active)
		RenderGO(m_ship);
	//On screen text

	//Exercise 5a: Render m_lives, m_score
	std::ostringstream ss;
	ss << "Lives: " << m_lives << " Score: " << m_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);
	//Exercise 5b: Render position, velocity & mass of ship
	ss.str("");
	ss << "Pos: " << m_ship->pos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);

	ss.str("");
	ss << "Vel: " << m_ship->vel;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

	ss.str("");
	ss << "Mass: " << m_ship->mass;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 18);

	ss.str("");
	ss.precision(3);
	ss << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
	
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid", Color(0, 1, 0), 3, 0, 0);
}

void SceneAsteroid::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}
