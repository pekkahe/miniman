#ifndef GAME_H_
#define GAME_H_

#include <map>
#include <string>
#include <vector>
#include "GUI.h"
#include "Level.h"
#include "Input.h"
#include "Player.h"
#include "Global.h"
#include "EnemyManager.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "CollisionDetection.h"
#include "GameSettings.h"

namespace sf { class RenderWindow; }

enum GameState
{
	GAMESTATE_MAINMENU,
	GAMESTATE_GAMEPLAY,
	GAMESTATE_PAUSED,
	GAMESTATE_GAMEOVER
};

/// <summary>
/// Game manager responsible for initializing game components
/// and updating their behaviours.
/// </summary>
class Game
{
public:
	Game(sf::RenderWindow& window);
	~Game();

	/// <summary>
	/// Initializes game components and returns true if successful.
	/// </summary>
	bool init();

	/// <summary>
	/// Starts the game behaviour for a level.
	/// </summary>
	void start();

	/// <summary>
	/// Updates game behaviour. Called once per frame.
	/// </summary>
	void update(sf::Time deltaTime);

	/// <summary>
	/// Updates the player score according to the given modifier.
	/// </summary>
	void updateScore(ScoreModifier scoreModifer);
	
	/// <summary>
	/// Updates the player high score and restarts the current level.
	/// </summary>
	void updateHighScoreAndRestart();

	/// <summary>
	/// Restarts the current level after the specified delay.
	/// </summary>
	void restartLevel(float waitForSeconds);

	/// <summary>
	/// Moves to the next level after the specified delay.
	/// </summary>
	void nextLevel(float waitForSeconds);

	/// <summary>
	/// Saves the high score and quits the game.
	/// </summary>
	void quit();

	/// <summary>
	/// Notifies the game that a dot has been eaten with the given score and
	/// handles the event.
	/// </summary>
	void dotEaten(ScoreModifier scoreModifer);

	/// <summary>
	/// Notifies the game that the player has no more lives left and it should end.
	/// </summary>
	void gameOver();

	/// <summary>
	/// Pauses or unpauses the game.
	/// </summary>
	void pause(bool paused = true);

	/// <summary>
	/// Unpauses the game.
	/// </summary>
	void unpause() { pause(false); }

	/// <summary>
	/// Returns true if the game is currently delayed to restart.
	/// </summary>
	bool isRestarting() const { return restartTimer_ > sf::Time::Zero; }
	
	/// <summary>
	/// Returns the time elapsed from the last game update.
	/// </summary>
	sf::Time deltaTime() const { return deltaTime_; }
	
	/// <summary>
	/// Returns the current player score.
	/// </summary>
	unsigned int score() { return score_; }
	
	/// <summary>
	/// Returns the all-time player high score.
	/// </summary>
	unsigned int highScore() { return highScore_; }
	
	/// <summary>
	/// Returns the current level number.
	/// </summary>
	unsigned int levelNumber() { return levelNumber_; }
	
	/// <summary>
	/// Returns the current game state.
	/// </summary>
	GameState gameState() { return gameState_; }
	
	/// <summary>
	/// Returns the game version information.
	/// </summary>
	std::string versionInformation();
		
private:
	void pollEvents();
	void lateUpdate();
	void checkCollisions();
	void saveHighScore();
	void loadHighScore();

private:
	GUI gui_;
	Level level_;
	Input input_;
	Player player_;
	EnemyManager enemyManager_;
	ResourceManager resourceManager_;
	AudioManager audioManager_;
	GameSettings settings_;
	CollisionDetection collision_;
	
	GameState gameState_;
	sf::Time deltaTime_;
	sf::Time restartTimer_;

	unsigned int levelNumber_;
	unsigned int dotsLeft_;
	unsigned int score_;
	unsigned int highScore_;
	
	const std::string majorVersion_;

	sf::RenderWindow& window_;
};

#endif