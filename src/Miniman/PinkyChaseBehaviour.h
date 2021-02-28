#ifndef PINKYCHASEBEHAVIOUR_H_
#define PINKYCHASEBEHAVIOUR_H_

#include <SFML/Graphics.hpp>
#include "BehaviourInterface.h"

class AI;

/// <summary>
/// Defines AI chase behaviour for Pinky; the ghost which attempts to box in the player.
/// </summary>
class PinkyChaseBehaviour : public BehaviourInterface
{
public:
	PinkyChaseBehaviour();
	~PinkyChaseBehaviour();

	/// <summary>
	/// Initializes the behaviour for the given AI.
	/// </summary>
	virtual void init(AI* ai);

	/// <summary>
	/// Acts according to behaviour. Attempts to move to a position a few tiles 
	/// ahead of the player's position.
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
	virtual sf::Vector2u getTargetTile();

private:
	AI* ai_;
};


#endif