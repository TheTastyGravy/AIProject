#pragma once
#include <vector>
#include "GameObject.h"


class GameApp
{
public:
	GameApp() {}
	virtual ~GameApp() {}

	void run();

	virtual void update(std::vector<GameObject*>& objects);
	virtual void draw(std::vector<GameObject*>& objects);

	virtual void startup(Vector2 screenSize);
	virtual void shutdown();

protected:
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HIGHT = 600;
};