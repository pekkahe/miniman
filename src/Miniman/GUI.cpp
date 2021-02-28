#include <iostream>
#include <string>
#include "Locator.h"
#include "ResourceManager.h"
#include "GUI.h"
#include "GUIMenu.h"
#include "Game.h"
#include "SvnVersionInfo.h"
#include "Player.h"
#include "Renderer.h"
#include "Level.h"

bool GUI::init(const std::string& fontFile, const std::string& playerTextureFile)
{
	if (!Locator::resource()->loadFont(fontFile, &font_)) {
		debug().error("Failed to load font " + fontFile);
		return false;
	}

	if (!Locator::resource()->loadTexture(playerTextureFile, &playerTexture_)) {
		debug().error("Failed to load texture " + playerTextureFile);
		return false;
	}

	// Initialize game menus
	mainMenu_.loadFont(fontFile);
	mainMenu_.setWindowSize(Locator::renderer()->getWindowSize());
	mainMenu_.setTitle(GAME_TITLE, 100, 20);
	mainMenu_.addItem("Play", &Game::start);
	mainMenu_.addItem("Quit", &Game::quit);
	mainMenu_.addText("Programming / SFX", sf::Vector2f(100, 500), 20, sf::Color(200, 200, 200));
	mainMenu_.addText("Pekka Hellsten", sf::Vector2f(115, 530), 20, sf::Color::White);
	mainMenu_.addText("Graphics", sf::Vector2f(620, 500), 20, sf::Color(200, 200, 200));
	mainMenu_.addText("Antti Hellsten", sf::Vector2f(600, 530), 20, sf::Color::White);
	mainMenu_.addText(Locator::game()->versionInformation(), sf::Vector2f(375, 500), 15, sf::Color(200, 200, 200));

	pauseMenu_.loadFont(fontFile);
	pauseMenu_.setWindowSize(Locator::renderer()->getWindowSize());
	pauseMenu_.setBackground(sf::Color(0, 0, 0, 150));
	pauseMenu_.setTitle("Paused", 80, 100);
	pauseMenu_.addItem("Continue", &Game::unpause);
	pauseMenu_.addItem("Restart", &Game::updateHighScoreAndRestart);
	pauseMenu_.addItem("Quit", &Game::quit);

	gameOverMenu_.loadFont(fontFile);
	gameOverMenu_.setWindowSize(Locator::renderer()->getWindowSize());
	gameOverMenu_.setBackground(sf::Color(0, 0, 0, 150));
	gameOverMenu_.setTitle("Game Over", 80, 60);
	gameOverMenu_.addItem("Restart", &Game::updateHighScoreAndRestart);
	gameOverMenu_.addItem("Quit", &Game::quit);

	// Get game view rect so we can position GUI elements properly
	auto gameView(Locator::renderer()->getGameViewRect());

	// Initialize text elements
	highScoreTitle_.setFont(font_);
	highScoreTitle_.setCharacterSize(20); // In pixels, not points!
	highScoreTitle_.setColor(sf::Color::White);
	highScoreTitle_.setString("High score");
	highScoreTitle_.setPosition(sf::Vector2f(
		static_cast<float>(gameView.left + (gameView.width / 2) - (highScoreTitle_.getLocalBounds().width / 2)),
		static_cast<float>(gameView.top - 50)));

	highScoreValue_.setFont(font_);
	highScoreValue_.setCharacterSize(20);
	highScoreValue_.setColor(sf::Color::White);
	highScoreValue_.setString(std::to_string(Locator::game()->highScore()));
	highScoreValue_.setPosition(sf::Vector2f(
		static_cast<float>(highScoreTitle_.getGlobalBounds().left + (highScoreTitle_.getGlobalBounds().width / 2) -
					      (highScoreValue_.getLocalBounds().width / 2)),
		static_cast<float>(gameView.top - 30)));

	playerScoreTitle_.setFont(font_);
	playerScoreTitle_.setCharacterSize(20);
	playerScoreTitle_.setColor(sf::Color::White);
	playerScoreTitle_.setString("Player");
	playerScoreTitle_.setPosition(sf::Vector2f(
		static_cast<float>(gameView.left + 10),
		static_cast<float>(gameView.top - 50)));

	playerScoreValue_.setFont(font_);
	playerScoreValue_.setCharacterSize(20);
	playerScoreValue_.setColor(sf::Color::White);
	playerScoreValue_.setString(std::to_string(0));
	playerScoreValue_.setPosition(sf::Vector2f(
		static_cast<float>(playerScoreTitle_.getGlobalBounds().left + (playerScoreTitle_.getGlobalBounds().width / 2) - 
						  (playerScoreValue_.getLocalBounds().width / 2)),
		static_cast<float>(gameView.top - 30)));


	#ifdef _DEBUG
		dotsLeft_.setFont(font_);
		dotsLeft_.setCharacterSize(20);
		dotsLeft_.setColor(sf::Color::White);
		dotsLeft_.setString("Dots:");
		dotsLeft_.setPosition(sf::Vector2f(
			static_cast<float>(gameView.left + gameView.width - (dotsLeft_.getGlobalBounds().width * 2)),
			static_cast<float>(gameView.top - 50)));
	#endif

	// Initialize the texture for the remaining player lives indicator
	playerTexture_.setRepeated(true);
	
	int maxLives = Locator::player()->lives();
	livesLeft_.setTexture(playerTexture_);
	livesLeft_.setPosition(sf::Vector2f(
		static_cast<float>(gameView.left + gameView.width - (maxLives * playerTexture_.getSize().x)),
		static_cast<float>(gameView.height + gameView.top)));

	updateLivesLeft(maxLives);

	return true;
}

