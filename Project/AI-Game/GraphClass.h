#pragma once
#include <vector>


template <typename NodeT, typename EdgeT>
class GraphClass
{
public:
	GraphClass() 
	{}
	virtual ~GraphClass() 
	{
		for (size_t i = 0; i < nodes.size(); i++)
		{
			removeNode(nodes[0]);
		}
	}


	// Forward declare due to cyclic reference
	struct Edge;

	class Node
	{
	public:
		// Create edge and add it to this node
		void addEdge(Node* otherNode, const EdgeT& data)
		{
			// Create new edge and add it (copy constructor)
			edges.push_back(new Edge({ otherNode, data }));
		}

		// Remove 'edge' from this node
		void removeEdge(Edge* edge)
		{
			for (int i = 0; i < edges.size(); i++)
			{
				// If this edge is 'edge', remove it
				if (edges[i] == edge)
					edges.erase(edges.begin() + i);
			}

			delete edge;
		}
		// Remove any edges referencing 'node'
		void removeEdge(const Node* node)
		{
			for (unsigned int i = 0; i < edges.size(); i++)
			{
				// If this edge references 'node', remove it
				if (edges[i]->node == node)
				{
					delete edges[i];
					edges.erase(edges.begin() + i);
				}
			}
		}


		NodeT data;
		std::vector<Edge*> edges;
	};

	struct Edge
	{
		// Const pointer to a node; cant point to a diferent node
		Node* const node;
		EdgeT data;
	};


	// Create new node with 'data', and add it to the graph without edges
	Node* addNode(const NodeT& data);
	// Add 'node' to the graph. 'node' should not have any existing edges
	void addNode(const Node* node);

	// Remove node with 'data', and any edges it has
	// Should not be used when there are multiple nodes with the same 'data' value
	void removeNode(const NodeT& data);
	// Remove 'node' from the graph, and any edges it has
	void removeNode(Node* node);

	// Add edge from 'n1' to 'n2' with 'edgeValue'
	void addEdge(Node* n1, Node* n2, const EdgeT& edgeValue);
	// Remove edges from 'n1' to 'n2'
	void removeEdge(Node* n1, const Node* n2);

	// Return a ptr to the node with 'data'. Returns nullptr is no such node exists
	Node* getNode(const NodeT& data) const;

protected:
	std::vector<Node*> nodes;
};



template<typename NodeT, typename EdgeT>
inline typename GraphClass<NodeT, EdgeT>::Node* GraphClass<NodeT, EdgeT>::addNode(const NodeT& data)
{
	// Create new node with 'data' (copy constructor from ref)
	Node* newNode = new Node({ data, {} });
	// Add to nodes
	nodes.push_back(newNode);
	return newNode;
}

template<typename NodeT, typename EdgeT>
inline void GraphClass<NodeT, EdgeT>::addNode(const Node* node)
{
	nodes.push_back(node);
}


template<typename NodeT, typename EdgeT>
inline void GraphClass<NodeT, EdgeT>::removeNode(const NodeT& data)
{
	// Get ptr to node of 'data'
	Node* node = getNode(data);

	// Remove 'node' from the graph
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i] == node)
		{
			nodes.erase(nodes.begin() + i);
			break;
		}
	}

	// After removing 'node' the vector is smalled, so use a new loop
	for (auto n : nodes)
	{
		// Call function on node to remove edges referencing 'node'
		n->removeEdge(node);
	}

	// Free the memory
	delete node;
}

template<typename NodeT, typename EdgeT>
inline void GraphClass<NodeT, EdgeT>::removeNode(Node* node)
{
	// Remove 'node' from the graph
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i] == node)
		{
			nodes.erase(nodes.begin() + i);
			break;
		}
	}

	// After removing 'node' the vector is smalled, so use a new loop
	for (auto n : nodes)
	{
		// Call function on node to remove edges referencing 'node'
		n->removeEdge(node);
	}

	// Free the memory
	delete node;
}


template<typename NodeT, typename EdgeT>
inline void GraphClass<NodeT, EdgeT>::addEdge(Node* n1, Node* n2, const EdgeT& edgeValue)
{
	// Create edge, and add it to 'n1' (copy constructor from ref)
	n1->addEdge(n2, edgeValue);
}

template<typename NodeT, typename EdgeT>
inline void GraphClass<NodeT, EdgeT>::removeEdge(Node* n1, const Node* n2)
{
	// Remove any references to 'n2' from 'n1'
	n1->removeEdge(n2);
}


template<typename NodeT, typename EdgeT>
inline typename GraphClass<NodeT, EdgeT>::Node* GraphClass<NodeT, EdgeT>::getNode(const NodeT& data) const
{
	// Iterate through nodes looking for one with the same data
	for (auto node : nodes)
	{
		if (node->data == data)
		{
			return node;
		}
	}

	// Node does not exist; return nullptr
	return nullptr;
}