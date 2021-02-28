#include <iostream>
#include "AI.h"
#include "Behaviours.h"
#include "EnemyMovementMotor.h"
#include "Global.h"
#include "GameSettings.h"

ScatterBehaviour::ScatterBehaviour(const std::vector<sf::Vector2u>& targets) :
	targets_(targets),
	current_(0)
{ }

void ScatterBehaviour::init(AI* ai)
{ 
	ai_ = ai;
	ai_->motor()->speed(Locator::settings()->enemySpeed());

	current_ = 0;

	findNextTarget();

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " is scattering. AI*: " +
			std::to_string((int) ai_) + " Enemy*: " +
			std::to_string((int) ai_->enemy()), DebugMessage::AI_BEHAVIOUR);
	#endif
};

void ScatterBehaviour::act()
{
	if (ai_->atPathEnd()) {
		ai_->clearPath();
		findNextTarget();
	}
}

void ScatterBehaviour::findNextTarget()
{
	auto target = targets_[current_];

	ai_->findPath(target);

	current_++;
	if (current_ >= targets_.size()) {
		current_ = 0;
	}
}

ScatterBehaviour::~ScatterBehaviour()
{ }