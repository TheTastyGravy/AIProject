#include "GameManager.h"
#include "Wall.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <functional>

std::vector<GameObject*> GameManager::pool;
std::vector<GameObject*> GameManager::leaders;
Graph2D GameManager::graph;


std::vector<GameObject*> GameManager::searchForTag(const Tag tag)
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

std::vector<GameObject*> GameManager::searchInRadius(const Tag tag, const float& radius, const Vector2& position)
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


void GameManager::addToPool(GameObject* const obj)
{
	// Check that obj isnt already in the pool
	for (auto var : pool)
	{
		if (var == obj)
			return;
	}

	pool.push_back(obj);
}

void GameManager::removeFromPool(const GameObject* const obj)
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


void GameManager::updateLeader(GameObject* const obj, const bool adding)
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


std::vector<Graph2D::Node*> GameManager::findPath(const Vector2& start, const Vector2& end)
{
	// Find the target nodes
	Graph2D::Node* startNode = graph.findClosestNode(start);
	Graph2D::Node* endNode = graph.findClosestNode(end);

	// Get a path from start to end
	std::vector<Graph2D::Node*> nodes;
	graph.findPath(startNode, endNode, nodes);

	// Return the path. If there is no path, it will be empty
	return nodes;
}

bool GameManager::loadMap(const std::string& fileName, const int screenHight, const int screenWidth, const int mapHight, const int mapWidth)
{
	// How large each cell on the map is
	const Vector2 CELL_SIZE = { (float)screenWidth / mapWidth, (float)screenHight / mapHight };
	// The range nodes will connect at
	const float LINK_RANGE = CELL_SIZE.x;
	// 2D vector for file data
	std::vector< std::vector<int> > map;
	

	// Open file and check it opened properly
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
	{
		std::cout << "Failed to open file" << std::endl;
		return false;
	}

	// Fill the map from the file
	std::string line;
	while (std::getline(file, line))
	{
		// Add another vector, then fill it with the line, and add it to the map
		std::vector<int> vec;
		for (auto itter : line)
		{
			vec.push_back(itter);
		}
		map.push_back(vec);
	}

	// Display success and close file
	std::cout << "File read successfuly" << std::endl;
	file.close();


	// List of nodes to make adding new ones easier
	std::vector<Graph2D::Node*> nodes;
	// Create a node at 'pos' with links to any other nodes within range
	std::function<Graph2D::Node * (Vector2)> createNodeWithLinks = [&LINK_RANGE, &nodes](Vector2 pos)
	{
		// Create node at the passed position
		Graph2D::Node* newNode = graph.addNode(pos);

		// Link any nodes close by
		for (auto node : nodes)
		{
			// If the nodes are close and its not the new node, link them
			if (Vector2Distance(node->data, newNode->data) <= LINK_RANGE)
			{
				// Use the distance between them as the weight
				int dist = (int)Vector2Distance(node->data, newNode->data);
				node->addEdge(newNode, dist);
				newNode->addEdge(node, dist);
			}
		}

		// Add the node to the list, and return its pointer
		nodes.push_back(newNode);
		return newNode;
	};

	// Read map
	for (int y = 0; y < mapHight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			if (map[y][x] == '0')
			{
				// Cell is empty, so create a node
				createNodeWithLinks({ x * CELL_SIZE.x + CELL_SIZE.x*0.5f, y * CELL_SIZE.y + CELL_SIZE.y*0.5f });
			}
			else if (map[y][x] == '1')
			{
				// Cell is a wall, so create one
				new Wall({ x * CELL_SIZE.x, y * CELL_SIZE.y }, CELL_SIZE);
			}
		}
	}


	// File read succesfully, so return true
	return true;
}