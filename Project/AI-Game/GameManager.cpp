#include "GameManager.h"

std::vector<GameObject*> GameManager::pool;
std::vector<GameObject*> GameManager::leaders;
Graph2D GameManager::graph;


std::vector<GameObject*> GameManager::searchForTag(Tag tag)
{
	// Return the leader vector if applicable
	if (tag == Tag::Leader)
		return leaders;

	std::vector<GameObject*> result;

	// Go trough each object in the pool
	for (auto obj : pool)
	{
		// Go through the object's tags
		for (auto objTag : obj->tags)
		{
			// The object has the tag; add it to result and move on
			if (objTag == tag)
			{
				result.push_back(obj);
				break;
			}
		}
	}

	// Return vector with all objects with the tag
	return result;
}

std::vector<GameObject*> GameManager::searchInRadius(Tag tag, float radius, Vector2 position)
{
	std::vector<GameObject*> result;

	// Go through each object in the pool
	for (auto obj : pool)
	{
		// If obj is out of range, move on
		if (Vector2Distance(obj->position, position) > radius)
		{
			continue;
		}

		// Go through the object's tags
		for (auto objTag : obj->tags)
		{
			// The object has the tag; add it to result and move on
			if (objTag == tag)
			{
				result.push_back(obj);
				break;
			}
		}
	}

	// Return vector with all valid objects
	return result;
}


void GameManager::addToPool(GameObject* obj)
{
	// Check that obj isnt already in the pool
	for (auto var : pool)
	{
		if (var == obj)
			return;
	}

	pool.push_back(obj);
}

void GameManager::removeFromPool(GameObject* obj)
{
	bool isLeader = false;

	// Check if the object is a leader
	for (auto itter : obj->tags)
	{
		if (itter == Tag::Leader)
		{
			isLeader = true;
			break;
		}
	}
	if (isLeader)
	{
		// Remove the object from the leader vector
		for (int i = 0; i < leaders.size(); i++)
		{
			if (leaders[i] == obj)
			{
				leaders.erase(leaders.begin() + i);
				break;
			}
		}
	}

	// Find the object and remove it
	for (int i = 0; i < pool.size(); i++)
	{
		if (pool[i] == obj)
		{
			pool.erase(pool.begin() + i);
			return;
		}
	}
}


void GameManager::updateLeader(GameObject* obj, bool adding)
{
	// If it needs to be added, add it and exit
	if (adding)
	{
		leaders.push_back(obj);
		return;
	}

	// Otherwise, find and remove it
	for (int i = 0; i < leaders.size(); i++)
	{
		if (leaders[i] == obj)
		{
			leaders.erase(leaders.begin() + i);
			return;
		}
	}
}


std::vector<Graph2D::Node*> GameManager::findPath(Vector2 start, Vector2 end)
{
	//TODO: add pathfinding stuff

	return std::vector<Graph2D::Node*>();
}

bool GameManager::loadGraph()
{
	//TODO: load graph

	return false;
}