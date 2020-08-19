#include "FindPlayerState.h"
#include "GameManager.h"
#include "Agent.h"
#include "Leader.h"
#include "Swarmer.h"
#include <map>

// Use a namespace to avoid global namespace (FPS = FindPlayerState)
namespace FPS
{
	// Holds an agent's path and what node theyre at
	struct UniquePath
	{
		std::vector<Graph2D::Node*> path;
		Graph2D::Node* currentNode = nullptr;

		float timePassed = 0.0f;
	};
	// Map using the agent's pointer as the key
	std::map<Agent*, UniquePath> pathInfo;
}


FindPlayerState::FindPlayerState(float arriveRadius, float formationRadius) :
	player(),
	arriveRadius(arriveRadius),
	formationRadius(formationRadius)
{
}

FindPlayerState::~FindPlayerState()
{
}


void FindPlayerState::setup(Agent* agent)
{
	// Update the player
	player = GameManager::searchForTag(Tag::Player).front();

	// Find a path from this agent to the player
	FPS::pathInfo[agent].path = GameManager::findPath(agent->getPos(), player->getPos());
	// The first node is the starting one
	FPS::pathInfo[agent].currentNode = FPS::pathInfo[agent].path.front();
}

Vector2 FindPlayerState::update(Agent* agent, float deltaTime)
{
	// If the agent is within range of the player and they have no formation members, enter formation
	if (Vector2Distance(agent->getPos(), player->getPos()) < formationRadius && ((Leader*)agent)->getFormation().empty())
	{
		// Offsets from 'agent' to cause a 'U' formation
		Vector2 offsets[4];
		offsets[0] = { 10, 20 };
		offsets[1] = { 10, -20 };
		offsets[2] = { 30, 40 };
		offsets[3] = { 30, -40 };


		std::vector<Swarmer*> swarm = ((Leader*)agent)->getSwarm();

		// Loop through 4 swarmers, or as many as avalable
		for (int i = 0; i < 4 && i < swarm.size(); i++)
		{
			// Make the swarmer enter a formation in the designated position around the leader
			swarm[i]->enterFormation(100.0f, agent, offsets[i]);
			((Leader*)agent)->addToFormation(swarm[i]);
		}
	}



	// If enough time has passed, get a new path
	if (FPS::pathInfo[agent].timePassed >= 1.0f)
	{
		FPS::pathInfo[agent].path = GameManager::findPath(agent->getPos(), player->getPos());
		FPS::pathInfo[agent].timePassed = 0.0f;
	}
	else
	{
		FPS::pathInfo[agent].timePassed += deltaTime;
	}


	// If there is no target node, something has gone wrong, and a transition should occur
	if (FPS::pathInfo[agent].currentNode == nullptr)
		return Vector2Zero();
	
	// Get the Vector2 from the current node in the path
	Vector2 target = FPS::pathInfo[agent].currentNode->data;


	// If the agent is close to the current node, set current to the next node
	if (Vector2Distance(agent->getPos(), target) < 17.0f)
	{
		// Find the index of the current node
		size_t i = 0;
		for (auto itter : FPS::pathInfo[agent].path)
		{
			if (itter == FPS::pathInfo[agent].currentNode)
				break;

			i++;
		}

		// If the index is out of range, use the first node, otherwise use the index
		if (i + 1 > FPS::pathInfo[agent].path.size())
			FPS::pathInfo[agent].currentNode = FPS::pathInfo[agent].path[0];
		else
			FPS::pathInfo[agent].currentNode = FPS::pathInfo[agent].path[i + 1];
	}


	// Get the diference in position to find the heading, then scale to max velocity
	Vector2 seekForce = Vector2Subtract(target, agent->getPos());
	if (seekForce.x != 0.0f || seekForce.y != 0.0f)
	{
		seekForce = Vector2Normalize(seekForce);
		seekForce = Vector2Scale(seekForce, agent->getMaxSpeed());
	}

	// Find the force nessesary to change the velocity
	seekForce = Vector2Subtract(seekForce, agent->getVel());
	// If non-zero, normalise to the base value
	if (seekForce.x != 0.0f || seekForce.y != 0.0f)
	{
		seekForce = Vector2Normalize(seekForce);
		seekForce = Vector2Scale(seekForce, 100.0f);
	}


	// Find the distance between the two agents
	float distance = Vector2Distance(target, agent->getPos());
	// If the agent is close to the target, start to slow down
	if (distance < arriveRadius)
	{
		seekForce = Vector2Scale(seekForce, (distance / arriveRadius));
		// Reduce velocity on approach
		seekForce = Vector2Subtract(seekForce, Vector2Scale(agent->getVel(), 2.0f));
	}


	// Return the final force
	return seekForce;
}

void FindPlayerState::shutdown(Agent* agent)
{
	// Clear the path as it is no longer needed
	FPS::pathInfo[agent].path.clear();
	FPS::pathInfo[agent].currentNode = nullptr;
}