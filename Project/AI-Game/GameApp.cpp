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
#include "Transition.h"
#include "FindPlayerState.h"
#include "FindSpawnerState.h"
#include "RecruitmentState.h"
#include "AttackState.h"

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
	
	//create a spawner
	new Spawner({ 150, 250 }, flock, 0.5f);


	//control an agent for testing
	Agent* player = new Agent({ 200, 500 });
	player->addTag(Tag::Player);
	player->addBehaviour(std::make_shared<KeyboardBehaviour>());



	//crate leader with state machine
	Leader* leader = new Leader({ 450, 500 }, 100.0f);
	leader->setRecruiting(true);

	//create state machine and states
	std::shared_ptr<StateMachine> stateMachine = std::make_shared<StateMachine>();
	std::shared_ptr<State> recruitState = std::make_shared<RecruitmentState>();
	std::shared_ptr<State> findPlayer = std::make_shared<FindPlayerState>(40.0f, 100.0f);
	std::shared_ptr<State> attackState = std::make_shared<AttackState>(50.0f);
	std::shared_ptr<State> findSpawner = std::make_shared<FindSpawnerState>(40.0f);

	//add states to state machine
	stateMachine->addState(findPlayer);
	stateMachine->addState(recruitState);
	stateMachine->addState(attackState);
	stateMachine->addState(findSpawner);


	// ----------- TRANSITIONS ----------

	// recruit -> findPlayer
	std::shared_ptr<Transition> swarmSizeHighTrans = std::make_shared<Transition>(findPlayer.get(), [](const Agent* agent) -> bool
	{
		// When the swarm is at least 50, trigger transition
		return (((Leader*)agent)->getSwarmSize() > 50);
	});
	recruitState->addTransition(swarmSizeHighTrans);

	// findPlayer -> attack
	std::shared_ptr<Transition> inRangeOfPlayerTrans = std::make_shared<Transition>(attackState.get(), [player](const Agent* agent) -> bool
	{
		// When the agent is within range, trigger transition
		return (Vector2Distance(player->getPos(), agent->getPos()) < 50.0f);
	});
	findPlayer->addTransition(inRangeOfPlayerTrans);

	// attack -> findSpawner
	std::shared_ptr<Transition> swarmSizeLowTrans = std::make_shared<Transition>(findSpawner.get(), [](const Agent* agent) -> bool
	{
		// When the swarm is below 10, trigger transition
		return (((Leader*)agent)->getSwarmSize() < 10);
	});
	attackState->addTransition(swarmSizeLowTrans);

	// findSpawner -> recruit
	std::shared_ptr<Transition> inRangeOfSpawnerTrans = std::make_shared<Transition>(recruitState.get(), [](const Agent* agent) -> bool
	{
		// Get spawners
		std::vector<GameObject*> spawners = GameManager::searchForTag(Tag::Spawner);

		// If within range of any spawner (agent could come within range of spawner besides their target), trigger transition
		for (auto itter : spawners)
		{
			if (Vector2Distance(itter->getPos(), agent->getPos()) < 40.0f)
				return true;
		}

		// Not close enough to any spawners, no triggered
		return false;;
	});
	findSpawner->addTransition(inRangeOfSpawnerTrans);


	//setup state machine
	findSpawner->setup(leader);
	stateMachine->setCurrentState(findSpawner.get());

	//add state machine to leader
	leader->addBehaviour(stateMachine);


	
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