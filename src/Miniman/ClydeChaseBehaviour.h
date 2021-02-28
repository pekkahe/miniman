#ifndef CLYDECHASEBEHAVIOUR_H_
#define CLYDECHASEBEHAVIOUR_H_

#include <vector>
#include "BehaviourInterface.h"

class AI;

/// <summary>
/// Defines AI chase behaviour for Clyde; the least dangerous ghost.
/// </summary>
class ClydeChaseBehaviour : public BehaviourInterface
{
public:
	ClydeChaseBehaviour(const std::vector<sf::Vector2u>& scatterTargets);
	~ClydeChaseBehaviour();

	/// <summary>
	/// Initializes the behaviour for the given AI.
	/// </summary>
	virtual void init(AI* ai);

	/// <summary>
	/// Acts according to behaviour. Follows the player but moves away if
	/// the player gets too close.
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

	sf::Vector2u nextScatterTarget();

private:
	AI* ai_;

	const std::vector<sf::Vector2u> scatterTargets_;
	unsigned int current_;
};

#endif