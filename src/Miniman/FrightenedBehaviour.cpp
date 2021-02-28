#include "FrightenedBehaviour.h"
#include "AI.h"
#include "Level.h"
#include "AStar/AStar.h"
#include "Global.h"
#include "EnemyMovementMotor.h"
#include "Locator.h"
#include "GameSettings.h"

FrightenedBehaviour::FrightenedBehaviour(sf::Time behaviourTime) :
	behaviourTime_(behaviourTime)
{ }

void FrightenedBehaviour::init(AI* ai)
{ 
	ai_ = ai;
	ai_->motor()->speed(Locator::settings()->enemyFrightenedSpeed());

	// Pause AI schedule so behaviour doesn't change suddenly
	ai_->schedule()->pause(true);

	clock_.restart();

	#ifdef _DEBUG
		debug().info(ai_->enemy()->name() + " is frightened. AI*: " +
			std::to_string((int) ai_) + " Enemy*: " +
			std::to_string((int) ai_->enemy()), DebugMessage::AI_BEHAVIOUR);
	#endif
}

void FrightenedBehaviour::reset()
{
	ai_->schedule()->pause(true);

	clock_.restart();
}

void FrightenedBehaviour::act()
{ 
	if (clock_.getElapsedTime() > behaviourTime_) {
		ai_->schedule()->restartCurrent();
		ai_->enemy()->scatter();
	}
}

FrightenedBehaviour::~FrightenedBehaviour()
{ }
