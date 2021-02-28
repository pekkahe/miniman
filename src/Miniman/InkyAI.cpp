#include <cassert>
#include "InkyAI.h"
#include "ScatterBehaviour.h"
#include "InkyChaseBehaviour.h"
#include "Game.h"
#include "Locator.h"

InkyAI::InkyAI() : 
	AI()
{ 
	#ifdef _DEBUG
		debug().info("InkyAI ctr", DebugMessage::INITIALIZATION);
	#endif
}

InkyAI::InkyAI(const InkyAI& other) :
	AI(other)
{ 
	#ifdef _DEBUG
		debug().info("Inky copy ctr", DebugMessage::INITIALIZATION);
	#endif
}

AI* InkyAI::clone() const
{
	#ifdef _DEBUG
		debug().info("Cloning Inky", DebugMessage::INITIALIZATION);
	#endif

	return new InkyAI(*this);
}

void InkyAI::chase()
{
	auto blinky = Locator::enemyManager()->enemy("Blinky");
	
	assert(blinky != nullptr);

	AI::chase(new InkyChaseBehaviour(blinky->motor()));
}

InkyAI::~InkyAI()
{ }
