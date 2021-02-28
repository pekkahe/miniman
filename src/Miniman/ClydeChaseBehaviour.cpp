#include <iostream>
#include <SFML/Graphics.hpp>
#include "ClydeChaseBehaviour.h"
#include "AI.h"
#include "Enemy.h"
#include "EnemyMovementMotor.h"
#include "Level.h"
#include "Global.h"
#include "GameSettings.h"
#include "Player.h"
#include "Locator.h"

ClydeChaseBehaviour::ClydeChaseBehaviour(const std::vector<sf::Vector2u>& scatterTargets) :
	scatterTargets_(scatterTargets),
	current_(0)
{ }

void ClydeChaseBehaviour::init(AI* ai)
{ 
	ai_ = ai;
	ai_->motor()->speed(Locator::settings()->enemySpeed());

	current_ = 0;

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " is chasing. AI*: " +
			std::to_string((int) ai_) + " Enemy*: " +
			std::to_string((int) ai_->enemy()), DebugMessage::AI_BEHAVIOUR);
	#endif
};

void ClydeChaseBehaviour::act()
{ 
	if (!ai_->motor()->isTracking() && !ai_->isSearchingForPath()) {
		ai_->clearPath();
		ai_->findPath(getTargetTile());
	}
}

sf::Vector2u ClydeChaseBehaviour::getTargetTile() 
{
	auto player = Locator::player()->tile();
	auto position = ai_->motor()->tile();
	auto distance = (int) util::distance(position, player);

	if (distance < 8) {
		#ifdef _DEBUG
			std::stringstream message;
			message << "Clyde distance: " << distance << " -> scatter";
			debug().info(message.str(), DebugMessage::AI_GENERAL);
		#endif
		return nextScatterTarget();
	} else {
		#ifdef _DEBUG
			std::stringstream message;
			message << "Clyde distance: " << distance << " -> chase player";
			debug().info(message.str(), DebugMessage::AI_GENERAL);
		#endif
		return Locator::player()->tile();
	}
}

sf::Vector2u ClydeChaseBehaviour::nextScatterTarget()
{
	auto target = scatterTargets_[current_];

	current_++;
	if (current_ >= scatterTargets_.size()) {
		current_ = 0;
	}

	return target;
}

ClydeChaseBehaviour::~ClydeChaseBehaviour()
{ }
