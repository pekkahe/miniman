#include <iostream>
#include "DeadBehaviour.h"
#include "AI.h"
#include "Global.h"
#include "Level.h"
#include "AStar/AStar.h"
#include "Global.h"
#include "GameSettings.h"
#include "EnemyMovementMotor.h"
#include "Locator.h"

DeadBehaviour::DeadBehaviour(const sf::Vector2u& ghostPenPosition, const sf::Time& respawnTime) :
	ghostPenPosition_(ghostPenPosition),
	respawnTime_(respawnTime)
{ }

void DeadBehaviour::init(AI* ai)
{ 
	ai_ = ai;

	// Pause AI schedule so behaviour doesn't change suddenly
	ai_->schedule()->pause(true);

	// Return to ghost pen if not already there
	if (ai_->motor()->tile() != ghostPenPosition_) {
		returnToGhostPen();
	} else {
		waitUntilRespawn();
		setOffset();
	}

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " is dead. AI*: " +
			std::to_string((int) ai_) + " Enemy*: " +
			std::to_string((int) ai_->enemy()), DebugMessage::AI_BEHAVIOUR);
	#endif
}

void DeadBehaviour::reset()
{
	// Reset position in ghost pen in case the enemy is respawning
	ai_->enemy()->moveToGhostPen();
	ai_->schedule()->pause(true);

	waitUntilRespawn();
	setOffset();

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " resetting dead behaviour.", DebugMessage::AI_BEHAVIOUR);
	#endif
}

void DeadBehaviour::returnToGhostPen()
{ 
	// Move fast to ghost pen and allow to pass the door
	ai_->motor()->speed(Locator::settings()->enemyDeadSpeed());
	ai_->motor()->allowMovement(Tile::TILE_GHOST_PEN_DOOR);

	// Find a path to the ghost pen position
	astar::pathFinder().include(Locator::level()->getGhostPenDoor(), ai_);
	ai_->findPath(ghostPenPosition_, false);
	
	// Set wait action once ghost pen position is reached
	ai_->motor()->onPathEnd(std::bind(&DeadBehaviour::waitUntilRespawn, this));

	// Set offset once we've reached the ghost pen entry
	ai_->motor()->onTile(Tile::TILE_GHOST_PEN_ENTRY, std::bind(&DeadBehaviour::setOffset, this));

	// Do not act again until we've reached the ghost pen
	action_ = Action::ACTION_RETURNING;
}

void DeadBehaviour::act()
{ 
	if (action_ == Action::ACTION_WAITING) {
		if (respawnClock_.getElapsedTime() > respawnTime_) {
			beginSpawn();
		}
	}
}

void DeadBehaviour::waitUntilRespawn()
{
	ai_->clearPath();

	// Slow down movement inside ghost pen
	ai_->motor()->speed(0.05f);

	// Restrict movement inside ghost pen
	ai_->motor()->restrictMovement(MoveDirection::MOVE_LEFT);
	ai_->motor()->restrictMovement(MoveDirection::MOVE_RIGHT);
	ai_->motor()->restrictMovement(Tile::TILE_GHOST_PEN_DOOR);

	// Start respawn timer
	respawnClock_.restart();

	action_ = Action::ACTION_WAITING;
}

void DeadBehaviour::beginSpawn()
{
	// Time to respawn, so find a path outside the ghost pen
	astar::pathFinder().include(Locator::level()->getGhostPenDoor(), ai_);
	ai_->findPath(Locator::level()->getGhostPenEntry(), false);

	// Reset direction movement and temporary allow movement pass door
	// Note: if path finding takes too long, reset movement only after path is found
	ai_->motor()->allowMovement(MoveDirection::MOVE_LEFT);
	ai_->motor()->allowMovement(MoveDirection::MOVE_RIGHT);
	ai_->motor()->allowMovement(Tile::TILE_GHOST_PEN_DOOR);

	// Set spawn completed action once we've exited the ghost pen
	ai_->motor()->onPathEnd(std::bind(&DeadBehaviour::endSpawn, this));

	// Do not act again
	action_ = Action::ACTION_SPAWNING;
}

void DeadBehaviour::endSpawn()
{
	ai_->clearPath();

	// Reset offset now that we've spawned
	resetOffset();

	// Reset normal tile movement by restricting door
	ai_->motor()->restrictMovement(Tile::TILE_GHOST_PEN_DOOR);

	// Restart and unpause AI schedule 
	ai_->schedule()->restartCurrent();

	// Switch to scatter mode
	ai_->enemy()->scatter();
}

void DeadBehaviour::setOffset()
{
	resetOffset(); // Reset before setting another offset

	ai_->enemy()->setOffset(sf::Vector2f(-8, 0));

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " setting offset.", DebugMessage::AI_BEHAVIOUR);
	#endif
}

void DeadBehaviour::resetOffset()
{
	ai_->enemy()->resetOffset();

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " resetting offset.", DebugMessage::AI_BEHAVIOUR);
	#endif
}

DeadBehaviour::~DeadBehaviour()
{ }