#pragma once
#include "GameObject.h"
#include "Graph2D.h"
#include <string>


// The GameManager is a static singleton thats tightly coupled to the GameObject abstract class
class GameManager final
{
	// GameObject's constructor and destructor is what adds an object to the pool
	friend GameObject::GameObject(const Vector2& position);
	friend GameObject::~GameObject();
	friend void GameObject::addTag(const Tag tag);
	friend void GameObject::removeTag(const Tag tag);
public:
	// Return ref to vector of all GameObjects that exist
	static std::vector<GameObject*>& getPool() { return pool; }

	// Return vector of all GameObjects with 'tag'
	static std::vector<GameObject*> searchForTag(const Tag tag);
	// Return all GameObjects with 'tag' in 'radius' of 'position'
	static std::vector<GameObject*> searchInRadius(const Tag tag, const float& radius, const Vector2& position);

	// Find a path from 'start' to 'finish' and retun a vector of nodes closest to thoes points
	static std::vector<Graph2D::Node*> findPath(const Vector2& start, const Vector2& end);
	// Load the graph and walls from the map. Returns true if successful
	static bool loadMap(const std::string& fileName, const int screenHight, const int screenWidth, const int mapHight, const int mapWidth);
	// Calls the draw function on the graph
	static void drawGraph(const float nodeSize) { graph.draw(nodeSize); }

private:
	GameManager() {}
	~GameManager() {}

	static void addToPool(GameObject* const obj);
	static void removeFromPool(const GameObject* const obj);

	// Called when a tag is added/removed and pools may need to be updated
	static void updateLeader(GameObject* const obj, const bool adding);

	// All GameObjects that currently exist
	static std::vector<GameObject*> pool;
	// Subset of pool containing leaders
	static std::vector<GameObject*> leaders;

	static Graph2D graph;
};