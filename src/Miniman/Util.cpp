#include <cstdlib>
#include <cstring>
#include <sstream>
#include <ctime>
#include <cmath>
#include <cstdarg>
#include <memory>
#include <cstdlib>
#include "Math.h"
#include "Util.h"

int util::randomize(int min, int max)
{
	std::srand((unsigned) std::time(nullptr));
	return rand() % max + min;
}

float util::distance(const sf::Vector2f& from, const sf::Vector2f& to)
{
	return math::magnitude(to - from);
}

float util::distance(const sf::Vector2u& from, const sf::Vector2u& to)
{
	return util::distance(
		sf::Vector2f(static_cast<float>(from.x), static_cast<float>(from.y)),
		sf::Vector2f(static_cast<float>(to.x), static_cast<float>(to.y)));
}

std::string util::toString(const sf::Vector2u& position)
{
	std::ostringstream stream;
	stream << "(" << position.x << "," << position.y << ")";
	return std::string(stream.str());
}

std::string util::toString(const sf::Vector2f& position)
{
	std::ostringstream stream;
	stream << "(" << position.x << "," << position.y << ")";
	return std::string(stream.str());
}


void util::split(const std::string& source, char delimiter, std::vector<std::string> &elements)
{
	std::stringstream stream(source);
	std::string item;

	while (std::getline(stream, item, delimiter)) {
		elements.push_back(item);
	}
}

std::vector<std::string> util::split(const std::string& source, char delimiter)
{
	std::vector<std::string> elements;
	util::split(source, delimiter, elements);

	return elements;
}
