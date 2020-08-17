#include "FindSpawnerState.h"
#include "GameManager.h"
#include "Agent.h"
#include <map>
#include <functional>
#include <algorithm>

// Namespace used to prevent conflicts. FSS = FindSpawnerState
namespace FSS
{
	// Holds an agent's path and what node theyre at
	struct UniquePath
	{
		std::vector<Graph2D::Node*> path;
		Graph2D::Node* currentNode = nullptr;
	};
	// Map using the agent's pointer as the key
	std::map<Agent*, UniquePath> pathInfo;
}



FindSpawnerState::FindSpawnerState(float arrivalRadius) :
    arriveRadius(arrivalRadius)
{
}

FindSpawnerState::~FindSpawnerState()
{
}


void FindSpawnerState::setup(Agent* agent)
{
    // Get spawners
    std::vector<GameObject*> spawners = GameManager::searchForTag(Tag::Spawner);

    // Comparison function to sort spawners by distance from the agent
    std::function<bool(GameObject*, GameObject*)> compair = [&agent](GameObject* lhs, GameObject* rhs) -> bool 
    {
        // Get distances from the agent
        float lhsDist = Vector2Distance(lhs->getPos(), agent->getPos());
        float rhsDist = Vector2Distance(rhs->getPos(), agent->getPos());

        // Compair distances
        return lhsDist < rhsDist;
    };
    // Sort the vector using the custom comparison lambda
    std::sort(spawners.begin(), spawners.end(), compair);


    // Itterate through spawners, ordered by distance from the agent
    for (auto itter : spawners)
    {
        // Get a path from the agent to the spawner
		FSS::pathInfo[agent].path = GameManager::findPath(agent->getPos(), itter->getPos());

        // If a path exists, set the current node and exit the loop
        if (!FSS::pathInfo[agent].path.empty())
        {
			FSS::pathInfo[agent].currentNode = FSS::pathInfo[agent].path.front();
            break;
        }
    }
}

Vector2 FindSpawnerState::update(Agent* agent, float deltaTime)
{
	// Spawners dont move, so the path never needs to be updated

	// If there is no target node, something has gone wrong, and a transition should occur
	if (FSS::pathInfo[agent].currentNode == nullptr)
		return Vector2Zero();

	// Get the Vector2 from the current node in the path
	Vector2 target = FSS::pathInfo[agent].currentNode->data;


	// If the agent is close to the current node, set current to the next node
	if (Vector2Distance(agent->getPos(), target) < 17.0f)
	{
		//bad code that works. same as in FindPlayerState

		size_t i = 0;

		// find the index of the current node
		for (auto itter : FSS::pathInfo[agent].path)
		{
			if (itter == FSS::pathInfo[agent].currentNode)
				break;

			i++;
		}

		//if index is out of range, use the first node, otherwise use the index
		FSS::pathInfo[agent].currentNode = (i + 1 > FSS::pathInfo[agent].path.size()) ? FSS::pathInfo[agent].path[0] : FSS::pathInfo[agent].path[i + 1];
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

void FindSpawnerState::shutdown(Agent* agent)
{
    // Clear the path as it is no longer needed
	FSS::pathInfo[agent].path.clear();
	FSS::pathInfo[agent].currentNode = nullptr;
}