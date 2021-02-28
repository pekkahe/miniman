#ifndef BEHAVIOURSCHEDULE_H_
#define BEHAVIOURSCHEDULE_H_

#include <vector>
#include <SFML\Graphics.hpp>
#include "PauseClock.h"

class AI;

/// <summary>
/// Defines a scheduling system for automatic AI behaviour switching. Only
/// switches between the scatter and chase modes, leaving the frightened and
/// dead behaviours to be triggered by other means.
/// </summary>
class BehaviourSchedule
{
public:
	BehaviourSchedule();
	~BehaviourSchedule();

	/// <summary>
	/// Updates the scheduler for the given AI and switches its behaviour
	/// if enough time has passed.
	/// </summary>
	void update(AI* ai);

	/// <summary>
	/// Pauses or unpauses the scheduler.
	/// </summary>
	void pause(bool paused);

	/// <summary>
	/// Restarts the timer for the current behaviour.
	/// </summary>
	void restartCurrent();

	/// <summary>
	/// Resets the behaviour schedule completely and starts from the beginning.
	/// </summary>
	void reset();

private:
	PauseClock clock_;
	std::vector<sf::Time> schedule_;
};

#endif