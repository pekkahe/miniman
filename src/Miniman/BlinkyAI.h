#ifndef BLINKYAI_H_
#define BLINKYAI_H_

#include "AI.h"

/// <summary>
/// Defines the AI for Blinky; the most aggressive ghost.
/// </summary>
class BlinkyAI : public AI
{
public:
	BlinkyAI();
	BlinkyAI(const BlinkyAI& other);
	virtual ~BlinkyAI();
	virtual AI* clone() const;

	/// <summary>
	/// Switches AI behaviour to chase mode.
	/// </summary>
	virtual void chase();
};

#endif
