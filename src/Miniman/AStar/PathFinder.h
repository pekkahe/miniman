#ifndef ASTAR_PATHFINDER_H_
#define ASTAR_PATHFINDER_H_

#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <map>
#include "AStarInternal.h"

namespace astar 
{
	/// <summary>
	/// The game character's interface to A* path finding. Attempts to find the shortest path
	/// from tile A to tile B. The A* algorithm is run in a separate thread implemented in <c>Worker</c>.
	/// </summary>
	class PathFinder
	{
	public:
		~PathFinder();

		/// <summary>
		/// Initializes the path finder by building a node visibility graph for the tiles
		/// in the given level.
		/// </summary>
		void initialize(const Level& level);
		
		/// <summary>
		/// Excludes the given position from the client's next path finding.
		/// </summary>
		void exclude(sf::Vector2u position, ClientInterface* client);

		/// <summary>
		/// Includes the given position to the client's next path finding.
		/// </summary>
		void include(sf::Vector2u position, ClientInterface* client);

		/// <summary>
		/// Finds the shortest path between the two positions. Notifies the client once
		/// the worker thread has finished path finding.
		/// </summary>
		void findPath(sf::Vector2u from, sf::Vector2u to, ClientInterface* client);

		/// <summary>
		/// Notifies the path finder that the game is being closed, and no more path
		/// finding threads should be started.
		/// </summary>
		void gameClosed();

		/// <summary>
		/// Unregisters the client from its active path finding operations.
		/// When the worker thread finishes, the client won't be notified.
		/// </summary>
		void unregister(ClientInterface* client);

	private:
		PathFinder();
		friend PathFinder& pathFinder();
		
		void startWorker(sf::Vector2u from, sf::Vector2u to, ClientInterface* client);
		void initGraphForClient(Graph* graph, ClientInterface* client);

	private:
		Graph* graph_;

		std::mutex mutex_;

		// Lock with mutex before reading or writing:
		std::map<ClientInterface*, std::thread::id> threads_;
		std::map<ClientInterface*, sf::Vector2u> includeMap_;
		std::map<ClientInterface*, sf::Vector2u> excludeMap_;

		bool gameClosed_;
	};

	/// <summary>
	/// Returns the static instance of the A* path finder.
	/// </summary>
	PathFinder& pathFinder();
}

#endif