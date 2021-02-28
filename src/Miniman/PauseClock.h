#ifndef PAUSECLOCK_H_
#define PAUSECLOCK_H_

#include <SFML/Graphics.hpp>

/// <summary>
/// Pausable clock built on top of sf::Clock.
/// </summary>
class PauseClock
{
public:
	PauseClock();
	~PauseClock();

	/// <summary>
	/// Pauses or unpauses the clock.
	/// </summary>
	void pause(bool paused);

	/// <summary>
	/// Returns true if the clock is currently paused.
	/// </summary>
	bool isPaused() const { return isPaused_; }

	/// <summary>
	/// Returns the elapsed time since the last call to <c>restart</c>.
	/// </summary>
	sf::Time getElapsedTime() const;

	/// <summary>
	/// Restarts the clock and returns the elapsed time.
	/// </summary>
	sf::Time restart();

private:
	bool isPaused_;
	sf::Time pauseStartTime_;
	sf::Time pauseTime_;
	sf::Clock clock_;
};

#endif

