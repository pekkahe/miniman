#ifndef BLINKYCHASEBEHAVIOUR_H_
#define BLINKYCHASEBEHAVIOUR_H_

#include <SFML/Graphics.hpp>
#include "BehaviourInterface.h"

class AI;

/// <summary>
/// Defines AI chase behavour for Blinky; the most aggressive ghost.
/// </summary>
class BlinkyChaseBehaviour : public BehaviourInterface
{
public:
	BlinkyChaseBehaviour();
	~BlinkyChaseBehaviour();

	/// <summary>
	/// Initializes the behaviour for the given AI.
	/// </summary>
	virtual void init(AI* ai);

	/// <summary>
	/// Acts according to behaviour. Attempts to catch the player.
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

private:
	AI* ai_;
};

#endif