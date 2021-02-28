#include <iostream>
#include "Locator.h"
#include "Player.h"
#include "Level.h"
#include "Game.h"
#include "PlayerMovementMotor.h"
#include "PlayerAnimatorController.h"
#include "Math.h"
#include "AudioManager.h"

Player::Player() :
	Character(),
	lives_(0),
	rotationSpeed_(PLAYER_ROTATION_SPEED),
	targetRotation_(0),
	targetRelativeRotation_(0),
	rotation_(0),
	hasMoved_(false)
{
	motor_ = new PlayerMovementMotor();
	animator_ = new PlayerAnimatorController(&motor(), sf::milliseconds(PLAYER_ANIMATION_INTERVAL_MS));

	#ifdef _DEBUG
		debug().info("Player ctr", DebugMessage::INITIALIZATION);
	#endif
}

void Player::reset()
{
	Character::reset();

	setTilePosition(sf::Vector2u(13, 23));
	setOffset(sf::Vector2f(-8, 0));

	motor_->speed(Locator::settings()->playerSpeed());

	setRotation(0);
	targetRotation_ = 0;
	hasMoved_ = false;

	animator().reset();
}

void Player::update()
{
	auto oldTile(motor_->tile());
	auto oldDirection(motor_->direction());
	
	motor_->update();

	setPosition(motor_->position());

	handleNewTile(oldTile, motor_->tile());

	rotatePlayer(oldDirection, motor_->direction());

	if (isFirstMove())
		resetOffset();

	rotatePerFrame();

	if (motor_->isMoving()) {
		animator().startEating();
	} else {
		animator().stopEating();
	}

	animator_->update();

	if (motor_->isMoving())
		hasMoved_ = true;
}

inline bool Player::isFirstMove() const
{
	if (hasMoved_)
		return false;

	if (motor_->isMoving())
		return !hasMoved_;

	return false;
}

void Player::handleNewTile(sf::Vector2u previousTile, sf::Vector2u currentTile)
{ 
	if (previousTile == currentTile)
		return;

	// Get tile for our current position
	Tile tile(Locator::level()->getTile(currentTile));

	// Consume dot or energizer if tile is one of those
	if (tile == Tile::TILE_DOT || tile == Tile::TILE_ENERGIZER)
	{
		Locator::level()->eatDot(currentTile);

		motor().slowMovement(sf::milliseconds(PLAYER_EAT_SLOWDOWN_MS));

		animator().changeSpeed(SPEED_FAST);
		animator().syncAudio();
	}
	else
	{
		animator().changeSpeed(SPEED_NORMAL);
		animator().syncAudio(false);
	}
}

void Player::rotatePlayer(sf::Vector2f previousDirection, sf::Vector2f currentDirection)
{ 
	// If player direction hasn't changed, no need to calculate new target rotations
	if (previousDirection == currentDirection)
		return;
	
	// If player is making a 180 turn, increase rotation speed
	if (previousDirection + currentDirection == sf::Vector2f(0, 0))
		rotationSpeed_ *= 1.5;

	// If player hasn't moved yet, use the sprite sheet's facing direction
	// (right) as previous.
	if (previousDirection == sf::Vector2f(0, 0))
		previousDirection = sf::Vector2f(1, 0);

	// Calculate the absolute and relative target rotations
	float angle = math::angle(sf::Vector2f(1, 0), currentDirection);
	int orientation = math::orientation(sf::Vector2f(1, 0), currentDirection);
	float relativeAngle = math::angle(previousDirection, currentDirection);
	int relativeOrientation = math::orientation(previousDirection, currentDirection);

	if (orientation < 0)
		angle = 360 - angle;

	if (relativeOrientation != 0)
		relativeAngle *= relativeOrientation;

	targetRotation_ = angle;
	targetRelativeRotation_ = relativeAngle;

	#ifdef _DEBUG
		std::stringstream message;
		message << "ROTATIONS: target=" << targetRotation_ << ", relative=" << targetRelativeRotation_;
		debug().info(message.str(), DebugMessage::TRANSFORMATIONS);
	#endif 
}

void Player::rotatePerFrame()
{ 
	// If we've reached our target rotation, nothing to do
	if (sf::Transformable::getRotation() == targetRotation_)
		return;

	// Rotate instantly on the first move at game start, because the default
	// orientation might be different from the direction the player is moving to.
	if (isFirstMove()) {
		sf::Transformable::setRotation(targetRotation_);
		return;
	}

	float deltaRotation = rotationSpeed_ * Locator::game()->deltaTime().asMilliseconds();
	int sign = math::sign(targetRelativeRotation_);

	// Gradually rotate towards target rotation each frame
	sf::Transformable::rotate(deltaRotation * sign);

	// Determine if near target rotation
	float rotation(sf::Transformable::getRotation());
	
	float r1(rotation - targetRotation_);
	if (r1 < 0)
		r1 += 360;

	float r2(targetRotation_ - rotation);
	if (r2 < 0)
		r2 += 360;

	float r3(std::min(r1, r2));

	// Snap rotation if near target rotation, and reset rotation speed
	if (r3 < deltaRotation * 1.5) {
		sf::Transformable::setRotation(targetRotation_);
		rotationSpeed_ = PLAYER_ROTATION_SPEED;
	}

	#ifdef _DEBUG
		std::stringstream message;
		message << "Rotation: now=" << sf::Transformable::getRotation() << ", r1=" << r1 << ", r2=" << r2;
		debug().info(message.str(), DebugMessage::TRANSFORMATIONS);
	#endif 
}

sf::Vector2f Player::forward() const
{ 
	auto direction = motor_->direction();

	// When we haven't started moving yet, forward vector
	// is the direction we're facing in the sprite sheet 
	if (direction == sf::Vector2f(0, 0))
		return sf::Vector2f(1, 0);

	return direction;
}

PlayerMovementMotor& Player::motor()
{
	return *static_cast<PlayerMovementMotor*>(motor_);
}

PlayerAnimatorController& Player::animator()
{
	return *static_cast<PlayerAnimatorController*>(animator_);
}

void Player::moveNext(MoveDirection direction)
{
	motor().setNextDirection(direction);
}

void Player::die()
{ 
	lives_--;
	Locator::gui()->updateLivesLeft(lives_);

	if (lives_ > 0) {
		Locator::game()->restartLevel(1);
	} else {
		Locator::game()->gameOver();
	}
}

Player::~Player()
{ }