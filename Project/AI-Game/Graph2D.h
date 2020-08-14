#pragma once
#include "GraphClass.h"
#include "raymath.h"


// Graph storing Vector2 nodes and int edges, with draw and path finding functions
class Graph2D :
	public GraphClass<Vector2, int>
{
public:
	Graph2D();
	virtual ~Graph2D();


	virtual void draw(float nodeSize) const;

	// Set 'outPath' to the full path from 'start' to 'end'. Retuns false if no path was found
	bool findPath(Node* start, Node* end, std::vector<Node*>& outPath) const;

	// Returns the closest node to 'pos'
	Node* findClosestNode(const Vector2& pos) const;
};