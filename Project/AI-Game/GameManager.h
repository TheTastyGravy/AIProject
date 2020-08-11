#pragma once
#include "GameObject.h"


class GameManager
{
	// GameObject's constructor and destructor is what adds an object to the pool
	friend GameObject::GameObject(Vector2 position);
	friend GameObject::~GameObject();
public:
	// Return ref to vector of all GameObjects that exist
	static std::vector<GameObject*>& getPool() { return pool; }

	// Return vector of all GameObjects with 'tag'
	static std::vector<GameObject*> searchForTag(Tag tag);
	// Return all GameObjects with 'tag' in 'radius' of 'position'
	static std::vector<GameObject*> searchInRadius(Tag tag, float radius, Vector2 position);


	// Return vector of all leaders that exist
	// This is done so swarmers dont cause the whole list be iterated through hundreds of times each update
	static std::vector<GameObject*> getLeaders();

private:
	GameManager() {}
	~GameManager() {}

	static void addToPool(GameObject* obj);
	static void removeFromPool(GameObject* obj);


	// All GameObjects that currently exist
	static std::vector<GameObject*> pool;
};