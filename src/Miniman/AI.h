#ifndef AI_H_
#define AI_H_

#include <vector>
#include <SFML/Graphics.hpp>
#include <mutex>
#include "Enemy.h"
#include "AStar/AStar.h"
#include "BehaviourSchedule.h"

/// <summary>
/// Defines the various enemy AI states.
/// </summary>
enum AiState
{
	CHASING,
	SCATTERING,
	FRIGHTENED,
	DEAD
};

class EnemyMovementMotor;
class BehaviourInterface;

/// <summary>
/// Base class for enemy character AI. Implements an AStar client interface
/// to enable pathfinding.
/// </summary>
class AI : public astar::ClientInterface
{
	friend class BehaviourSchedule;

public:
	AI();
	AI(const AI& other);
	virtual ~AI();
	virtual AI* clone() const = 0;

	/// <summary>
	/// Updates AI behaviour.
	/// </summary>
	void tick();

	/// <summary>
	/// Initializes AI for the given enemy.
	/// </summary>
	void init(Enemy* enemy);

	/// <summary>
	/// Returns the enemy character associated with this AI.
	/// </summary>
	Enemy* enemy();

	/// <summary>
	/// Returns the enemy movement motor associated with this AI.
	/// </summary>
	EnemyMovementMotor* motor();
	
	/// <summary>
	/// Finds the shortest path to the given position using non-blocking AStar pathfinder.
	/// </summary>
	void findPath(sf::Vector2u position, bool preventBackTracking = true);

	/// <summary>
	/// Called by AStar pathfinder from its worker thread once path finding is complete.
	/// </summary>
	void pathFound(astar::Path* path);

	/// <summary>
	/// Clears the current path, if any, and aborts any running pathfinder.
	/// </summary>
	void clearPath();

	/// <summary>
	/// Returns true if the enemy has traversed his current path.
	/// </summary>
	bool atPathEnd() const;

	/// <summary>
	/// Returns true if the enemy has a path to follow.
	/// </summary>
	bool hasPath() const;

	/// <summary>
	/// Returns true if path finding has been initialized and currently running.
	/// </summary>
	bool isSearchingForPath() const { return isSearchingForPath_; }

	/// <summary>
	/// Switches AI behaviour to scatter mode.
	/// </summary>
	virtual void scatter();

	/// <summary>
	/// Switches AI behaviour to frightened mode.
	/// </summary>
	virtual void frightened();

	/// <summary>
	/// Switches AI behaviour to chase mode.
	/// </summary>
	virtual void chase() = 0;

	/// <summary>
	/// Switches AI behaviour to return quickly to the ghost pen and wait there
	/// for the given respawn time.
	/// </summary>
	void die(const sf::Time& respawnTime);

	/// <summary>
	/// Resets AI behaviour.
	/// </summary>
	void resetBehaviour();

	/// <summary>
	/// Returns the AI behaviour schedule used to time behaviour changes.
	/// </summary>
	BehaviourSchedule* schedule() { return &behaviourSchedule_; }

	/// <summary>
	/// Returns the active AI behaviour state.
	/// </summary>
	AiState state() const { return state_; }

public:
	const sf::Time tickTime;

protected:
	virtual void chase(BehaviourInterface* behaviour);
	void setBehaviour(BehaviourInterface* behaviour);

protected:
	AiState state_;

private:
	Enemy* enemy_;
	BehaviourInterface* behaviour_;
	BehaviourSchedule behaviourSchedule_;
	
	astar::Path* path_;
	std::mutex mutex_;
	bool isSearchingForPath_;
};

#endif