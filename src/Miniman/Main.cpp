#include <SFML/Graphics.hpp>
#include <iostream>
#include "Global.h"
#include "Level.h"
#include "Game.h"
#include "Renderer.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_TITLE);

	Game game(window);
	Locator::provide(&game);
	
	Renderer renderer(window);
	Locator::provide(&renderer);

	// Initialize game or exit if failed
	if (!game.init())
	{
		std::cout << "Press Enter to exit." << std::endl;
		std::cin.get();
		return -1;
	}

	// Set constant 30 fps
	const sf::Time deltaTime = sf::seconds(1.0f / 30.0f);
	sf::Clock clock;

	// Enter game loop
    while (window.isOpen())
    {
		sf::Time elapsed = clock.getElapsedTime();

		if (elapsed >= deltaTime)
		{
			// Update game behaviour
			game.update(elapsed);
			
			// Draw frame
			renderer.draw();

			clock.restart();
		}
    }

    return 0;
}
