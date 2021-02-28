#ifndef ASTAR_GRAPH_H_
#define ASTAR_GRAPH_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "AStarInternal.h"

namespace astar
{
	/// <summary>
	/// Defines a game specific node visilibity graph for the A* algorithm. 
	/// Stores node instances and maps their visibilities between each other.
	/// </summary>
	class Graph
	{
	public:
		Graph(const Level& level);
		Graph(const Graph& other);
		~Graph();

		/// <summary>
		/// Initializes the node graph by iterating through all tiles in the level
		/// and mapping them to each other by their adjacency.
		/// </summary>
		void initialize();

		/// <summary>
		/// Resets the node graph. 
		/// </summary>
		void reset();

		/// <summary>
		/// Returns the node at the given tile map index.
		/// </summary>
		Node* getNode(int index);

		/// <summary>
		/// Returns the node at the given tile position.
		/// </summary>
		Node* getNode(sf::Vector2u position);
		
		/// <summary>
		/// Returns all node which are visible/reachable from the given node.
		/// </summary>
		std::vector<Node*> getReachableNodes(Node* node);

		/// <summary>
		/// Returns the A* algorithm movement cost for one tile.
		/// </summary>
		unsigned int movementCostPerTile() const { return movementCostPerTile_; }

		/// <summary>
		/// Calculates the A* algorithm  G cost for the given node; how much has the 
		/// movement cost from the starting point to this node.
		/// </summary>
		unsigned int calculateGCost(Node* node) const; 

		/// <summary>
		/// Calculates the A* algorithm alternative G cost for the given node; how much
		/// would G cost be if we used the given node to traverse the path instead of
		/// the original node.
		/// </summary>
		unsigned int alternativeGCost(Node* current) const;

		/// <summary>
		/// Calculates the A* algorithm H cost for the given node; how much will the
		/// movement cost from this node to the specified final destination.
		/// </summary>
		unsigned int calculateHCost(Node* node, Node* endNode) const;
		
		/// <summary>
		/// Adds the tile at the given position to the graph.
		/// </summary>
		void add(sf::Vector2u position);

		/// <summary>
		/// Adds the tile at the given tile map index to the graph.
		/// </summary>
		void add(int index);

		/// <summary>
		/// Removes the tile at the given position from the graph.
		/// </summary>
		void remove(sf::Vector2u position);

		/// <summary>
		/// Removes the tile at the given tile map index from the graph.
		/// </summary>
		void remove(int index);

	private:
		/// <summary>
		/// Maps the tile specified by its tile map array index to the graph.
		/// </summary>
		void mapTile(int index);

		/// <summary>
		/// Returns true if the tile specified by its tile map array index is mappable to the graph.
		/// </summary>
		bool isMappable(int index) const;

	private:
		const Level& level_;

		/// <summary>
		/// A* algorithm movement cost between adjacent nodes.
		/// </summary>
		const unsigned int movementCostPerTile_;

		/// <summary>
		/// Stores all nodes in the level. Key is the node's tile map array index.
		/// </summary>
		std::map<int, Node> nodes_;

		/// <summary>
		/// Stores visibilities for each node to each other. Key is the node's tile map array index,
		/// and value contains indexes to the node's visible/reachable nodes.
		/// </summary>
		std::map<int, std::vector<int>> visibilityMap_;
	};
}

#endif

