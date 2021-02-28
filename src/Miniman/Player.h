#ifndef PLAYER_H_
#define PLAYER_H_

#include <SFML\Graphics.hpp>
#include "Character.h"

class PlayerMovementMotor;
class PlayerAnimatorController;

/// <summary>
/// Defines the player actor. Manages and acts as an interface to the character's state,
/// player input, movement and animation.
/// </summary>
class Player : public Character
{
public:
	Player();
	virtual ~Player();

	/// <summary>
	/// Updates player character behaviour. Called once per frame.
	/// </summary>
	virtual void update();
	
	/// <summary>
	/// Returns the forward vector of the player character.
	/// </summary>
	sf::Vector2f forward() const;

	/// <summary>
	/// Sets the player character's next movement direction.
	/// </summary>
	/// <remarks>
	/// The next movement direction is applied when a new tile is reached.
	/// </remarks>
	void moveNext(MoveDirection direction);

	/// <summary>
	/// Kills the player and restarts the level, or ends the game if the player has
	/// no more lives left.
	/// </summary>
	void die();

	/// <summary>
	/// Resets the player character state.
	/// </summary>
	virtual void reset();

	/// <summary>
	/// Sets the amount of lives the player has left.
	/// </summary>
	void lives(int count) { lives_ = count; }

	/// <summary>
	/// Returns the amount of lives the player has left.
	/// </summary>
	int lives() const { return lives_; }
	
private:
	PlayerMovementMotor& motor();
	PlayerAnimatorController& animator();

	/// <summary>
	/// Handles behaviour for new tiles the player has moved into.
	/// </summary>
	void handleNewTile(sf::Vector2u previousTile, sf::Vector2u currentTile);

	/// <summary>
	/// Sets a new target rotation for the character if the given directions differ.
	/// </summary>
	void rotatePlayer(sf::Vector2f previousDirection, sf::Vector2f currentDirection);

	/// <summary>
	/// Rotates the character towards the target rotation each frame.
	/// </summary>
	void rotatePerFrame();

	/// <summary>
	/// Returns true if this frame's movement was the first one made after restart.
	/// </summary>
	bool isFirstMove() const;

private:
	float rotation_;
	int targetOrientation_; // 1 = right, -1 = left
	float targetRotation_;
	float targetRelativeRotation_;
	int lives_;
	float rotationSpeed_;
	bool hasMoved_;
};

#endif