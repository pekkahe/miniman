#ifndef INKYCHASEBEHAVIOUR_H_
#define INKYCHASEBEHAVIOUR_H_

#include <vector>
#include <SFML/Graphics.hpp>
#include "BehaviourInterface.h"

class AI;
class EnemyMovementMotor;

/// <summary>
/// Defines AI chase behaviour for Inky; the least predictable of the ghosts.
/// </summary>
class InkyChaseBehaviour : public BehaviourInterface
{
public:
	InkyChaseBehaviour(EnemyMovementMotor& blinkyMotor);
	~InkyChaseBehaviour();

	/// <summary>
	/// Initializes the behaviour for the given AI.
	/// </summary>
	virtual void init(AI* ai);

	/// <summary>
	/// Acts according to behaviour. Attempts to move ahead of the player using
	/// the direction vector formed by Blinky's and the player's positions as a guide.
	/// </summary>
	virtual void act();

	/// <summary>
	/// Resets the behaviour.
	/// </summary>
	virtual void reset() {};

private:
	/// <summary>
	/// Returns the tile which should be targeted for movement.
	/// </summary>
	sf::Vector2u getTargetTile();

	sf::Vector2u getNearbyTile(const sf::Vector2u& position) const;

private:
	const EnemyMovementMotor& blinkyMotor_;
	AI* ai_;
};

#endif