#include "PlayerAnimatorController.h"
#include <iostream>
#include "Player.h"
#include "PlayerMovementMotor.h"
#include "Global.h"
#include "Locator.h"
#include "AudioManager.h"

PlayerAnimatorController::PlayerAnimatorController(PlayerMovementMotor* motor, const sf::Time& animationSpeed) :
	AnimatorController(motor, animationSpeed),
	motor_(motor),
	isEating_(false),
	wasEating_(false),
	syncAudio_(false)
{ }


void PlayerAnimatorController::reset()
{
	AnimatorController::reset();
	AnimatorController::changeSpeed(sf::milliseconds(PLAYER_ANIMATION_INTERVAL_MS));
	
	isEating_ = false;
	wasEating_ = false;
	syncAudio_ = false;
}

void PlayerAnimatorController::update()
{ 
	AnimatorController::update();
}

void PlayerAnimatorController::animate()
{
	if (isEating_ && motor_->isMoving())
		AnimatorController::pingPongSprite(0);

	if (!isEating_ && wasEating_) {
		// Animate to open mouth and stop
		AnimatorController::setNextSprite();

		if (isMouthOpen())
			wasEating_ = false;
	}

	if (!syncAudio_)
		return;

	if (isEating_ && isMouthClosing() || wasEating_ && isMouthClosing()) {
		Locator::audio()->playEatSound(0.4f);
	}
}

void PlayerAnimatorController::syncAudio(bool sync)
{
	syncAudio_ = sync;
}

void PlayerAnimatorController::changeSpeed(EatSpeed speed)
{
	if (speed == SPEED_NORMAL) { 
		AnimatorController::changeSpeed(sf::milliseconds(PLAYER_ANIMATION_INTERVAL_MS));
	} else if (speed == SPEED_FAST) {
		AnimatorController::changeSpeed(sf::milliseconds(PLAYER_FAST_ANIMATION_INTERVAL_MS));
	}
}

bool PlayerAnimatorController::isMouthOpen() const
{
	return spriteIndex() == 3;
}

bool PlayerAnimatorController::isMouthClosing() const
{ 
	return spriteIndex() == 1 && previousSpriteIndex() > spriteIndex();
}

bool PlayerAnimatorController::isMouthClosed() const
{
	return spriteIndex() == 0;
}

void PlayerAnimatorController::startEating()
{
	if (!isEating_)
	{
		isEating_ = true;
	}
}

void PlayerAnimatorController::stopEating()
{ 
	if (isEating_)
	{
		isEating_ = false;
		wasEating_ = true;
	}
}

PlayerAnimatorController::~PlayerAnimatorController()
{ }
