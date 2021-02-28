#include "PinkyAI.h"
#include "PinkyChaseBehaviour.h"
#include "ScatterBehaviour.h"

PinkyAI::PinkyAI() :
	AI()
{ 
	#ifdef _DEBUG
		debug().info("PinkyAI ctr", DebugMessage::INITIALIZATION);
	#endif
}

PinkyAI::PinkyAI(const PinkyAI& other) :
	AI(other)
{ 
	#ifdef _DEBUG
		debug().info("Pinky copy ctr", DebugMessage::INITIALIZATION);
	#endif
}

AI* PinkyAI::clone() const
{
	#ifdef _DEBUG
		debug().info("Cloning Pinky", DebugMessage::INITIALIZATION);
	#endif

	return new PinkyAI(*this);
}

void PinkyAI::chase()
{
	AI::chase(new PinkyChaseBehaviour());
}

PinkyAI::~PinkyAI()
{ }
