#ifndef ASTAR_PATHITERATOR_H_
#define ASTAR_PATHITERATOR_H_

#include <SFML/Graphics.hpp>
#include "AStarInternal.h"

namespace astar
{
	/// <summary>
	/// An iterator for <c>Path</c> traversing.
	/// </summary>
	class PathIterator
	{
	public:
		PathIterator(const Path* path);
		PathIterator(const PathIterator& other);
		~PathIterator();

		sf::Vector2u item();

		PathIterator& operator++();
		PathIterator operator++(int);
		bool operator==(const PathIterator& other);
		bool operator!=(const PathIterator& other);
		bool operator< (const PathIterator& other);
		bool operator> (const PathIterator& other);
		bool operator<=(const PathIterator& other);
		bool operator>=(const PathIterator& other);

	private:
		const Path* path_;
		int index_;
	};
}

#endif
