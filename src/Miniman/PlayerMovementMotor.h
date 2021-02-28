#ifndef PLAYERMOVEMENTMOTOR_H_
#define PLAYERMOVEMENTMOTOR_H_

#include "MovementMotor.h"

/// <summary>
/// Handles the player character movement and position.
/// </summary>
class PlayerMovementMotor : public MovementMotor
{
public:
	PlayerMovementMotor();
	~PlayerMovementMotor();

	/// <summary>
	/// Updates movement behaviour. Called once per frame.
	/// </summary>
	virtual void update();

	/// <summary>
	/// Sets the direction which the player character will target next
	/// once the current target tile has been reached.
	/// </summary>
	void setNextDirection(MoveDirection direction);

	/// <summary>
	/// Slows down the player character's speed for the given amount of time.
	/// </summary>
	void slowMovement(sf::Time time);

private:
	virtual MoveDirection getNextDirection();
	
private:
	MoveDirection nextDirection_;
	
	sf::Clock directionClock_;
	const sf::Time rememberDirection_;
	
	sf::Time slowDownTime_;
};

#endif