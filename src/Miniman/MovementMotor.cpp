#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>
#include "Locator.h"
#include "MovementMotor.h"
#include "Level.h"
#include "Global.h"
#include "Game.h"

MovementMotor::MovementMotor() :
	moveDirection_(MoveDirection::NONE),
	tile_(0, 0),
	previousTile_(0, 0),
	position_(0, 0),
	speed_(0),
	target_(0, 0),
	isMoving_(false)
{ 
	movableTiles_.push_back(Tile::TILE_EMPTY);
	movableTiles_.push_back(Tile::TILE_DOT);
	movableTiles_.push_back(Tile::TILE_ENERGIZER);
	movableTiles_.push_back(Tile::TILE_TELEPORT);
	movableTiles_.push_back(Tile::TILE_GHOST_PEN_ENTRY);
	movableTiles_.push_back(Tile::TILE_DEBUG_TRACK);
	movableTiles_.push_back(Tile::TILE_DEBUG_YELLOW);
	movableTiles_.push_back(Tile::TILE_DEBUG_RED);
}

MovementMotor::MovementMotor(const MovementMotor& other) :
	moveDirection_(other.moveDirection_),
	tile_(other.tile_), 
	previousTile_(other.previousTile_), 
	position_(other.position_),
	speed_(other.speed_),
	target_(other.target_),
	movableTiles_(movableTiles_)
{ }

void MovementMotor::update()
{
	bool targetReached = moveTowardsTarget();

	if (targetReached)
	{
		auto nextDirection = getNextDirection();

		// Prevent changing direction on teleport tiles, so we don't get stuck
		if (Locator::level()->isTeleport(tile_)) {
			nextDirection = moveDirection_;
		}

		auto nextTile = getNextTile(nextDirection);
		
		if (canMoveTo(nextTile)) {
			setTarget(nextTile);
		} 

		moveDirection_ = nextDirection;
	}
}

void MovementMotor::reset()
{
	isMoving_ = false;
	moveDirection_ = MoveDirection::NONE;
}

bool MovementMotor::canMoveTowards(MoveDirection direction) const
{
	auto tile = getNextTile(direction);
	return canMoveTo(tile);
}

bool MovementMotor::canMoveTo(const sf::Vector2u& position) const
{
	if (!Locator::level()->isValid(position)) {
		return false;
	}

	auto tile = Locator::level()->getTile(position);
	
	return std::count(movableTiles_.begin(), movableTiles_.end(), tile) > 0;
}

bool MovementMotor::moveTowardsTarget()
{
	auto oldPosition(position_);
	auto oldTile(tile_);

	float deltaMovement = speed_ * Locator::game()->deltaTime().asMilliseconds();
	float distance = 0.f;

	position_ += deltaMovement * direction();

	switch (moveDirection_)
	{
		case MoveDirection::MOVE_UP:
			distance = position_.y - target_.y;
			break;

		case MoveDirection::MOVE_DOWN:
			distance = target_.y - position_.y;
			break;

		case MoveDirection::MOVE_RIGHT:
			distance = target_.x - position_.x;
			break;

		case MoveDirection::MOVE_LEFT:
			distance = position_.x - target_.x;
			break;

		// If not yet started moving
		case MoveDirection::NONE:
			return true; 
	}

	// Snap to target, if we've moved past it
	if (distance < 0) 
	{
		if (moveDirection_ == MOVE_UP || moveDirection_ == MOVE_DOWN) {
			position_.y = target_.y;
		} else {
			position_.x = target_.x;
		}
	}
	
	// Did we move?
	isMoving_ = position_ != oldPosition;

	// Map tile if position changed
	if (isMoving_) 
	{
		auto level(Locator::level());

		tile_ = level->mapToTilePosition(position_);

		// Update previous tile if tile changed
		if (tile_ != oldTile) {
			previousTile_ = oldTile;

			// If we moved to a teleport tile, teleport!
			if (level->isTeleport(tile_)) {
				setPosition(level->getTeleportTarget(tile_));
				return true;
			}
		}
	}

	return distance < 0;
}

void MovementMotor::setPosition(const sf::Vector2u& tile)
{
	position_ = Locator::level()->getTileCenter(tile);
	previousTile_ = tile_;
	tile_ = tile;
}

void MovementMotor::move(MoveDirection moveDirection)
{
	moveDirection_ = moveDirection;
	target_ = position_ + direction(); // Target cannot be zero vector
}

void MovementMotor::setTarget(const sf::Vector2u& tile)
{
	target_ = Locator::level()->getTileCenter(tile);
}

sf::Vector2f MovementMotor::velocity() const
{
	auto velocity = direction();
	velocity.x *= speed_;
	velocity.y *= speed_;
	return velocity;
}

sf::Vector2f MovementMotor::direction() const
{
	switch (moveDirection_)
	{
		case MoveDirection::MOVE_UP:
			return sf::Vector2f(0, -1);
		case MoveDirection::MOVE_DOWN:
			return sf::Vector2f(0, 1);
		case MoveDirection::MOVE_RIGHT:
			return sf::Vector2f(1, 0);
		case MoveDirection::MOVE_LEFT:
			return sf::Vector2f(-1, 0);
		default:
			return sf::Vector2f(0, 0);
	}
}

sf::Vector2u MovementMotor::getNextTile(MoveDirection direction) const
{
	auto nextTile(tile_);
	auto levelSize = Locator::level()->size();

	switch (direction)
	{
		case MoveDirection::MOVE_UP:
			if (nextTile.y > 0)
				nextTile.y--;
			break;

		case MoveDirection::MOVE_DOWN:
			if (nextTile.y < levelSize.y - 1)
				nextTile.y++;
			break;

		case MoveDirection::MOVE_RIGHT:
			if (nextTile.x < levelSize.x - 1)
				nextTile.x++;
			break;

		case MoveDirection::MOVE_LEFT:
			if (nextTile.x > 0)
				nextTile.x--;
			break;
		
		// Shouldn't reach here
		case MoveDirection::NONE:
			break;
	}

	return nextTile;
}

MovementMotor::~MovementMotor()
{ }
