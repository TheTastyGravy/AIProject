#include "Graph2D.h"
#include "raylib.h"
#include <map>
#include <queue>
#include <functional>


Graph2D::Graph2D()
{
}

Graph2D::~Graph2D()
{
}


void Graph2D::draw(const float& nodeSize) const
{
	for (auto& node : nodes)
	{
		// Draw edges
		for (auto& edge : node->edges)
		{
			DrawLineV(node->data, edge->node->data, BLACK);
		}
	}

	// Draw nodes after edges to they appear above
	for (auto& node : nodes)
	{
		DrawCircleV(node->data, nodeSize, SKYBLUE);
		DrawCircleLines((int)node->data.x, (int)node->data.y, nodeSize, BLACK);
	}
}


bool Graph2D::findPath(Node* start, Node* end, std::vector<Node*>& outPath) const
{
	// A node can be in one of three states:
	//		Open if it hasnt been toutched yet
	//		InQueue if it is ready to be processed, but hasnt yet
	//		Closed if it has been processed
	enum State { Open, InQueue, Closed };
	// Used to hold a nodes info that shouldnt be persistant
	struct NodeInfo
	{
		State state;
		Node* parent = nullptr;
		int g = 0;
		int f = 0;
	};


	// Map a node pointer to state, parent, and g-score
	std::map<Node*, NodeInfo> nodeInfo;

	// Custom comparison function for priority_queue. Smallest f-score on top
	std::function<bool(Node*, Node*)> comp = [&nodeInfo](Node* a, Node* b) { return nodeInfo[a].f > nodeInfo[b].f; };
	// Create priority_queue using custom comp function so they are ordered by g-score
	std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>> queue(comp);
	// Add the starting node
	queue.push(start);
	

	// Run as long as there are nodes
	while (!queue.empty())
	{
		// Get the next node, and remove it from the queue
		Node* node = queue.top();
		queue.pop();

		// If the node has been done, skip, else flag it as done
		if (nodeInfo[node].state == State::Closed)
			continue;
		else
			nodeInfo[node].state = State::Closed;

		
		// This is the target, end
		if (node == end)
			break;


		// Iterate over the node's edges
		for (auto edge : node->edges)
		{
			// Skip if its been processed
			if (nodeInfo[edge->node].state == State::Closed)
				continue;


			// g-score = this.g + edge.cost
			int gScore = nodeInfo[node].g + edge->data;

			// If the node is in the queue and has a beter path, dont change it
			if (nodeInfo[edge->node].state == State::InQueue && nodeInfo[edge->node].g <= gScore)
				continue;


			// Node is either open, or in queue and this is a beter path

			// Set nodeInfo to path from this node
			nodeInfo[edge->node].state = State::InQueue;
			nodeInfo[edge->node].parent = node;
			nodeInfo[edge->node].g = gScore;
			// f-score = g-score + dist(node.pos, end.pos)
			nodeInfo[edge->node].f = gScore + (int)Vector2Distance(edge->node->data, end->data);
			// Add the node to the queue
			queue.push(edge->node);
		}
	}
	

	// If the algorithm didnt get to 'end', return false
	if (nodeInfo[end].state != State::Closed)
		return false;


	// Recursive lambda that gets the full path
	std::function<void(Node*, std::vector<Node*>&)> getPath = [&nodeInfo, &getPath, &start](Node* n, std::vector<Node*>& vec)
	{
		// Recursion until we get to the start of the path
		if (n != start)
			getPath(nodeInfo[n].parent, vec);
		// Add 'n' to the end of the vector
		vec.push_back(n);
	};

	// Set the out param to the full path, and return true
	getPath(end, outPath);
	return true;
}


Graph2D::Node* Graph2D::findClosestNode(const Vector2& pos) const
{
	// If there are no nodes, return nullptr
	if (nodes.empty())
		return nullptr;


	// Start with the first node being the best
	Node* bestNode = nodes[0];
	float bestDist = Vector2Distance(pos, bestNode->data);

	// Itterate through each node
	for (auto node : nodes)
	{
		// Get the distance from the node to the target position
		float dist = Vector2Distance(pos, node->data);

		// If this node is closer, its the best node
		if (dist < bestDist)
		{
			bestNode = node;
			bestDist = dist;
		}
	}

	// Return the closest node
	return bestNode;
}