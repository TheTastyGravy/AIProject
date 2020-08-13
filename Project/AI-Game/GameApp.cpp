#include "GameApp.h"
#include <memory>
#include <time.h>
#include "raylib.h"
#include "GameManager.h"

#include "Spawner.h"


void GameApp::run()
{
	// Create window and starting objects
	startup({ (const float)SCREEN_WIDTH, (const float)SCREEN_HIGHT });

	// Game loop
	while (!WindowShouldClose())
	{
		update(GameManager::getPool());

		draw(GameManager::getPool());
	}

	shutdown();
}


void GameApp::update(std::vector<GameObject*>& objects)
{
	float deltaTime = GetFrameTime();
	
	// Update objects
	for (int i = 0; i < objects.size(); i++)
		objects[i]->update(deltaTime);
}

void GameApp::draw(std::vector<GameObject*>& objects)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	// Draw objects
	for (auto object : objects)
		object->draw();

	// Draw FPS
	DrawFPS(5, 5);

	EndDrawing();
}


void GameApp::startup(Vector2 screenSize)
{
	// Setup window
	InitWindow(SCREEN_WIDTH, SCREEN_HIGHT, "AI Game");
	SetTargetFPS(60);


	//create a spawner
	new Spawner({ 200, 200 }, nullptr);
}

void GameApp::shutdown()
{
	std::vector<GameObject*> objects = GameManager::getPool();

	// Delete all objects
	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}

	CloseWindow();
}