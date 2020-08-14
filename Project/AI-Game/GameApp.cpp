#include "GameApp.h"
#include <memory>
#include <time.h>
#include "raylib.h"
#include "GameManager.h"

#include "Spawner.h"
#include "Swarmer.h"
#include "Leader.h"

#include "FlockingStateBehav.h"

#include "StateMachine.h"
#include "FindPlayerState.h"

#include "KeyboardBehaviour.h"


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

	// Toggle showInfo on tab
	if (IsKeyPressed(KEY_TAB))
		showInfo = !showInfo;
}

void GameApp::draw(std::vector<GameObject*>& objects)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	// Draw objects
	for (auto object : objects)
		object->draw();

	// Show extra information
	if (showInfo)
	{
		GameManager::drawGraph(6);
	}

	// Draw FPS
	DrawFPS(5, 5);

	EndDrawing();
}


void GameApp::startup(Vector2 screenSize)
{
	// Setup window
	InitWindow(SCREEN_WIDTH, SCREEN_HIGHT, "AI Game");
	SetTargetFPS(60);

	// Load walls and graph from file
	GameManager::loadMap("map.txt", SCREEN_HIGHT, SCREEN_WIDTH, 10, 16);


	std::shared_ptr<Behaviour> flock = std::make_shared<FlockingStateBehav>(35.0f, 10.0f, //flock radius
																			90.0f, 15.0f, 10.0f, //wander vals
																			3.0f, 2.5f, 3.0f, 1.0f); //weights
	
	//control an agent for testing
	Agent* player = new Agent({ 200, 500 });
	player->addTag(Tag::Player);
	player->addBehaviour(std::make_shared<KeyboardBehaviour>());



	//crate leader with state machine
	Leader* leader = new Leader({ 450, 500 }, 100.0f);


	//create state machine and states
	std::shared_ptr<StateMachine> stateMachine = std::make_shared<StateMachine>();
	std::shared_ptr<State> findPlayer = std::make_shared<FindPlayerState>(40.0f, 100.0f);

	stateMachine->addState(findPlayer);
	findPlayer->setup(leader);
	stateMachine->setCurrentState(findPlayer.get());


	leader->addBehaviour(stateMachine);


	//create a spawner
	new Spawner({ 150, 250 }, flock, 0.5f);
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