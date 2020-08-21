#include "WanderStateBehav.h"
#include "Agent.h"
#include "Swarmer.h"
#include "Leader.h"
#include "GameManager.h"
#include <map>


// Map using the agents pointer as a key so every agent has a unique target
std::map<Agent*, Vector2> agentTargetInfo;


WanderStateBehav::WanderStateBehav(const Vector2& wanderCenter, const int wanderRadius, const float& transitionDistance, const float& multi) :
	wandCenter(wanderCenter),
	radius(wanderRadius),
	maxDist(transitionDistance),
	multi(multi)
{
}

WanderStateBehav::~WanderStateBehav()
{
}


void WanderStateBehav::setup(Agent* agent)
{
	// Find a new random target within the area
	agentTargetInfo[agent] = { (float)(rand() % radius + wandCenter.x - radius*0.5f), (float)(rand() % radius + wandCenter.y - radius*0.5f) };
}

Vector2 WanderStateBehav::update(Agent* agent, const float& deltaTime)
{
	// ---------- TRANSITION ----------

	// Itterate through all leaders in the game
	for (auto obj : GameManager::searchForTag(Tag::Leader))
	{
		// Cast to a Leader pointer
		Leader* leader = (Leader*)obj;

		// If the leader is recruiting...
		if (leader->getRecruiting())
		{
			// ...and within range, enter the flocking state
			if (Vector2Distance(agent->getPos(), leader->getPos()) <= maxDist)
			{
				((Swarmer*)agent)->enterFlocking(1.0f, leader);
			}
		}
	}


	// ---------- WANDER FORCE ----------

	// If the agent is close to the target, get a new one
	if (Vector2Distance(agentTargetInfo[agent], agent->getPos()) < 5.0f)
	{
		// Setup gets a random target
		setup(agent);
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