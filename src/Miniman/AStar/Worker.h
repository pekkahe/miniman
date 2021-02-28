#ifndef ASTAR_WORKER_H_
#define ASTAR_WORKER_H_

#include <SFML/Graphics.hpp>
#include <list>
#include <deque>
#include <algorithm>
#include "AStarInternal.h"

namespace astar
{
	/// <summary>
	/// Implements the A* path finding algorithm. Run in separate threads managed by <c>PathFinder</c>.
	/// </summary>
	class Worker
	{
	public:
		Worker() {}
		~Worker() {}

		/// <summary>
		/// Finds the shortest path between the two positions using the A* algorithm,
		/// accompanied with the given node visibility graph built for the level.
		/// </summary>
		Path* findPath(sf::Vector2u from, sf::Vector2u to, Graph* graph);

	private:
		/// <summary>
		/// Returns true if the first node has a lower F score than the second,
		/// false otherwise.
		/// </summary>
		static bool compare_byFScore(const Node* first, const Node* second);

		/// <summary>
		/// <para>
		/// Runs one iteration of the A* algorithm by taking the lowest F score node from the open list,
		/// evaluating it by checking its visible nodes, and adding them to the list of open nodes,
		/// if not already in it or evaluated.
		/// </para>
		/// <para>
		/// Returns true if the evaluated node marks the final target position or there are no more nodes
		/// left to evaluate, false otherwise.
		/// </para>
		/// </summary>
		bool searchOpen();

		/// <summary>
		/// Adds the node to the list of possible path nodes under the 
		/// current parent node.
		/// </summary>
		void addToOpenList(Node* node, Node* current);

		/// <summary>
		/// Returns true if the given node is the final destination node,
		/// or there are no more nodes to evaluate.
		/// </summary>
		bool hasReachedEnd(Node* node) const;

		/// <summary>
		/// Returns true if the specified node list contains the given node.
		/// </summary>
		bool contains(const std::list<Node*> list, const Node* value) const;

		/// <summary>
		/// Collects the nodes which form the path.
		/// </summary>
		std::deque<Node*> getPathNodes();

	private:
		Graph* graph_;
		Node* startNode_;
		Node* endNode_;

		/// <summary>
		/// Contains nodes which are yet to be evaluated. List is kept in order,
		/// so that the lowest F score node is always the first.
		/// </summary>
		std::list<Node*> openList_;

		/// <summary>
		/// Contains nodes which have been evaluated.
		/// </summary>
		std::list<Node*> closedList_;
	};
}

#endif