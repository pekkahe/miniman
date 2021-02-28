#ifndef ASTAR_NODE_H_
#define ASTAR_NODE_H_

#include <SFML/Graphics.hpp>

namespace astar
{
	/// <summary>
	/// An A* algorithm path finding node.
	/// </summary>
	class Node
	{
	public:
		Node(sf::Vector2u position) : 
			position_(position), parent(nullptr), gCost(0), hCost(0) { }
		Node(const Node& other) : 
			position_(other.position_), parent(other.parent), gCost(other.gCost), hCost(other.hCost) { }

		/// <summary>
		/// The parent node for this node.
		/// </summary>
		Node* parent;

		/// <summary>
		/// The G cost for this node.
		/// </summary>
		unsigned int gCost; 

		/// <summary>
		/// The H cost for this node.
		/// </summary>
		unsigned int hCost;

		/// <summary>
		/// Calculates the F score for this node.
		/// </summary>
		unsigned int fScore() const { return gCost + hCost; }

		/// <summary>
		/// Returns the tile position for this node.
		/// </summary>
		sf::Vector2u position() { return position_; }

		/// <summary>
		/// Resets the node information.
		/// </summary>
		void reset() { parent = nullptr; gCost = 0; hCost = 0; }

	private:
		sf::Vector2u position_;
	};
}

#endif

