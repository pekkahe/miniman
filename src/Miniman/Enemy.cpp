#include <iostream>
#include "Global.h"
#include "Locator.h"
#include "Enemy.h"
#include "AI.h"
#include "Game.h"
#include "EnemyMovementMotor.h"
#include "EnemyAnimatorController.h"
#include "AStar/AStar.h"

Enemy::Enemy(const std::string& name, AI* ai, sf::Time spawnTime, MoveDirection spawnMovement) :
	Character(),
	name_(name),
	ai_(ai),
	spawnTime_(spawnTime),
	spawnMovement_(spawnMovement)
{
	motor_ = new EnemyMovementMotor(*ai);
	animator_ = new EnemyAnimatorController(*this, sf::milliseconds(ENEMY_ANIMATION_INTERVAL_MS));
	ai_->init(this);

	#ifdef _DEBUG
		debug().info(name_ + " ctr", DebugMessage::INITIALIZATION);
	#endif
}

Enemy::Enemy(const Enemy& other) :
	Character(other),
	name_(other.name_),
	ai_(other.ai_->clone()),
	scatterTargets_(other.scatterTargets_),
	ghostPenPosition_(other.ghostPenPosition_),
	spawnTime_(other.spawnTime_),
	spawnMovement_(other.spawnMovement_)
{
	motor_ = static_cast<EnemyMovementMotor*>(other.motor_)->clone();
	animator_ = static_cast<EnemyAnimatorController*>(other.animator_)->clone();
	ai_->init(this);

	#ifdef _DEBUG
		debug().info(name_ + " copy ctr", DebugMessage::INITIALIZATION);
	#endif
}

void Enemy::update()
{
	// Update movement and screen position
	motor_->update();
	setPosition(motor_->position());

	// Update AI once per tick
	elapsed_ += Locator::game()->deltaTime();
	if (elapsed_ > ai_->tickTime)
	{
		ai_->tick();
		elapsed_ = sf::Time::Zero;
	}

	// Update animations
	animator_->update();
}

// NOTE: Should we remove these convenience setters altogether?
// It's not safe to do anything else here, since AI calls its
// behaviour change functions directly. Alternatively, enforce
// AI to use these functions instead.
void Enemy::scatter()
{
	ai_->scatter();
}
	
void Enemy::frightened()
{
	ai_->frightened();
}
	
void Enemy::chase()
{
	ai_->chase();
}

void Enemy::die(const sf::Time& respawnTime)
{
	ai_->die(respawnTime);
}

bool Enemy::isFrightened() const 
{
	return ai_->state() == AiState::FRIGHTENED;
}

bool Enemy::isDead() const 
{
	return ai_->state() == AiState::DEAD;
}

void Enemy::scatterTiles(const sf::Vector2u& tile1, const sf::Vector2u& tile2)
{
	scatterTargets_.clear();
	scatterTargets_.push_back(tile1);
	scatterTargets_.push_back(tile2);
}

const std::vector<sf::Vector2u>& Enemy::scatterTiles() const
{
	return scatterTargets_;
}

void Enemy::ghostPenTile(const sf::Vector2u& tile)
{
	ghostPenPosition_ = tile;
}

const sf::Vector2u Enemy::ghostPenTile() const
{
	return ghostPenPosition_;
}

void Enemy::moveToGhostPen()
{
	setTilePosition(ghostPenPosition_);
}

EnemyMovementMotor& Enemy::motor()
{
	return *static_cast<EnemyMovementMotor*>(motor_);
}

EnemyAnimatorController& Enemy::animator()
{
	return *static_cast<EnemyAnimatorController*>(animator_);
}

void Enemy::reset()
{
	Character::reset();

	moveToGhostPen();
	Character::move(spawnMovement_);

	ai_->schedule()->reset();
	ai_->die(spawnTime_);
	
	elapsed_ = sf::Time::Zero;
}

Enemy::~Enemy()
{
	if (ai_ != nullptr)
		delete ai_;
}