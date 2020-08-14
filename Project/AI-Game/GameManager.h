#pragma once
#include "GameObject.h"
#include "Graph2D.h"
#include <string>


class GameManager
{
	// GameObject's constructor and destructor is what adds an object to the pool
	friend GameObject::GameObject(Vector2 position);
	friend GameObject::~GameObject();
	friend void GameObject::addTag(Tag tag);
	friend void GameObject::removeTag(Tag tag);
public:
	// Return ref to vector of all GameObjects that exist
	static std::vector<GameObject*>& getPool() { return pool; }

	// Return vector of all GameObjects with 'tag'
	static std::vector<GameObject*> searchForTag(Tag tag);
	// Return all GameObjects with 'tag' in 'radius' of 'position'
	static std::vector<GameObject*> searchInRadius(Tag tag, float radius, Vector2 position);

	// Find a path from 'start' to 'finish' and retun a vector of nodes closest to thoes points
	static std::vector<Graph2D::Node*> findPath(Vector2 start, Vector2 end);
	// Load the graph and walls from the map. Returns true if successful
	static bool loadMap(std::string fileName, const int screenHight, const int screenWidth, const int mapHight, const int mapWidth);

private:
	GameManager() {}
	~GameManager() {}

	static void addToPool(GameObject* obj);
	static void removeFromPool(GameObject* obj);

	// Called when a tag is added/removed and pools may need to be updated
	static void updateLeader(GameObject* obj, bool adding);

	// All GameObjects that currently exist
	static std::vector<GameObject*> pool;
	// Subset of pool containing leaders
	static std::vector<GameObject*> leaders;

	static Graph2D graph;
};