void GUI::updateHighScore()
{
	// Update the displayed value and the position so the text remains centered
	highScoreValue_.setString(std::to_string(Locator::game()->highScore()));
	highScoreValue_.setPosition(sf::Vector2f(
		highScoreTitle_.getGlobalBounds().left +
		(highScoreTitle_.getGlobalBounds().width / 2) -
		(highScoreValue_.getLocalBounds().width / 2), highScoreValue_.getPosition().y));
}

void GUI::updateScore(int score)
{
	// Update the displayed value and the position so the text remains centered
	playerScoreValue_.setString(std::to_string(score));
	playerScoreValue_.setPosition(sf::Vector2f(
		playerScoreTitle_.getGlobalBounds().left +
		(playerScoreTitle_.getGlobalBounds().width / 2) -
		(playerScoreValue_.getLocalBounds().width / 2), playerScoreValue_.getPosition().y));
}

void GUI::updateLivesLeft(int lives)
{
	livesLeft_.setTextureRect(sf::IntRect(
		0, 0, playerTexture_.getSize().x * lives, playerTexture_.getSize().y));
}

void GUI::updateDotsLeft(int dots)
{ 
	dotsLeft_.setString("Dots: " + std::to_string(dots));
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	switch (Locator::game()->gameState())
	{
		case GAMESTATE_MAINMENU:
			target.draw(mainMenu_, states);
			break;
		
		case GAMESTATE_GAMEPLAY:
			drawGameplayGui(target, states);
			break;

		case GAMESTATE_PAUSED:
			drawGameplayGui(target, states);
			target.draw(pauseMenu_, states);
			break;

		case GAMESTATE_GAMEOVER:
			drawGameplayGui(target, states);
			target.draw(gameOverMenu_, states);
			break;

		default:
			break;
	}
}

void GUI::drawGameplayGui(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(highScoreTitle_, states);
	target.draw(highScoreValue_, states);
	target.draw(playerScoreTitle_, states);
	target.draw(playerScoreValue_, states);
	target.draw(livesLeft_, states);

	#ifdef _DEBUG
		target.draw(dotsLeft_, states);
	#endif
}

GUIMenu* GUI::menu()
{
	switch (Locator::game()->gameState())
	{
		case GAMESTATE_MAINMENU:
			return &mainMenu_;

		case GAMESTATE_PAUSED:
			return &pauseMenu_;

		case GAMESTATE_GAMEOVER:
			return &gameOverMenu_;

		default: 
			return nullptr;
	}
}
