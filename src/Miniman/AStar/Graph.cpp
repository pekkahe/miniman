#include <cassert>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "Graph.h"
#include "Node.h"
#include "../Level.h"
#include "../Global.h"

using namespace astar;

Graph::Graph(const Level& level) :
	level_(level),
	movementCostPerTile_(1)
{ }

Graph::Graph(const Graph& other) :
	level_(other.level_),
	movementCostPerTile_(other.movementCostPerTile_),
	nodes_(other.nodes_),
	visibilityMap_(other.visibilityMap_)
{ }

void Graph::initialize()
{
	nodes_.clear();
	visibilityMap_.clear();

	for (unsigned int i = 0; i < level_.width_; ++i)
	{
		for (unsigned int j = 0; j < level_.height_; ++j)
		{
			// Get the array index for the current tile
			int index = i + j * level_.width_;

			// Early out if current tile is non-reachable
			if (!isMappable(index)) {
				continue;
			}

			// Initialize and store a Node object
			nodes_.insert(std::make_pair(index, Node(sf::Vector2u(i, j))));
			
			// NOTE: Consider separating Node and visibility map initializations to
			// their own functions. Then we could separate visibility map to its own
			// class completely, allowing us to change how often it is initialized
			// (dynamic vs. static level).
			
			// Map the tile to our visibility map
			mapTile(index);
		}
	}
}

void Graph::mapTile(int index)
{
	int width = level_.width_;
	int x = index % width;

	// Only check left index if we can actually move there; current tile is not at (0, y)
	if (x > 0) {
		int left = index - 1;
		if (isMappable(left)) {
			visibilityMap_[index].push_back(left);
		}
	}

	// Only check right index if we can actually move there; current tile is not at (max, y)
	if (x + 1 < width) {
		int right = index + 1;
		if (isMappable(right)) {
			visibilityMap_[index].push_back(right);
		}
	}

	int up = index - width;
	if (isMappable(up)) {
		visibilityMap_[index].push_back(up);
	}

	int down = index + width;
	if (isMappable(down)) {
		visibilityMap_[index].push_back(down);
	}
}

inline bool Graph::isMappable(int index) const
{
	if (index > 0 && index <= static_cast<int>(level_.size_)) {
		auto tile = static_cast<Tile>(level_.tileMap_[index]);
		return tile == Tile::TILE_EMPTY || 
			   tile == Tile::TILE_DOT || 
			   tile == Tile::TILE_ENERGIZER || 
			   tile == Tile::TILE_TELEPORT || 
			   tile == Tile::TILE_GHOST_PEN_ENTRY || 
			   tile == Tile::TILE_GHOST_PEN || 
			   tile == Tile::TILE_DEBUG_TRACK ||
			   tile == Tile::TILE_DEBUG_YELLOW ||
			   tile == Tile::TILE_DEBUG_RED;
	}

	return false;
}

void Graph::add(sf::Vector2u position)
{
	add(level_.getIndex(position));
}

void Graph::add(int index)
{
	if (nodes_.count(index) > 0 || visibilityMap_.count(index) > 0) {
		return;
	}

	// Create a new Node
	int width = level_.width_;
	int x = index % width;
	int y = index / width;

	nodes_.insert(std::make_pair(index, Node(sf::Vector2u(x, y))));

	// Map node to others
	mapTile(index);

	// Map from other nodes
	std::vector<int> friends = visibilityMap_[index];

	for (auto it = friends.begin(); it != friends.end(); ++it) {
		std::vector<int>& links = visibilityMap_.at(*it); // Note ref!
		links.push_back(index);
	}
}

void Graph::remove(sf::Vector2u position)
{
	remove(level_.getIndex(position));
}

void Graph::remove(int index) 
{
	if (nodes_.count(index) == 0 || visibilityMap_.count(index) == 0) {
		return;
	}

	// Clear friend links
	std::vector<int> friends = visibilityMap_[index];

	for (auto it = friends.begin(); it != friends.end(); ++it) {
		std::vector<int>& links = visibilityMap_.at(*it); // Note ref!
		
		// Must use algorithm to remove item from vector
		auto first = std::remove(links.begin(), links.end(), index);
		links.erase(first, links.end());
	}

	// Clear own links
	visibilityMap_[index].clear();

	// Remove Node from map
	nodes_.erase(index);
}

Node* Graph::getNode(sf::Vector2u position)
{
	int index = level_.getIndex(position);

	return getNode(index);
}

Node* Graph::getNode(int index)
{
	auto it = nodes_.find(index);

	if (it != nodes_.end()) {
		return &it->second;
	} else {
		return nullptr;
	}
}

void Graph::reset()
{
	for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
		it->second.reset();
	}
}

std::vector<Node*> Graph::getReachableNodes(Node* node)
{
	int index = level_.getIndex(node->position());

	std::vector<Node*> nodes;
	std::vector<int> friends = visibilityMap_[index];

	for (auto it = friends.begin(); it != friends.end(); ++it) {
		nodes.push_back(getNode(*it));
	}

	return nodes;
}

unsigned int Graph::alternativeGCost(Node* current) const
{
	return current->gCost + movementCostPerTile_;
}

unsigned int Graph::calculateGCost(Node* node) const
{
	// If there's no parent, the node must be the start node,
	// which in itself is pointless to calculate a G cost for.
	if (node->parent == nullptr) {
		#ifdef _DEBUG
			std::cout << "WARNING: Cannot calculate G cost. ";
			std::cout << "Node (" << node->position().x << "," << node->position().y << ") has no parent." << std::endl;
		#endif
		return 0;
	}

	return movementCostPerTile_ + node->parent->gCost;
}

unsigned int Graph::calculateHCost(Node* node, Node* endNode) const
{
	auto from = node->position();
	auto to = endNode->position();

	int x = abs((int)(from.x - to.x));
	int y = abs((int)(from.y - to.y));

	return (x + y) * movementCostPerTile_;
}

Graph::~Graph() { }