#ifndef ENEMYMANAGER_H_
#define ENEMYMANAGER_H_

#include <vector>
#include <SFML\Graphics.hpp>
#include "Enemy.h"

class Player;

/// <summary>
/// Game component responsible for enemy management and collectively
/// triggering their behaviours.
/// </summary>
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	/// <summary>
	/// Initializes all enemies in the game and returns true if successful.
	/// </summary>
	bool init();
	
	/// <summary>
	/// Updates each enemy's game behaviour. Called once per frame.
	/// </summary>
	void update();
	
	/// <summary>
	/// Resets each enemy's game behaviour.
	/// </summary>
	void reset();

	/// <summary>
	/// Sets enemies to a state where they try to catch or follow the player.
	/// </summary>
	void chase();

	/// <summary>
	/// Sets enemies to a state where they move between a set of designated tiles.
	/// </summary>
	void scatter();

	/// <summary>
	/// Sets enemies to a state where they move aimlessly around.
	/// </summary>
	void frightened();

	/// <summary>
	/// Returns the enemies.
	/// </summary>
	const std::vector<Enemy*>& enemies();

	/// <summary>
	/// Returns a pointer to the enemy with the given name,
	/// or <c>nullptr</c> if no such enemy exists.
	/// </summary>
	Enemy* enemy(const std::string& name);

private:
	std::vector<Enemy*> enemies_;

	// Individual enemy pointers provided for conveniance
	Enemy* blinky_;
	Enemy* inky_;
	Enemy* pinky_;
	Enemy* clyde_;
};

#endif
