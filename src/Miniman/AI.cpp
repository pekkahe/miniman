#include <iostream>
#include <cstdio>
#include "Locator.h"
#include "AI.h"
#include "Global.h"
#include "Level.h"
#include "Enemy.h"
#include "Game.h"
#include "EnemyMovementMotor.h"
#include "Behaviours.h"
#include "AStar/AStar.h"

AI::AI() :
	tickTime(sf::seconds(1.f)),
	path_(nullptr),
	behaviour_(nullptr),
	enemy_(nullptr),
	isSearchingForPath_(false)
{ }

AI::AI(const AI& other):
	tickTime(other.tickTime),
	path_(other.path_),
	behaviour_(other.behaviour_),
	enemy_(other.enemy_),
	isSearchingForPath_(other.isSearchingForPath_)
{ }

void AI::init(Enemy* enemy)
{
	enemy_ = enemy;

	#ifdef _DEBUG
		debug().info(enemy_->name() + " AI init", DebugMessage::INITIALIZATION);
	#endif
}

void AI::tick()
{
	if (behaviour_ != nullptr) {
		behaviour_->act();
	}

	behaviourSchedule_.update(this);
}

Enemy* AI::enemy()
{
	return enemy_; 
}

EnemyMovementMotor* AI::motor()
{
	return &enemy_->motor(); 
}

void AI::findPath(sf::Vector2u position, bool preventBackTracking)
{
	if (path_ != nullptr || isSearchingForPath_) {
		return;
	}

	isSearchingForPath_ = true;

	motor()->startTracking();

	// Ignore previous position on path finding, 
	// to avoid turning towards opposite direction
	if (preventBackTracking) {
		astar::pathFinder().exclude(motor()->previousTile(), this);
	}

	astar::pathFinder().findPath(motor()->tile(), position, this);
}

void AI::pathFound(astar::Path* path)
{
	mutex_.lock();

	path_ = path;
	motor()->setPath(path);

	isSearchingForPath_ = false;

	#ifdef _DEBUG
		if (path == nullptr) {
			debug().info(enemy_->name() + "found no path.", DebugMessage::AI_GENERAL);
		}
	#endif

	mutex_.unlock();
}

bool AI::atPathEnd() const
{
	return path_ != nullptr && path_->endReached();
}

bool AI::hasPath() const
{
	return path_ != nullptr; 
}

void AI::clearPath()
{
	astar::pathFinder().unregister(this);

	isSearchingForPath_ = false;

	motor()->setPath(nullptr);

	if (path_ != nullptr) {
		delete path_;
		path_ = nullptr;
	}
}

void AI::setBehaviour(BehaviourInterface* behaviour)
{
	clearPath();

	if (behaviour_ != nullptr) {
		delete behaviour_;
		behaviour_ = nullptr;
	}

	behaviour_ = behaviour;
	behaviour_->init(this);
}

void AI::resetBehaviour()
{
	if (behaviour_ != nullptr) {
		clearPath();
		behaviour_->init(this);
	}
}

void AI::frightened()
{
	// Ghosts in pen shouldn't be affected by energizers
	if (state_ == AiState::DEAD) {
		return;
	}

	if (state_ != AiState::FRIGHTENED) {
		state_ = AiState::FRIGHTENED;
		setBehaviour(new FrightenedBehaviour(Locator::settings()->frightTime()));
	} else {
		behaviour_->reset();
	}
}

void AI::scatter()
{
	if (state_ != AiState::SCATTERING) {
		state_ = AiState::SCATTERING;
		setBehaviour(new ScatterBehaviour(enemy_->scatterTiles()));
	} else {
		behaviour_->reset();
	}
}

void AI::chase(BehaviourInterface* behaviour)
{
	if (state_ != AiState::CHASING) {
		state_ = AiState::CHASING;
		setBehaviour(behaviour);
	} else {
		behaviour_->reset();
	}
}

void AI::die(const sf::Time& respawnTime)
{
	if (state_ != AiState::DEAD) {
		state_ = AiState::DEAD;
		setBehaviour(new DeadBehaviour(enemy_->ghostPenTile(), respawnTime));
	} else {
		behaviour_->reset();
	}
}

AI::~AI() 
{
	if (behaviour_ != nullptr) {
		delete behaviour_;
	}
	if (path_ != nullptr) {
		delete path_;
	}
}