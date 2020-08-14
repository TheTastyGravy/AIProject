#include "Leader.h"
#include "Swarmer.h"
#include "raylib.h"


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


void Leader::update(float deltaTime)
{
	//TODO: leader logic

	//state machine in agent::update
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