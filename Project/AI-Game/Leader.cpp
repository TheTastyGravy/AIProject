#include "Leader.h"
#include "Swarmer.h"
#include "raylib.h"
#include "GameManager.h"
#include "Wall.h"


Leader::Leader(Vector2 position, float importance) :
	Agent(position),
	swarm(),
	formation(),
	isRecruiting(false),
	importance(importance),
	rotation(0.0f)
{
	addTag(Tag::Leader);
}

Leader::~Leader()
{
}


void Leader::update(const float& deltaTime)
{
	//Run regular behaviours
	Agent::update(deltaTime);

	// Increment rotation
	rotation += deltaTime * rotationSpeed;
	if (rotation > 360)
		rotation -= 360;
}

void Leader::draw()
{
	// Draw leaders as a rotating violet triangle
	DrawPoly(position, 3, 8, rotation, VIOLET);
}


void Leader::joinSwarm(Swarmer* swarmer)
{
	// Add the swarmer to the swarm
	swarm.push_back(swarmer);
}

void Leader::leaveSwarm(Swarmer* swarmer)
{
	// Find and remove the swarmer from the swarm
	for (int i = 0; i < swarm.size(); i++)
	{
		if (swarm[i] == swarmer)
		{
			swarm.erase(swarm.begin() + i);
			break;
		}
	}

	// It could also be in formation...
	for (int i = 0; i < formation.size(); i++)
	{
		if (formation[i] == swarmer)
		{
			formation.erase(formation.begin() + i);
			break;
		}
	}
}


void Leader::collisionDetection(float deltaTime, bool& outCollidedX, bool& outCollidedY)
{
	// Get walls within proximity of the agent
	std::vector<GameObject*> walls = GameManager::searchInRadius(Tag::Wall, 200.0f, position);

	for (auto obj : walls)
	{
		// If it collided with a wall, set outs and exit
		if (CheckCollisionCircleRec(Vector2Add(position, Vector2Scale(velocity, deltaTime)), 8, ((Wall*)obj)->getRec()))
		{
			outCollidedX = true;
			outCollidedY = true;
			return;
		}
	}
	
}