#include <iostream>
#include "Tests.h"
#include "AStarInternal.h"
#include "AStar.h"
#include "Graph.h"
#include "Worker.h"
#include "../Level.h"
#include "../Locator.h"

using namespace astar;

Tests::Tests()
{ }

void Tests::initialize()
{
	int tiles[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3,
        0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 0, 3, 0, 0, 0, 0,
        0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 0, 3, 3, 0,
        0, 3, 0, 3, 3, 3, 0, 3, 0, 3, 0, 3, 0, 0, 3, 0,
        0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 3, 3, 0, 0, 0,
        0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0,
        0, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    Locator::level()->load("../assets/textures/level.png", sf::Vector2u(32, 32), tiles, 16, 8);
}

Path* Tests::findPath(sf::Vector2u from, sf::Vector2u to)
{
	auto level = Locator::level();

	Graph graph(*level);
	graph.initialize();

	Worker finder;
	Path* path = finder.findPath(from, to, &graph);

	return path;
}

void Tests::run()
{
	initialize();

	auto from = sf::Vector2u(4, 4);
	auto to = sf::Vector2u(14, 0);

	std::cout << "Asking path from (" << from.x << "," << from.y << ") ";
	std::cout << "to (" << to.x << "," << to.y << ")." << std::endl;

	Path* path = findPath(from, to);
	if (path != nullptr) {
		std::cout << "Path contains " << path->count() << " positions: ";

		if (path->last() == to) {
			std::cout << "Path finding successful!" << std::endl;
		} else {
			std::cout << "Path finding failed." << std::endl;
		}

		delete path;
	}

	std::cout << "Tests complete." << std::endl;
}