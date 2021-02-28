#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <exception>
#include <string>
#include <SFML\Graphics.hpp>
#include "DebugTools.h"
#include "Util.h"
#include "Math.h"
#include "Locator.h"

#ifdef _DEBUG
//#define DISABLE_ENEMIES
//#define GOD_MODE
#endif

// Add a Visual Studio compliant debug breaker macro
#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak()
#endif

typedef unsigned int uint;

/// <summary>
/// The game's title.
/// </summary>
const std::string GAME_TITLE = "Miniman";

/// <summary>
/// The current major version of the game.
/// </summary>
const std::string MAJOR_VERSION = "1.1";

/// <summary>
/// The width of the game window in pixels.
/// </summary>
const int SCREEN_WIDTH = 800;

/// <summary>
/// The height of the game window in pixels.
/// </summary>
const int SCREEN_HEIGHT = 600;

/// <summary>
/// How many lives does the player start with.
/// </summary>
const int PLAYER_LIVES = 5;

/// <summary>
/// The player's normal movement animation speed in milliseconds.
/// </summary>
const int PLAYER_ANIMATION_INTERVAL_MS = 70;

/// <summary>
/// The player's fast movement animation speed in milliseconds.
/// </summary>
const int PLAYER_FAST_ANIMATION_INTERVAL_MS = 20;

/// <summary>
/// The enemy's movement animation speed in milliseconds.
/// </summary>
const int ENEMY_ANIMATION_INTERVAL_MS = 80;

/// <summary>
/// How long the player's movement is slowed down after eating a dot.
/// </summary>
const int PLAYER_EAT_SLOWDOWN_MS = 200;

/// <summary>
/// The player's rotation speed modifier.
/// </summary>
const float PLAYER_ROTATION_SPEED = 0.5f;

/// <summary>
/// Indicates an invalid position which doesn't exist.
/// </summary>
const sf::Vector2u NO_SUCH_POSITION = sf::Vector2u(999, 999);

/// <summary>
/// Indicates an array index out of bounds error.
/// </summary>
class OutOfBoundsException : public std::exception
{
public:
	OutOfBoundsException(int index = 0) : index_(index) { }

	virtual const char* what() const throw()
	{
		if (index_ != 0) {
			std::string error = "Index [";
			error += index_;
			error += "] is out of bounds.";
			return error.c_str();
		} else {
			return "Index is out of bounds.";
		}
	}

private:
	int index_;
};

/// <summary>
/// Defines the available movement directions for actors.
/// </summary>
enum MoveDirection 
{
	NONE,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_RIGHT,
	MOVE_LEFT 
};

/// <summary>
/// Defines the available score modifiers in the game.
/// </summary>
enum ScoreModifier 
{ 
	SCORE_DOT,
	SCORE_ENERGIZER
};

#endif