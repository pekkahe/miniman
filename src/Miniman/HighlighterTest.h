#ifndef HIGHLIGHTERTEST_H_
#define HIGHLIGHTERTEST_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

enum HighlightColor
{
	GREY = 42,
	YELLOW = 43,
	RED = 44
};

class Level;
enum Tile;

namespace tests
{
	class HighlighterTest
	{
	public:
		HighlighterTest(Level* const level);
		~HighlighterTest();

		void highlight(const sf::Vector2u& position, HighlightColor color);
		void clear(HighlightColor color);

	private:
		Level* const level_;
		
		std::map<unsigned int, HighlightColor> highlighted_;
		std::map<unsigned int, unsigned int> preHighlight_;
	};

	HighlighterTest& highlighter();
	void highlight(sf::Vector2u position, HighlightColor color);
}

#endif