#ifndef FRIGHTENEDBEHAVIOUR_H_
#define FRIGHTENEDBEHAVIOUR_H_

#include <SFML\Graphics.hpp>
#include "BehaviourInterface.h"

class AI;

/// <summary>
/// Defines the behaviour which is triggered when the player eats an energizer.
/// Enemies will move slower and aimlessly around, allowing the player to eat them.
/// </summary>
class FrightenedBehaviour : public BehaviourInterface
{
public:
	FrightenedBehaviour(sf::Time behaviourTime);
	~FrightenedBehaviour();

	/// <summary>
	/// Initializes the behaviour for the given AI.
	/// </summary>
	virtual void init(AI* ai);

	/// <summary>
	/// Acts according to the behaviour. Switches to scatter mode  if enough time
	/// has passed.
	/// </summary>
	virtual void act();

	/// <summary>
	/// Resets the behaviour.
	/// </summary>
	virtual void reset();

private:
	AI* ai_;

	const sf::Time behaviourTime_;
	sf::Clock clock_;
};

#endif

