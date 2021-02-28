#ifndef ASTAR_TESTS_H_
#define ASTAR_TESTS_H_

#include <SFML\Graphics.hpp>

namespace astar
{
	class Path;

	class Tests
	{
	public:
		Tests();
		
		void run();

	private:
		void initialize();
		Path* findPath(sf::Vector2u from, sf::Vector2u to);
	};
}

#endif