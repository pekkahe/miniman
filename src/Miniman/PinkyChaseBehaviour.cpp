#include <SFML/Graphics.hpp>
#include "PinkyChaseBehaviour.h"
#include "Player.h"
#include "AI.h"
#include "Enemy.h"
#include "EnemyMovementMotor.h"
#include "Locator.h"
#include "GameSettings.h"

PinkyChaseBehaviour::PinkyChaseBehaviour()
{ }

void PinkyChaseBehaviour::init(AI* ai)
{ 
	ai_ = ai;
	ai_->motor()->speed(Locator::settings()->enemySpeed());

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " is chasing. AI*: " +
			std::to_string((int) ai_) + " Enemy*: " +
			std::to_string((int) ai_->enemy()), DebugMessage::AI_BEHAVIOUR);
	#endif
};

void PinkyChaseBehaviour::act()
{ 
	if (!ai_->motor()->isTracking() && !ai_->isSearchingForPath()) {
		auto target = getTargetTile();

		if (ai_->motor()->canMoveTo(target)) {
			ai_->clearPath();
			ai_->findPath(target);
		}
	}
}

sf::Vector2u PinkyChaseBehaviour::getTargetTile() 
{
	auto player = Locator::player();
	auto target = player->tile();
	auto position = ai_->motor()->tile();
	bool isTooClose = false;

	const int offset = 4;
	switch (player->moveDirection())
	{
		case MoveDirection::MOVE_UP:
			target.y -= offset;
			isTooClose = position.x == target.x && position.y >= target.y;
			break;

		case MoveDirection::MOVE_DOWN:
			target.y += offset;
			isTooClose = position.x == target.x && position.y <= target.y;
			break;

		case MoveDirection::MOVE_RIGHT:
			target.x += offset;
			isTooClose = position.y == target.y && position.x <= target.x;
			break;

		case MoveDirection::MOVE_LEFT:
			target.x -= offset;
			isTooClose = position.y == target.y && position.x >= target.x;
			break;

		default:
			break;
	}

	if (isTooClose) {
		return player->tile();
	} else {
		return target;
	}
}

PinkyChaseBehaviour::~PinkyChaseBehaviour()
{ }