#include "WanderStateBehav.h"
#include "Agent.h"
#include "Swarmer.h"
#include "Leader.h"
#include "GameManager.h"
#include <map>


// Map using the agents pointer as a key so every agent has a unique target
std::map<Agent*, Vector2> agentTargetInfo;


WanderStateBehav::~WanderStateBehav()
{
}


void WanderStateBehav::setup(Agent* agent)
{
	//new target
}

Vector2 WanderStateBehav::update(Agent* agent, float deltaTime)
{
	//get vector of leaders from GameManager
	//check recruiting flag
	//check distance from agent
	//if close enough, call func to enter flocking
	//continue



	// ---------- WANDER FORCE ----------

	// If the agent is close to the target, get a new one
	if (Vector2Distance(agentTargetInfo[agent], agent->getPos()) < 5.0f)
	{
		//new target
	}


	// Find the displacement between the current and desiered velocity
	Vector2 force = Vector2Subtract(agentTargetInfo[agent], agent->getPos());

	// Scale the point to the base value
	if (force.x != 0.0f && force.y != 0.0f)
	{
		force = Vector2Normalize(force);
		force = Vector2Scale(force, agent->getMaxSpeed());
	}

	// Find the force nessesary to change the velocity
	force = Vector2Subtract(force, agent->getVel());
	// If non-zero, normalise to the base value
	if (force.x != 0.0f && force.y != 0.0f)
	{
		force = Vector2Normalize(force);
		force = Vector2Scale(force, BASE_FORCE);
	}


	// Multiply the force to alter its impact
	return Vector2Scale(force, multi);
}