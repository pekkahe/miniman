#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "BlinkyChaseBehaviour.h"
#include "EnemyMovementMotor.h"
#include "AI.h"
#include "GameSettings.h"
#include "Player.h"
#include "Locator.h"

BlinkyChaseBehaviour::BlinkyChaseBehaviour()
{ }

void BlinkyChaseBehaviour::init(AI* ai)
{ 
	ai_ = ai;
	ai_->motor()->speed(Locator::settings()->enemySpeed());

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " is chasing. AI*: " +
			std::to_string((int) ai_) + " Enemy*: " +
			std::to_string((int) ai_->enemy()), DebugMessage::AI_BEHAVIOUR);
	#endif
};

void BlinkyChaseBehaviour::act()
{
	if (!ai_->motor()->isTracking() && !ai_->isSearchingForPath()) {
		ai_->clearPath();
		ai_->findPath(getTargetTile());
	}
}

sf::Vector2u BlinkyChaseBehaviour::getTargetTile() 
{
	return Locator::player()->tile();
}

BlinkyChaseBehaviour::~BlinkyChaseBehaviour()
{ }