#ifndef SCATTERBEHAVIOUR_H_
#define SCATTERBEHAVIOUR_H_

#include <vector>
#include "BehaviourInterface.h"

class AI;

/// <summary>
/// Defines the behaviour which is used as the default behaviour for the enemies.
/// Enemies move between their designated scatter tiles until triggered to do otherwise.
/// </summary>
class ScatterBehaviour : public BehaviourInterface
{
public:
	ScatterBehaviour(const std::vector<sf::Vector2u>& targets);
	~ScatterBehaviour();

	/// <summary>
	/// Initializes the behaviour for the given AI.
	/// </summary>
	virtual void init(AI* ai);

	/// <summary>
	/// Acts according to behaviour. Moves between the designated scatter tiles,
	/// ignoring the player.
	/// </summary>
	virtual void act();

	/// <summary>
	/// Resets the behaviour.
	/// </summary>
	virtual void reset() {};

private:
	void findNextTarget();

private:
	AI* ai_;

	const std::vector<sf::Vector2u> targets_;
	unsigned int current_;
};

#endif
