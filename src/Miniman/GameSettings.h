#ifndef GAME_SETTINGS_H_
#define GAME_SETTINGS_H_

#include <SFML\System.hpp>

/// <summary>
/// Defines the game's dynamic settings based on difficulty.
/// </summary>
class GameSettings
{
public:
	GameSettings();
	~GameSettings();

	/// <summary>
	/// Increases game difficulty by adjusting the settings based on the given level number.
	/// </summary>
	void increaseDifficulty(int level);
	
	/// <summary>
	/// Returns the designated player movement speed.
	/// </summary>
	float playerSpeed() const;
	
	/// <summary>
	/// Returns the designated player movement speed when eating dots.
	/// </summary>
	float playerEatSpeed() const;
	
	/// <summary>
	/// Returns the designated enemy movement speed.
	/// </summary>
	float enemySpeed() const;
	
	/// <summary>
	/// Returns the designated enemy movement speed when frightened.
	/// </summary>
	float enemyFrightenedSpeed() const;
	
	/// <summary>
	/// Returns the designated enemy movement speed when dead.
	/// </summary>
	float enemyDeadSpeed() const;

	/// <summary>
	/// Returns the designated enemy frightened time.
	/// </summary>
	sf::Time frightTime() const;

private:
	float playerSpeed_;
	sf::Time frightTime_;
};

#endif