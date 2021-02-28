#include "PlayerMovementMotor.h"
#include "Locator.h"
#include "Global.h"
#include "Game.h"
#include "GameSettings.h"

PlayerMovementMotor::PlayerMovementMotor() :
	MovementMotor(),
	rememberDirection_(sf::milliseconds(500)),
	nextDirection_(MoveDirection::NONE)
{ }

void PlayerMovementMotor::update()
{
	MovementMotor::update();

	if (slowDownTime_ > sf::Time::Zero)	{
		slowDownTime_ -= Locator::game()->deltaTime();

		if (slowDownTime_ <= sf::Time::Zero) {
			speed(Locator::settings()->playerSpeed());
		}
	}
}

void PlayerMovementMotor::slowMovement(sf::Time time)
{
	slowDownTime_ = time;
	speed(Locator::settings()->playerEatSpeed());
}

MoveDirection PlayerMovementMotor::getNextDirection()
{
	sf::Time elapsed = directionClock_.getElapsedTime();
	
	// If too much time has elapsed since direction change,
	// reset to current direction
	if (elapsed > rememberDirection_) {
		return currentDirection();
	}

	// If we're still in the time frame and can move to next
	// direction, use it
	if (canMoveTowards(nextDirection_)) {
		return nextDirection_;
	}

	// Can't move to next direction, so use current
	return currentDirection();
}

void PlayerMovementMotor::setNextDirection(MoveDirection direction)
{
	nextDirection_ = direction;
	directionClock_.restart();
}

PlayerMovementMotor::~PlayerMovementMotor()
{ }
