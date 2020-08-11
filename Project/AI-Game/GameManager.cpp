#include "GameManager.h"

std::vector<GameObject*> GameManager::pool;


std::vector<GameObject*> GameManager::searchForTag(Tag tag)
{
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


std::vector<GameObject*> GameManager::getLeaders()
{
	// Temporary solution so this function can be called by swarmers
	return searchForTag(Tag::Leader);
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