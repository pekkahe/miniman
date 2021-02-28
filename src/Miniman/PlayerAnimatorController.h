#ifndef PLAYER_ANIMATOR_CONTROLLER_H_
#define PLAYER_ANIMATOR_CONTROLLER_H_

#include "AnimatorController.h"

class PlayerMovementMotor;

enum EatSpeed
{
	SPEED_NORMAL,
	SPEED_FAST
};

/// <summary>
/// Sprite sheet based player character animator.
/// </summary>
class PlayerAnimatorController : public AnimatorController
{
public:
	PlayerAnimatorController(PlayerMovementMotor* motor, const sf::Time& animationSpeed);
	~PlayerAnimatorController();

	/// <summary>
	/// Updates the player animator. Called once per frame.
	/// </summary>
	virtual void update();

	/// <summary>
	/// Animates the sprite sheet by switching to the next designated sprite.
	/// </summary>
	virtual void animate();

	/// <summary>
	/// Resets the player animator.
	/// </summary>
	virtual void reset();

	/// <summary>
	/// Starts the eating animation.
	/// </summary>
	void startEating();

	/// <summary>
	/// Stops the eating animation.
	/// </summary>
	void stopEating();

	/// <summary>
	/// Changes the speed of the eating animation.
	/// </summary>
	void changeSpeed(EatSpeed speed);

	/// <summary>
	/// Enables or disables the audio synchronization with the eating animation.
	/// If disabled, no audio is played.
	/// </summary>
	void syncAudio(bool sync = true);

private:
	bool isMouthOpen() const;
	bool isMouthClosed() const;
	bool isMouthClosing() const;

private:
	PlayerMovementMotor* motor_;
	bool isEating_;
	bool wasEating_;
	bool syncAudio_;
};

#endif
