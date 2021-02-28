#include <iostream>
#include <SFML/Graphics.hpp>
#include "Global.h"
#include "InkyChaseBehaviour.h"
#include "AI.h"
#include "Enemy.h"
#include "EnemyMovementMotor.h"
#include "Player.h"
#include "Locator.h"
#include "GameSettings.h"

InkyChaseBehaviour::InkyChaseBehaviour(EnemyMovementMotor& blinkyMotor) :
	blinkyMotor_(blinkyMotor)
{ }

void InkyChaseBehaviour::init(AI* ai)
{ 
	ai_ = ai;
	ai_->motor()->speed(Locator::settings()->enemySpeed());

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " is chasing. AI*: " +
			std::to_string((int) ai_) + " Enemy*: " +
			std::to_string((int) ai_->enemy()), DebugMessage::AI_BEHAVIOUR);
	#endif
}

void InkyChaseBehaviour::act()
{ 
	if (!ai_->motor()->isTracking() && !ai_->isSearchingForPath()) {
		auto target = getTargetTile();

		if (ai_->motor()->canMoveTo(target)) {
			ai_->clearPath();
			ai_->findPath(target);
		} else {
			auto nearby = getNearbyTile(target);
			if (nearby != NO_SUCH_POSITION) {
				ai_->clearPath();
				ai_->findPath(nearby);
			}
		}
	}
}

sf::Vector2u InkyChaseBehaviour::getTargetTile() 
{
	auto player = Locator::player();
	const int offset = 2;

	auto playerDirection = player->direction();
	playerDirection.x *= offset;
	playerDirection.y *= offset;

	auto playerPosition = player->tile();
	auto end = sf::Vector2f(playerPosition.x + playerDirection.x, playerPosition.y + playerDirection.y);
	
	auto blinkyPosition = blinkyMotor_.tile();
	auto start = sf::Vector2f(static_cast<float>(blinkyPosition.x), static_cast<float>(blinkyPosition.y));

	auto distance = end - start;
	auto target = start + (distance * 2.f);

	return sf::Vector2u(
		static_cast<unsigned int>(target.x),
		static_cast<unsigned int>(target.y));
}

sf::Vector2u InkyChaseBehaviour::getNearbyTile(const sf::Vector2u& position) const
{
	sf::Vector2u tiles[8];
	tiles[0] = sf::Vector2u(position.x - 1, position.y - 1);
	tiles[1] = sf::Vector2u(position.x, position.y - 1);
	tiles[2] = sf::Vector2u(position.x + 1, position.y - 1);
	tiles[3] = sf::Vector2u(position.x - 1, position.y);
	tiles[4] = sf::Vector2u(position.x + 1, position.y);
	tiles[5] = sf::Vector2u(position.x - 1, position.y + 1);
	tiles[6] = sf::Vector2u(position.x, position.y + 1);
	tiles[7] = sf::Vector2u(position.x + 1, position.y + 1);

	#ifdef _DEBUG
		debug().info("Target " + util::toString(position) + " unreachable.", DebugMessage::AI_GENERAL);
		debug().info("Finding nearby tile... ", DebugMessage::AI_GENERAL);
	#endif

	for (int i = 0; i < 8; ++i) {
		if (ai_->motor()->canMoveTo(tiles[i])) {

			#ifdef _DEBUG
				debug().info(util::toString(tiles[i]), DebugMessage::AI_GENERAL);
			#endif

			return tiles[i];
		}
	}

	#ifdef _DEBUG
		debug().info("none found.", DebugMessage::AI_GENERAL);
	#endif

	return NO_SUCH_POSITION;
}

InkyChaseBehaviour::~InkyChaseBehaviour()
{ }

