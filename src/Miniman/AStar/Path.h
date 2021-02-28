#ifndef ASTAR_PATH_H_
#define ASTAR_PATH_H_

#include <SFML/Graphics.hpp>
#include <deque>
#include "AStarInternal.h"

namespace astar
{
	/// <summary>
	/// Defines a path which leads from tile A to tile B. Output from <c>PathFinder</c>.
	/// </summary>
	class Path
	{
	public:
		Path();
		Path(const Path& other);
		Path(const std::deque<Node*>& nodes);
		
		friend class PathIterator;
	
		/// <summary>
		/// Adds the tile position to the beginning of the path.
		/// </summary>
		void addToBeginning(const sf::Vector2u& position);

		/// <summary>
		/// Adds the tile position to the end of the path.
		/// </summary>
		void addToEnd(const sf::Vector2u& position);

		/// <summary>
		/// Removes a single position from the beginning of the path.
		/// </summary>
		void removeFromBeginning();

		/// <summary>
		/// Removes the specified amount of positions from the beginning of the path.
		/// </summary>
		void removeFromBeginning(int count);

		/// <summary>
		/// Removes a single position from the end of the path.
		/// </summary>
		void removeFromEnd();

		/// <summary>
		/// Removes the specified amount of positions from the end of the path.
		/// </summary>
		void removeFromEnd(int count);

		/// <summary>
		/// Removes all positions from the path.
		/// </summary>
		void clear();

		/// <summary>
		/// Updates the current position in the path to the next position.
		/// </summary>
		bool increment();

		/// <summary>
		/// Returns the current position in the path.
		/// </summary>
		sf::Vector2u current() const;

		/// <summary>
		/// Returns the first position in the path.
		/// </summary>
		sf::Vector2u first() const;

		/// <summary>
		/// Returns the last position in the path.
		/// </summary>
		sf::Vector2u last() const;

		/// <summary>
		/// Returns the position at the given index.
		/// </summary>
		sf::Vector2u at(size_t index) const;

		/// <summary>
		/// Returns the total amount of position in the path.
		/// </summary>
		int count() const;

		/// <summary>
		/// Returns true if the path has been traversed; the current position
		/// is marked as the last position in the path.
		/// </summary>
		bool endReached() const;

		/// <summary>
		/// Returns an iterator to the beginning of the path.
		/// </summary>
		PathIterator begin() const;

		/// <summary>
		/// Returns an iterator to the end of the path.
		/// </summary>
		PathIterator end() const;

	private:
		/// <summary>
		/// Instantiates a new path iterator.
		/// </summary>
		PathIterator iterator() const;

	private:
		std::deque<sf::Vector2u> positions_;
		unsigned int index_;
	};
}

#endif