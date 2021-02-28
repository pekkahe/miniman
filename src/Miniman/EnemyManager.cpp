#include <iostream>
#include "Locator.h"
#include "ResourceManager.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "Player.h"
#include "BlinkyAI.h"
#include "PinkyAI.h"
#include "InkyAI.h"
#include "ClydeAI.h"
#include "Locator.h"

EnemyManager::EnemyManager() :
	blinky_(nullptr),
	inky_(nullptr),
	pinky_(nullptr),
	clyde_(nullptr)
{ }

bool EnemyManager::init()
{
	#ifdef DISABLE_ENEMIES
		return true;
	#endif

	blinky_ = new Enemy("Blinky", new BlinkyAI(), sf::Time::Zero, MoveDirection::MOVE_UP);
	blinky_->scatterTiles(sf::Vector2u(26, 1), sf::Vector2u(22, 5));
	blinky_->ghostPenTile(sf::Vector2u(13, 14));
	if (!blinky_->loadSpriteSheet(Locator::resource()->getPath("ghost_red.png"))) {
		return false;
	}

	inky_ = new Enemy("Inky", new InkyAI(), sf::seconds(7), MoveDirection::MOVE_DOWN);
	inky_->scatterTiles(sf::Vector2u(21, 29), sf::Vector2u(21, 26));
	inky_->ghostPenTile(sf::Vector2u(11, 14));
	if (!inky_->loadSpriteSheet(Locator::resource()->getPath("ghost_green.png"))) {
		return false;
	}

	pinky_ = new Enemy("Pinky", new PinkyAI(), sf::seconds(15), MoveDirection::MOVE_UP);
	pinky_->scatterTiles(sf::Vector2u(1, 1), sf::Vector2u(6, 4));
	pinky_->ghostPenTile(sf::Vector2u(13, 14));
	if (!pinky_->loadSpriteSheet(Locator::resource()->getPath("ghost_violet.png"))) {
		return false;
	}

	clyde_ = new Enemy("Clyde", new ClydeAI(), sf::seconds(30), MoveDirection::MOVE_DOWN);
	clyde_->scatterTiles(sf::Vector2u(6, 29), sf::Vector2u(6, 26));
	clyde_->ghostPenTile(sf::Vector2u(15, 14));
	if (!clyde_->loadSpriteSheet(Locator::resource()->getPath("ghost_orange.png"))) {
		return false;
	}

	// Store pointers in a collection for conveniance
	enemies_.push_back(blinky_);
	enemies_.push_back(inky_);
	enemies_.push_back(pinky_);
	enemies_.push_back(clyde_);

	reset();

	return true;
}

void EnemyManager::reset()
{
	for (auto it = enemies_.begin(); it != enemies_.end(); ++it) {
		(*it)->reset();
	}
}

void EnemyManager::update()
{
	for (auto it = enemies_.begin(); it != enemies_.end(); ++it) {
		(*it)->update();
	}
}

void EnemyManager::chase()
{
	for (auto it = enemies_.begin(); it != enemies_.end(); ++it) {
		(*it)->chase();
	}
}

void EnemyManager::scatter()
{
	for (auto it = enemies_.begin(); it != enemies_.end(); ++it) {
		(*it)->scatter();
	}
}

void EnemyManager::frightened()
{
	for (auto it = enemies_.begin(); it != enemies_.end(); ++it) {
		(*it)->frightened();
	}
}

const std::vector<Enemy*>& EnemyManager::enemies()
{
	return enemies_; 
}

Enemy* EnemyManager::enemy(const std::string& name)
{
	for (auto it = enemies_.begin(); it != enemies_.end(); ++it) {
		if ((*it)->name() == name) {
			return (*it);
		}
	}

	return nullptr;
}

EnemyManager::~EnemyManager()
{ 
	std::size_t size = enemies_.size();
	for (std::size_t i = 0; i < size; ++i) {
		delete enemies_[i];
	}
}