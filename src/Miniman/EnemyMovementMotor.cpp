#include <iostream>
#include <algorithm>
#include <assert.h>
#include <exception>
#include <thread>
#include "Global.h"
#include "EnemyMovementMotor.h"
#include "AStar/AStar.h"
#include "HighlighterTest.h"
#include "AI.h"
#include "Level.h"
#include "Locator.h"

EnemyMovementMotor::EnemyMovementMotor(AI& ai) :
	MovementMotor(),
	ai_(ai),
	path_(nullptr),
	isTracking_(false),
	wasTracking_(false),
	onPathReached_(nullptr),
	onTile_(nullptr)
{ 
	movableTiles_.push_back(Tile::TILE_GHOST_PEN);

	moveDirectionPriority_[0] = MoveDirection::MOVE_UP;
	moveDirectionPriority_[1] = MoveDirection::MOVE_LEFT;
	moveDirectionPriority_[2] = MoveDirection::MOVE_DOWN;
	moveDirectionPriority_[3] = MoveDirection::MOVE_RIGHT;
}

EnemyMovementMotor::EnemyMovementMotor(const EnemyMovementMotor& other)	:
	MovementMotor(other),
	path_(other.path_),
	ai_(other.ai_),
	tracks_(other.tracks_),
	isTracking_(other.isTracking_),
	wasTracking_(other.wasTracking_),
	moveRestrictions_(other.moveRestrictions_),
	moveDirectionPriority_(other.moveDirectionPriority_),
	onPathReached_(other.onPathReached_),
	onTile_(other.onTile_)
{ }

EnemyMovementMotor* EnemyMovementMotor::clone() const
{
	return new EnemyMovementMotor(*this);
}

void EnemyMovementMotor::update()
{
	MovementMotor::update();

	// Callback if we've reached a tile of the specified type
	if (onTile_ != nullptr && onTileTarged_ == Locator::level()->getTile(tile_)) {
		onTile_();
		onTile_ = nullptr;
	}
}

MoveDirection EnemyMovementMotor::getNextDirection()
{
	if (isTracking_)
		storeTracks();
	
	if (wasTracking_ && !isTracking_) {
		truncatePath(path_);

		if (path_ != nullptr && path_->endReached()) {
			path_ = nullptr;
		}

		wasTracking_ = false;
	} 
	
	if (tracks_.count() > 0 && !isTracking_)
		return followTracks();

	// If we have no path, choose an arbitrary direction
	if (path_ == nullptr)
		return chooseNextDirection();

	// If we've reached target tile, increment path
	bool endReached = false;
	if (tile_ == path_->current())
		endReached = !path_->increment();

	// If we've reached path end, continue moving towards current direction
	if (endReached)
	{
		path_ = nullptr;

		// Callback if necessary
		if (onPathReached_ != nullptr) {
			onPathReached_();
			onPathReached_ = nullptr;
		}

		return currentDirection(); 
	}

	// Otherwise, follow path
	auto direction = getDirectionTowards(path_->current());

	// Failsafe; if for some reason we've dropped out of path
	// and have reached a wall, reset path and behaviour
	if (!canMoveTowards(direction)) {
		#ifdef _DEBUG
			debug().error("Enemy has dropped out of path and cannot move towards next direction. Resetting.");
			DEBUG_BREAK;
		#endif
			
		path_ = nullptr;
		onPathReached_ = nullptr;

		ai_.resetBehaviour();

		return chooseNextDirection();
	}

	return direction;
}

bool EnemyMovementMotor::isFollowingPath() const
{
	return path_ != nullptr && tracks_.count() == 0 && !isTracking_ && !wasTracking_;
}

MoveDirection EnemyMovementMotor::getDirectionTowards(const sf::Vector2u& target)
{
	auto direction = getDirectionTowards(tile_, target);
	
	// If source and target is at the same position, continue with current direction
	if (direction == MoveDirection::NONE) {
		return currentDirection(); 
	}
	
	return direction; 
}

