#include "BlinkyAI.h"
#include "BlinkyChaseBehaviour.h"
#include "ScatterBehaviour.h"

BlinkyAI::BlinkyAI() :
	AI()
{ 
	#ifdef _DEBUG
		debug().info("BlinkyAI ctr", DebugMessage::INITIALIZATION);
	#endif
}

BlinkyAI::BlinkyAI(const BlinkyAI& other) :
	AI(other)
{ 
	#ifdef _DEBUG
		debug().info("Blinky copy ctr", DebugMessage::INITIALIZATION);
	#endif
}

AI* BlinkyAI::clone() const
{
	#ifdef _DEBUG
		debug().info("Cloning Blinky", DebugMessage::INITIALIZATION);
	#endif

	return new BlinkyAI(*this);
}

void BlinkyAI::chase()
{
	AI::chase(new BlinkyChaseBehaviour());
}

BlinkyAI::~BlinkyAI()
{ }
