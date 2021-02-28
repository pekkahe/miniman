#include <SFML/Graphics.hpp>
#include <iostream>
#include "Locator.h"
#include "Renderer.h"
#include "GUI.h"
#include "Level.h"
#include "Player.h"
#include "Game.h"

Renderer::Renderer(sf::RenderWindow& window) : 
	window_(window)
{ }

void Renderer::initViewport(const Level& level)
{
	// Set game view to show whole level
	gameView_.reset(sf::FloatRect(0, 0, level.screenSize().x, level.screenSize().y));

	// Show game view at the center of the window on its native scale
	float width = level.screenSize().x / SCREEN_WIDTH;
	float height = level.screenSize().y / SCREEN_HEIGHT;
	float left = (1 - width) * 0.5f;
	float top = (1 - height) * 0.5f;

	gameView_.setViewport(sf::FloatRect(left, top, width, height));
}

void Renderer::draw()
{
	// Not the nicest solution, but this prevent the player's rotation from showing
	// on game restarts.
	// NOTE: Needs to be removed if we want to show a player death animation.
	if (Locator::game()->isRestarting())
		return;

	window_.clear();
	
	switch (Locator::game()->gameState())
	{
		case GAMESTATE_GAMEPLAY:
		case GAMESTATE_PAUSED:
		case GAMESTATE_GAMEOVER:
			drawGameObjects();
			break;

		default:
			break;
	}

	window_.draw(*Locator::gui());

	window_.display();
}

void Renderer::drawGameObjects()
{
	// Switch to game view
	window_.setView(gameView_);

	// Draw game objects
	window_.draw(*Locator::level());
	window_.draw(*Locator::player());

	auto enemies = Locator::enemyManager()->enemies();
	for (std::size_t i = 0; i < enemies.size(); i++) {
		window_.draw(*enemies[i]);
	}

	// Restore original view
	window_.setView(window_.getDefaultView());
}

sf::Vector2u Renderer::getWindowSize() const
{ 
	return window_.getSize();
}

sf::IntRect Renderer::getGameViewRect() const
{
	auto viewport(gameView_.getViewport()); // In range 0..1
	auto size(gameView_.getSize()); // In pixels

	float left = viewport.left * SCREEN_WIDTH;
	float top = viewport.top * SCREEN_HEIGHT;

	return sf::IntRect(
		static_cast<int>(left),
		static_cast<int>(top), 
		static_cast<int>(size.x),
		static_cast<int>(size.y));
}

Renderer::~Renderer()
{ }