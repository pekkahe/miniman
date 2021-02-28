#ifndef DEADBEHAVIOUR_H_
#define DEADBEHAVIOUR_H_

#include <functional>
#include <SFML\Graphics.hpp>
#include "BehaviourInterface.h"

class AI;

/// <summary>
/// Defines the behaviour which is triggered when an enemy dies. The player can eat/kill
/// frightened enemies which will cause them to return to the ghost pen for respawning.
/// </summary>
class DeadBehaviour : public BehaviourInterface
{
	/// <summary>
	/// Defines the various states of a dead enemy.
	/// </summary>
	enum Action
	{
		ACTION_RETURNING,
		ACTION_WAITING,
		ACTION_SPAWNING
	};

public:
	DeadBehaviour(const sf::Vector2u& ghostPenPosition, const sf::Time& respawnTime);
	~DeadBehaviour();

	/// <summary>
	/// Initializes the behaviour for the given AI.
	/// </summary>
	virtual void init(AI* ai);

	/// <summary>
	/// Acts according to the behaviour. Triggers respawn if enough time has passed at
	/// the ghost pen.
	/// </summary>
	virtual void act();

	/// <summary>
	/// Resets the behaviour.
	/// </summary>
	virtual void reset();

private:
	void returnToGhostPen();
	void waitUntilRespawn();
	void beginSpawn();
	void endSpawn();
	void setOffset();
	void resetOffset();

private:
	AI* ai_;

	const sf::Vector2u ghostPenPosition_;
	const sf::Time respawnTime_;

	sf::Clock respawnClock_;
	Action action_;
};

#endif