MoveDirection EnemyMovementMotor::getDirectionTowards(const sf::Vector2u& source, const sf::Vector2u& target)
{
	if (source.y == target.y)
	{
		if (source.x < target.x) {
			return MoveDirection::MOVE_RIGHT;
		} else if (source.x > target.x) {
			return MoveDirection::MOVE_LEFT;
		}
	} 
	else if (source.x == target.x)
	{
		if (source.y < target.y) {
			return MoveDirection::MOVE_DOWN;
		} else if (source.y > target.y) {
			return MoveDirection::MOVE_UP;
		}
	} 

	// Source and target are the same
	return MoveDirection::NONE;
}

void EnemyMovementMotor::startTracking()
{
	tracks_.clear();

	// Store current position as first track
	storeTracks();

	isTracking_ = true;
	wasTracking_ = true;

	#ifdef _DEBUG
		tests::highlighter().clear(HighlightColor::GREY);
	#endif
}

void EnemyMovementMotor::setPath(astar::Path* path)
{
	path_ = path;

	#ifdef _DEBUG
		tests::highlighter().clear(HighlightColor::RED);
		tests::highlighter().clear(HighlightColor::YELLOW);

		if (path != nullptr) {
			tests::highlighter().highlight(path_->first(), HighlightColor::RED);
			tests::highlighter().highlight(path_->last(), HighlightColor::YELLOW);
		} 
	#endif
	
	isTracking_ = false;
}

void EnemyMovementMotor::truncatePath(astar::Path* path)
{
	if (path == nullptr || tracks_.count() == 0) {
		return;
	}

	// Get the shared position count between the path and tracks
	int sharedCount = compareTracks(path);

	#ifdef _DEBUG
		debug().info("Truncating tracks: " + tracks_.count(), DebugMessage::AI_TRACKING);
		debug().info("Same with path: " + sharedCount, DebugMessage::AI_TRACKING);
	#endif
	
	// Get the direction where the path is leading from the starting intersection
	MoveDirection pathDirection = MoveDirection::NONE;
	if (path->count() > 1) {
		pathDirection = getDirectionTowards(path->at(0), path->at(1));
	}

	// Get the direction where the tracks are leading, or current direction if there's not enough tracks
	MoveDirection trackDirection = MoveDirection::NONE;
	if (tracks_.count() > 1) {
		trackDirection = getDirectionTowards(tracks_.last(), tracks_.at(tracks_.count() - 2));
	} else {
		trackDirection = currentDirection();
	}

	// If the path and tracks leads to the same direction, remove all shared positions from the path
	// (and tracks) to prevent abrupt 180 degree turning. If they lead to separate directions, we
	// should leave the first shared position in the path, so we know where to turn.
	if (pathDirection == trackDirection) {
		path->removeFromBeginning(sharedCount);
	} else {
		path->removeFromBeginning(sharedCount - 1);
	}

	// Don't forget to truncate the tracks also by removing the shared positions from it
	tracks_.removeFromEnd(sharedCount);
}

int EnemyMovementMotor::compareTracks(astar::Path* path)
{
	int samePositionCount = 0;

	for (int i = 0; i < tracks_.count(); i++)
	{
		if (i >= path->count()) {
			break;
		}

		int j = tracks_.count() - 1 - i;

		// Break on first different position
		if (tracks_.at(j) != path->at(i)) {
			break;
		}

		samePositionCount++;
	}

	return samePositionCount;
}

MoveDirection EnemyMovementMotor::followTracks()
{
	// If we've reached the end of the tracks, continue on current direction
	if (tile_ == tracks_.current() && !tracks_.increment()) 
	{
		tracks_.clear();

		#ifdef _DEBUG
			tests::highlighter().clear(HighlightColor::GREY);
		#endif

		return currentDirection(); 
	}

	auto target = tracks_.current();

	#ifdef _DEBUG
		debug().info("Following track: " + util::toString(target), DebugMessage::AI_TRACKING);
	#endif

	return getDirectionTowards(target);
}

