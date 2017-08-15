#include "Application.h"
#include "SceneManager.h"

#include <GLFW\glfw3.h>

SceneManager *SceneManager::_instance = 0;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

SceneManager* SceneManager::getInstance() {
	if (!_instance) {
		_instance = new SceneManager();
	}
	return _instance;
}


void SceneManager::Update() {

	m_timer.startTimer(); // Start timer to calculate how long it takes to render this frame

	while (!glfwWindowShouldClose(glfwGetCurrentContext()) && !Application::IsKeyPressed(VK_ESCAPE))
	{
		currentScene->Update(m_timer.getElapsedTime());
		currentScene->Render();
		//Swap buffers
		glfwSwapBuffers(glfwGetCurrentContext());
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.  

											// Change scene if there is any pending scene
		if (_hasPendingScene) {
			currentScene->Exit();
			delete currentScene;
			currentScene = pendingScene;
			currentScene->Init();
			_hasPendingScene = false;
			pendingScene = nullptr;
		}
	} //Check if the ESC key had been pressed or if the window had been closed

	currentScene->Exit();
	delete currentScene;
}

void SceneManager::changeScene(Scene* scene) {
	if (!currentScene) {
		delete currentScene;
		if (currentScene == nullptr)
		{
			currentScene = scene;
			scene->Init();
		}
	}
	else {
		pendingScene = scene;
		_hasPendingScene = true;
	}
}

void SceneManager::endGame() { ExitGame = true; }