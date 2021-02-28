#include <iostream>
#include "BehaviourSchedule.h"
#include "Enemy.h"
#include "DebugTools.h"
#include "PauseClock.h"
#include "AI.h"

BehaviourSchedule::BehaviourSchedule()
{ 
	reset();
}

void BehaviourSchedule::update(AI* ai)
{
	if (schedule_.empty() || clock_.isPaused()) {
		return;
	}

	if (clock_.getElapsedTime() > schedule_.back()) 
	{
		debug().info("Behaviour schedule changed.", DebugMessage::AI_BEHAVIOUR);

		schedule_.pop_back();

		// After schedule runs out, force chase mode
		if (schedule_.empty()) {
			ai->enemy()->chase();
			return;
		}

		// Toggle mode
		if (ai->state_ == AiState::SCATTERING) {
			ai->enemy()->chase();
		} else {
			ai->enemy()->scatter();
		}

		clock_.restart();
	}
}

void BehaviourSchedule::pause(bool paused)
{
	clock_.pause(paused);
}

void BehaviourSchedule::restartCurrent()
{
	clock_.restart();
}

void BehaviourSchedule::reset()
{
	// Setup schedule times for behaviour alteration.
	// Processed in LIFO order.
	schedule_.clear();
	schedule_.push_back(sf::seconds(5));
	schedule_.push_back(sf::seconds(20));
	schedule_.push_back(sf::seconds(5));
	schedule_.push_back(sf::seconds(20));
	schedule_.push_back(sf::seconds(5));	// etc.
	schedule_.push_back(sf::seconds(20));	// Chase
	schedule_.push_back(sf::seconds(5));	// Scatter

	clock_.restart();
}

BehaviourSchedule::~BehaviourSchedule()
{ }
