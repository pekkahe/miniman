#include <iostream>
#include "Worker.h"
#include "Graph.h"
#include "Node.h"
#include "Path.h"

//#define _DEBUG_VERBOSE
using namespace astar;

Path* Worker::findPath(sf::Vector2u from, sf::Vector2u to, Graph* graph)
{
	graph_ = graph;

	// Get the according start and end nodes to our target positions
	startNode_ =  graph->getNode(from);
	endNode_ = graph->getNode(to);

	if (startNode_ == nullptr || endNode_ == nullptr) {
		#ifdef _DEBUG_VERBOSE
			std::cout << "Error: No start or end node found for given positions." << std::endl;
		#endif
		return nullptr;
	}

	// Add start node to open list
	openList_.push_front(startNode_);

	// Start A* iteration
	bool endReached = false;
	do {
		endReached = searchOpen();
	} while (!endReached);

	#ifdef _DEBUG_VERBOSE
		std::cout << "Path finding finished, open list: " << openList_.size();
		std::cout << " closed list: " << closedList_.size() << std::endl;
	#endif

	if (openList_.empty()) {
		#ifdef _DEBUG_VERBOSE
			std::cout << "Could not find path from (" << from.x << "," << from.y << ") ";
			std::cout << "to (" << to.x << "," << to.y << ")." << std::endl;
		#endif
		return nullptr;
	}

	// Collect path nodes into a LIFO container
	std::deque<Node*> nodes = getPathNodes();

	// Create a path from the nodes
	Path* path = new Path(nodes);

	return path;
}

std::deque<Node*> Worker::getPathNodes()
{
	std::deque<Node*> stack;
	
	Node* current = closedList_.back();
	stack.push_front(current);

	while (current->parent != nullptr)
	{
		current = current->parent;
		stack.push_front(current);
	}

	return stack;
}

bool Worker::searchOpen()
{
	// Step 1: Get the lowest F cost node from the open list and set is as current.
    //		   The open list is kept sorted by F cost, so just return the first value from the list.
	Node* current = openList_.front();

	#ifdef _DEBUG_VERBOSE
		std::cout << "Current node: (" << current->position().x << ","<< current->position().y << ") with ";
		std::cout << "F score: " << current->fScore() << "." << std::endl;
	#endif

	// Step 2: Move the current node to the closed list.
	openList_.remove(current);
	closedList_.push_back(current);

	// Step 3: Add reachable nodes to the open list, if they are not already in the closed list.
    //		   Also, if a node is already in the open list, check if the current path is better.
	std::vector<Node*> nodes = graph_->getReachableNodes(current);
	
	#ifdef _DEBUG_VERBOSE
		std::cout << "Reachable nodes: " << nodes.size() << std::endl;
	#endif

	for (size_t i = 0; i < nodes.size(); i++)
	{
		Node* node = nodes[i];

		#ifdef _DEBUG_VERBOSE
			std::cout << "Checking node (" << node->position().x << "," << node->position().y << "): ";
		#endif

		// Step 3b: Ignore nodes which are already in the closed list
		if (contains(closedList_, node)) {
			#ifdef _DEBUG_VERBOSE
				std::cout << "Closed." << std::endl;
			#endif

			continue;
		}

		// Step 3c: If node is already on the open list, check if current path
        //		    to that node is a better one, using G cost as a measure
		if (contains(openList_, node))
		{
			#ifdef _DEBUG_VERBOSE
				std::cout << "Open." << std::endl;
			#endif

			auto altG = graph_->alternativeGCost(current);

			// The node already has a G cost since it's in the open list.
			// Compare it against the G cost which we would have with the current path.
			if (altG < node->gCost)
			{
				#ifdef _DEBUG_VERBOSE
					std::cout << "Alternative G cost smaller. Setting current node as parent." << std::endl;
				#endif

				// Current path is better, change node parent and recalculate G and F costs
				node->parent = current;
				node->gCost = altG;

				// Re-sort openlist since F score has changed
				openList_.sort(compare_byFScore);
			}
		}
		// Step 3d: If node is neither in the closed nor open list, add it to the open list.
		else
		{
			#ifdef _DEBUG_VERBOSE
				std::cout << "New -> Adding to open. ";
			#endif

			addToOpenList(node, current);
		}
	}

	// Step 4: Stop when the current node is the target node, or if we fail to find
	//		   the target, meaning that the open list is empty.
	return hasReachedEnd(current);
}

void Worker::addToOpenList(Node* node, Node* current)
{
	// Make the current node parent for the other node so we can trace our path
	node->parent = current;

	// Calculate the node's G and H scores
	node->gCost = graph_->calculateGCost(node);
	node->hCost = graph_->calculateHCost(node, endNode_);

	#ifdef _DEBUG_VERBOSE
		std::cout << "Parent: (" << node->parent->position().x << "," << node->parent->position().y << "), ";
		std::cout << "G: " << node->gCost << ", H: " << node->hCost << std::endl;
	#endif

	// Add before a node which has a higher
	// F cost, so our list is kept sorted.
	for (auto it = openList_.begin(), end = openList_.end(); it != end; ++it) 
	{
		if (node->fScore() < (*it)->fScore())
		{
			openList_.insert(it, node);
			return;
		}
	}

	// If there wasn't a hit, just add to the end
	openList_.push_back(node);
}

inline bool Worker::hasReachedEnd(Node* node) const
{ 
	return node == endNode_ || openList_.empty(); 
}

bool Worker::contains(const std::list<Node*> list, const Node* value) const 
{ 
	return std::count(list.begin(), list.end(), value) > 0; 
}

bool Worker::compare_byFScore(const Node* first, const Node* second)
{
	return first->fScore() < second->fScore();
}
