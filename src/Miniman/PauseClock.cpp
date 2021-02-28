#include "PauseClock.h"

PauseClock::PauseClock() :
	isPaused_(false),
	pauseStartTime_(sf::Time::Zero),
	pauseTime_(sf::Time::Zero)
{ }

sf::Time PauseClock::getElapsedTime() const
{
	if (isPaused_) {
		return pauseStartTime_;
	}

	return clock_.getElapsedTime() - pauseTime_;
}

sf::Time PauseClock::restart()
{
	isPaused_ = false;
	pauseTime_ = sf::Time::Zero;

	return clock_.restart();
}

void PauseClock::pause(bool paused) 
{
	// If we're already paused, don't overwrite start time
	if ((isPaused_ && paused) || (!isPaused_ && !paused)) {
		return;
	}

	if (paused) {
		pauseStartTime_ = clock_.getElapsedTime();
	} else {
		pauseTime_ = clock_.getElapsedTime() - pauseStartTime_;
	}

	isPaused_ = paused;
}

PauseClock::~PauseClock()
{ }
