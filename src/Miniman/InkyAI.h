#ifndef INKYAI_H_
#define INKYAI_H_

#include "AI.h"

/// <summary>
/// Defines the AI for Inky; the least predictable of the ghosts.
/// </summary>
class InkyAI : public AI
{
public:
	InkyAI();
	InkyAI(const InkyAI& other);
	virtual ~InkyAI();
	virtual AI* clone() const;

	/// <summary>
	/// Switches AI behaviour to chase mode.
	/// </summary>
	virtual void chase();
};

#endif