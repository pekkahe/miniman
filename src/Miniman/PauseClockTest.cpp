#include <thread>
#include <iostream>
#include <vector>
#include "PauseClockTest.h"

using namespace tests;

PauseClockTest::PauseClockTest()
{ }

void PauseClockTest::run()
{
	sf::Clock clock;

	clock.restart();
	pauseClock_.restart();

	int hits = 0;
	while (true)
	{
		sf::Time elapsed = clock.getElapsedTime();

		std::cout << "Time " << (int) clock.getElapsedTime().asSeconds() << "s. ";
		std::cout << "Pause clock: " << (int) pauseClock_.getElapsedTime().asSeconds() << "s." << std::endl;

		if (elapsed > sf::seconds(2) && hits == 0) {
			pause(); 
			hits++;
		}
		if (elapsed > sf::seconds(3) && hits == 1) {
			pause();
			hits++;
		}
		if (elapsed > sf::seconds(7) && hits == 2) {
			unpause();
			hits++;
		}
		if (elapsed > sf::seconds(9) && hits == 3) {
			unpause();
			hits++;
		}
		if (elapsed > sf::seconds(10) && hits == 4) {
			restart();
			hits++;
		}
		if (elapsed > sf::seconds(13) && hits == 5) {
			pause();
			hits++;
		}
		if (elapsed > sf::seconds(15) && hits == 6) {
			unpause();
			hits++;
		}
		if (elapsed > sf::seconds(20)) {
			break;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	std::cout << "Tests finished." << std::endl;

	while (true)
	{ }
}

void PauseClockTest::pause()
{
	pauseClock_.pause(true);
	std::cout << "Pause clock paused." << std::endl;
}

void PauseClockTest::unpause()
{
	pauseClock_.pause(false);
	std::cout << "Pause clock unpaused." << std::endl;
}

void PauseClockTest::restart()
{
	pauseClock_.restart();
	std::cout << "Pause restarted." << std::endl;
}

PauseClockTest::~PauseClockTest()
{ }
