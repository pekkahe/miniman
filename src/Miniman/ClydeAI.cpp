#include "ClydeAI.h"
#include "ClydeChaseBehaviour.h"
#include "ScatterBehaviour.h"

ClydeAI::ClydeAI() :
	AI()
{ 
	#ifdef _DEBUG
		debug().info("ClydeAI ctr", DebugMessage::INITIALIZATION);
	#endif
}

ClydeAI::ClydeAI(const ClydeAI& other) :
	AI(other)
{ 
	#ifdef _DEBUG
		debug().info("Clyde copy ctr", DebugMessage::INITIALIZATION);
	#endif
}

AI* ClydeAI::clone() const
{
	#ifdef _DEBUG
		debug().info("Cloning Clyde", DebugMessage::INITIALIZATION);
	#endif

	return new ClydeAI(*this);
}

void ClydeAI::chase()
{
	AI::chase(new ClydeChaseBehaviour(enemy()->scatterTiles()));
}

ClydeAI::~ClydeAI()
{ }
