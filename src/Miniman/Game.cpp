#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "Locator.h"
#include "ResourceManager.h"
#include "AStar/AStar.h"
#include "Util.h"
#include "Game.h"
#include "Renderer.h"
#include "DebugTools.h"

Game::Game(sf::RenderWindow& window) :
	window_(window),
	score_(0),
	highScore_(0),
	gameState_(GAMESTATE_MAINMENU),
	restartTimer_(sf::Time::Zero),
	levelNumber_(1),
	dotsLeft_(1),
	majorVersion_(MAJOR_VERSION)
{ }

bool Game::init()
{
	debug().logMessages(DebugMessage::ERROR);
	debug().logMessages(DebugMessage::WARNING);
	debug().logMessages(DebugMessage::GENERAL);
	debug().logMessages(DebugMessage::LEVEL);

	// Provide game components to the service locator
	Locator::provide(&gui_);
	Locator::provide(&level_);
	Locator::provide(&player_);
	Locator::provide(&input_);
	Locator::provide(&enemyManager_);
	Locator::provide(&audioManager_);
	Locator::provide(&settings_);
	Locator::provide(&resourceManager_);
		
	if (!resourceManager_.init()) {
		debug().error("Failed to initialize resource manager.");
		return false;
	}

	// Load the level tile map
	int tileMapWidth;
	int tileMapHeight;
	int* tileMap = resourceManager_.loadTileMap(resourceManager_.getPath("level.lvl"), tileMapWidth, tileMapHeight);

	if (!level_.load(resourceManager_.getPath("level.png"), sf::Vector2u(16, 16), tileMap, tileMapWidth, tileMapHeight)) {
		debug().error("Failed to load level.");
		return false;
	}

	// Initialize path finder graph after level has been loaded
	astar::pathFinder().initialize(level_);

	// Initialize game view port before GUI but after level
	Locator::renderer()->initViewport(level_);

	// Load high score before GUI
	loadHighScore();

	// Initialize GUI after renderer has the game view port
	if (!gui_.init(resourceManager_.getPath("PalookaBB.ttf"), resourceManager_.getPath("player_life.png"))) {
		debug().error("Failed to initialize GUI.");
		return false;
	}

	if (!player_.loadSpriteSheet(resourceManager_.getPath("pacman.png"))) {
		debug().error("Failed to load player sprite sheet.");
		return false;
	}

	if (!enemyManager_.init()) {
		debug().error("Failed to initialize enemies.");
		return false;
	}

	if (!audioManager_.init()) {
		debug().error("Failed to initialize audio.");
		return false;
	}
	
	return true;
}

void Game::start()
{ 
	player_.reset();
	level_.reset();
	enemyManager_.reset();

	player_.lives(PLAYER_LIVES);
	score_ = 0;

	gui_.updateLivesLeft(player_.lives());
	gui_.updateScore(score_);

	#ifdef _DEBUG
		gui_.updateDotsLeft(level_.dotsLeft());
	#endif

	gameState_ = GAMESTATE_GAMEPLAY;
}

void Game::update(sf::Time deltaTime)
{
	deltaTime_ = deltaTime;

	// Read SFML window events such as keyboard input
	pollEvents();

	if (restartTimer_ > sf::Time::Zero)
	{
		restartTimer_ -= deltaTime;
		return;
	}

	// Update gameplay behaviour
	if (gameState_ == GAMESTATE_GAMEPLAY)
	{
		collision_.preDetect();

		player_.update();
		enemyManager_.update();

		collision_.detect();
	}

	lateUpdate();
}

void Game::updateHighScoreAndRestart()
{
	saveHighScore();
	gui_.updateHighScore();

	start();
}

void Game::restartLevel(float waitForSeconds)
{
	restartTimer_ = sf::seconds(waitForSeconds);

	enemyManager_.reset();
	player_.reset();

	#ifdef _DEBUG
		gui_.updateDotsLeft(level_.dotsLeft());
	#endif

	gameState_ = GAMESTATE_GAMEPLAY;
}

void Game::nextLevel(float waitForSeconds)
{
	debug().info("Level complete. Loading next level.", DebugMessage::LEVEL);

	audioManager_.playLevelCompleteSound(0.4f);

	levelNumber_++;
	
	settings_.increaseDifficulty(levelNumber_);

	restartTimer_ = sf::seconds(waitForSeconds);

	player_.reset();
	enemyManager_.reset();
	level_.reset();

	#ifdef _DEBUG
		gui_.updateDotsLeft(level_.dotsLeft());
	#endif

	gameState_ = GAMESTATE_GAMEPLAY;

	debug().info("Level loaded. Pausing for a short while.", DebugMessage::LEVEL);
}

void Game::pause(bool paused)
{
	// Restrict pausing to two game states
	if (gameState_ == GAMESTATE_PAUSED && !paused)
		gameState_ = GAMESTATE_GAMEPLAY;
	else if (gameState_ == GAMESTATE_GAMEPLAY && paused)
		gameState_ = GAMESTATE_PAUSED;
}

void Game::gameOver()
{
	gameState_ = GAMESTATE_GAMEOVER;

	// Write the current score to the Game Over menu
	auto menu = gui_.menu();
	if (menu != nullptr)
		menu->setSubtitle("Score: " + std::to_string(score_), 35, 20);

	audioManager_.playGameOverSound();
}

void Game::lateUpdate()
{
	// New levels need to be loaded on late update since we 
	// do initializations on each level load
	if (dotsLeft_ == 0) {
		dotsLeft_ = 1; // Prevent infinite loading loop

		nextLevel(2.f);
	}
}

void Game::pollEvents()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				quit();
				break;

			case sf::Event::KeyPressed:
				input_.read(event.key.code);
			
				#ifdef _DEBUG
					debug().readInput(event.key.code);
				#endif
				break;

			default:
				break;
		}
	}
}

void Game::quit()
{
	saveHighScore();

	astar::pathFinder().gameClosed();

	window_.close();
}

void Game::saveHighScore()
{
	if (score_ > highScore_) {
		highScore_ = score_;
		resourceManager_.saveHighScore(Locator::resource()->getPath("highscore.sc"), score_);
	}
}

void Game::loadHighScore()
{
	highScore_ = resourceManager_.loadHighScore(Locator::resource()->getPath("highscore.sc"));
}

void Game::dotEaten(ScoreModifier scoreModifer)
{
	updateScore(scoreModifer);

	if (scoreModifer == SCORE_ENERGIZER) {
		audioManager_.playEnergizerSound(0.4f);
	}

	// Store dot count so we can check it on late update
	dotsLeft_ = level_.dotsLeft();

	#ifdef _DEBUG
		gui_.updateDotsLeft(dotsLeft_);
	#endif
}

void Game::updateScore(ScoreModifier scoreModifer)
{
	switch (scoreModifer)
	{
		case SCORE_DOT:
			score_ += 10;
			break;
	
		case SCORE_ENERGIZER:
			score_ += 50;
			break;

		default:
			return;
	}

	gui_.updateScore(score_);
}

std::string Game::versionInformation()
{
	return "v " + majorVersion_ + "." + std::to_string(SVN_REVISION);
}

Game::~Game()
{ }