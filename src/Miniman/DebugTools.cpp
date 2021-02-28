#include <iostream>
#include <sstream>
#include "DebugTools.h"
#include "AStar/Tests.h"
#include "Player.h"
#include "Enemy.h"
#include "Game.h"
#include "Locator.h"

DebugTools& debug()
{
	static DebugTools debug;
	return debug;
}

DebugTools::DebugTools() :
	flags_(0)
{ }

void DebugTools::logMessages(DebugMessage messageType)
{
	flags_.set(messageType);
}

void DebugTools::log(const std::string& message, DebugMessage messageType)
{ 
	if (flags_.test(messageType)) {
		auto time = clock_.getElapsedTime();
		std::cout << std::to_string((int)time.asSeconds()) << "s: " << message << std::endl;
	}
}

void DebugTools::info(const std::string& message, DebugMessage messageType)
{
	log("INFO: " + message, messageType);
}

void DebugTools::error(const std::string& message)
{
	log("ERROR: " + message, DebugMessage::ERROR);
}

void DebugTools::warning(const std::string& message)
{
	log("WARNING: " + message, DebugMessage::WARNING);
}

void DebugTools::readInput(const sf::Keyboard::Key keyCode)
{ 
	switch (keyCode)
	{
		case sf::Keyboard::Num1:
			{
				info("Running A* tests", DebugMessage::GENERAL);

				astar::Tests tests;
				tests.run();
			}
			break;

		case sf::Keyboard::Num2:
			{
				info("Killing enemies", DebugMessage::GENERAL);

				auto enemies = Locator::enemyManager()->enemies();
				for (auto it = enemies.begin(); it < enemies.end(); ++it) {
					(*it)->die(sf::seconds(2));
				}
			}
			break;
		
		case sf::Keyboard::Num3:
			{
				info("Killing player", DebugMessage::GENERAL);

				Locator::player()->die();
			}
			break;

		default:
			break;
	}
}

DebugTools::~DebugTools()
{ }
