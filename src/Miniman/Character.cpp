#include <cmath>
#include <iostream>
#include "Character.h"
#include "IMovementMotor.h"
#include "AnimatorController.h"

Character::Character() :
	motor_(nullptr),
	animator_(nullptr)
{ }
	
Character::Character(const Character& other) :
	motor_(other.motor_),
	animator_(other.animator_)
{ }

bool Character::loadSpriteSheet(const std::string& fileName)
{
	if (!animator_->loadSpriteSheet(fileName)) {
		debug().error("Failed to load sprite sheet " + fileName);
		return false;
	}

	resetOrigin();
	return true;
}

void Character::setOffset(const sf::Vector2f& offset)
{
	auto origin = getOrigin();
	origin += offset;

	setOrigin(origin);
}

void Character::resetOffset()
{ 
	resetOrigin();
}

void Character::resetOrigin()
{
	auto size = animator_->spriteSize();

	setOrigin(sf::Vector2f(static_cast<float>(size.x / 2), static_cast<float>(size.y / 2)));
}

void Character::reset()
{
	resetOffset();

	motor_->reset();
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(animator_->sprite(), states);
}

sf::Vector2f Character::position() const 
{
	return motor_->position(); 
}

sf::Vector2u Character::tile() const 
{
	return motor_->tile(); 
}

sf::Vector2f Character::direction() const
{
	return motor_->direction();
}

MoveDirection Character::moveDirection() const
{
	return motor_->moveDirection();
}

void Character::setTilePosition(const sf::Vector2u& tile)
{
	motor_->setPosition(tile);
}

void Character::move(MoveDirection direction)
{
	motor_->move(direction);
}

Character::~Character()
{
	if (motor_ != nullptr) {
		delete motor_;
	}
	if (animator_ != nullptr) {
		delete animator_;
	}
}