void EnemyMovementMotor::storeTracks()
{
	// Prevent storing same track twice
	if (tracks_.count() > 0)
	{
		int index = tracks_.count() - 1;
		if (tracks_.at(index) == tile_) {
			return;
		}
	}

	tracks_.addToBeginning(tile_);

	#ifdef _DEBUG
		debug().info("Track added: " + util::toString(tile_), DebugMessage::AI_TRACKING);
		tests::highlighter().highlight(tile_, HighlightColor::GREY);
	#endif
}

MoveDirection EnemyMovementMotor::chooseNextDirection()
{
	// Continue with current direction until we hit a wall
	if (canMoveTowards(currentDirection())) {
		return currentDirection();
	} 

	// Choose first available direction with highest priority
	for (std::size_t i = 0; i < moveDirectionPriority_.size(); ++i)
	{
		if (canChooseDirection(moveDirectionPriority_[i])) {
			return moveDirectionPriority_[i];
		}
	}
	
	#ifdef _DEBUG
		debug().warning("Enemy couldn't choose next direction. Using NONE.");
	#endif

	return MoveDirection::NONE;
}

bool EnemyMovementMotor::canChooseDirection(MoveDirection direction)
{
	if (!canMoveTowards(direction)) {
		return false;
	}

	// If we have any restrictions at all, the direction is valid if it's not on the list
	if (moveRestrictions_.size() > 0) { 
		return std::count(moveRestrictions_.begin(), moveRestrictions_.end(), direction) == 0;
	}

	// Otherwise, the direction is valid if it doesn't cause a 180 degree turn
	return currentDirection() != getOppositeDirection(direction);
}

void EnemyMovementMotor::restrictMovement(MoveDirection direction)
{
	// Restrict direction movement by adding the direction to our move restrictions
	if (std::count(moveRestrictions_.begin(), moveRestrictions_.end(), direction) == 0) {
		moveRestrictions_.push_back(direction);
	}
}

void EnemyMovementMotor::restrictMovement(Tile tile)
{
	// Restrict tile movement by removing the tile from our movable tiles
	auto first = std::remove(movableTiles_.begin(), movableTiles_.end(), tile);
	movableTiles_.erase(first, movableTiles_.end());
}

void EnemyMovementMotor::allowMovement(MoveDirection direction)
{
	// Allow direction movement by removing the direction from our move restrictions
	auto first = std::remove(moveRestrictions_.begin(), moveRestrictions_.end(), direction);
	moveRestrictions_.erase(first, moveRestrictions_.end());
}

void EnemyMovementMotor::allowMovement(Tile tile)
{
	// Allow tile movement by adding the tile to our movable tiles
	if (std::count(movableTiles_.begin(), movableTiles_.end(), tile) == 0) {
		movableTiles_.push_back(tile);
	}
}

void EnemyMovementMotor::onPathEnd(std::function<void()> function)
{ 
	onPathReached_ = function;
}

void EnemyMovementMotor::onTile(Tile tile, std::function<void()> function)
{
	onTileTarged_ = tile;
	onTile_ = function;
}

MoveDirection EnemyMovementMotor::getOppositeDirection(MoveDirection direction)
{
	switch (direction)
	{
		case MoveDirection::MOVE_UP:
			return MoveDirection::MOVE_DOWN;
		case MoveDirection::MOVE_LEFT:
			return MoveDirection::MOVE_RIGHT;
		case MoveDirection::MOVE_DOWN:
			return MoveDirection::MOVE_UP;
		case MoveDirection::MOVE_RIGHT:
			return MoveDirection::MOVE_LEFT;
		default:
			return MoveDirection::NONE;
	}
}

EnemyMovementMotor::~EnemyMovementMotor()
{ }