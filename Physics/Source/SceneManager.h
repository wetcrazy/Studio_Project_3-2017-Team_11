#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "Application.h"
#include <string>

class SceneManager
{
	friend class Application;
	static SceneManager* _instance;

public:
	static SceneManager* getInstance();

	bool hasPendingScene()
	{
		return _hasPendingScene;
	}
	std::string Location;
	void changeScene(Scene* scene);
	void endGame();

protected:
	SceneManager() {};
	~SceneManager() { delete _instance; };

private:
	Scene* currentScene = 0;
	Scene* pendingScene = 0;

	bool _hasPendingScene = false;
	bool ExitGame = false;

	StopWatch m_timer;
	void Update();
};

#endif