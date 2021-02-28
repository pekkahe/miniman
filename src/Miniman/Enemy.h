#ifndef ENEMY_H_
#define ENEMY_H_

#include <string>
#include "Character.h"

enum AiState;
class AI;
class EnemyMovementMotor;
class EnemyAnimatorController;

/// <summary>
/// Defines an enemy actor. Manages and acts as an interface to the character's state,
/// AI, movement and animation.
/// </summary>
class Enemy : public Character
{
public:
	Enemy(const std::string& name, AI* ai, sf::Time spawnTime, MoveDirection spawnMovement);
	Enemy(const Enemy& other);
	virtual ~Enemy();
	
	/// <summary>
	/// Updates enemy behaviour. Called once per frame.
	/// </summary>
	virtual void update();
	
	/// <summary>
	/// Returns the enemy's name.
	/// </summary>
	const std::string& name() const { return name_; }
	
	/// <summary>
	/// Returns true if the enemy is frightened and moving aimlessly around.
	/// </summary>
	bool isFrightened() const;

	/// <summary>
	/// Returns true if the enemy is dead and returning to the ghost pen.
	/// </summary>
	bool isDead() const;

	/// <summary>
	/// Sets the two tiles which the enemy will try to reach during scatter behaviour.
	/// </summary>
	void scatterTiles(const sf::Vector2u& tile1, const sf::Vector2u& tile2);

	/// <summary>
	/// Returns the two tiles which the enemy will try to reach during scatter behaviour.
	/// </summary>
	const std::vector<sf::Vector2u>& scatterTiles() const;

	/// <summary>
	/// Sets the tile in the ghost pen which the enemy will move to once dead.
	/// </summary>
	void ghostPenTile(const sf::Vector2u& tile);

	/// <summary>
	/// Returns the tile in the ghost pen which the enemy will move to once dead.
	/// </summary>
	const sf::Vector2u ghostPenTile() const;

	/// <summary>
	/// Switches the enemy's behaviour to a state where he ignores the player and moves
	/// between a set of designated tiles.
	/// </summary>
	void scatter();

	/// <summary>
	/// Switches the enemy's behaviour to a state where he moves slowly and aimlessly
	/// around for a short period.
	/// </summary>
	void frightened();

	/// <summary>
	/// Switches the enemy's behaviour to a state where he tries to catch or follow the player.
	/// </summary>
	void chase();

	/// <summary>
	/// Switches the enemy's behaviour to a state where he returns to the ghost pen to spawn.
	/// </summary>
	void die(const sf::Time& respawnTime = sf::seconds(0));

	/// <summary>
	/// Moves the enemy instantly to the ghost pen.
	/// </summary>
	void moveToGhostPen();

	/// <summary>
	/// Resets enemy state.
	/// </summary>
	virtual void reset();

	/// <summary>
	/// Returns the movement manager for the enemy.
	/// </summary>
	EnemyMovementMotor& motor();

private:
	EnemyAnimatorController& animator();

private:
	AI* ai_;
	sf::Time elapsed_;
	
	std::string name_;
	std::vector<sf::Vector2u> scatterTargets_;
	sf::Vector2u ghostPenPosition_;
	sf::Time spawnTime_;
	MoveDirection spawnMovement_;
};

#endif
