#ifndef PINKYAI_H_
#define PINKYAI_H_

#include "AI.h"

/// <summary>
/// Defines the AI for Pinky; the ghost which attempts to box in the player.
/// </summary>
class PinkyAI : public AI
{
public:
	PinkyAI();
	PinkyAI(const PinkyAI& other);
	virtual ~PinkyAI();
	virtual AI* clone() const;

	/// <summary>
	/// Switches AI behaviour to chase mode.
	/// </summary>
	virtual void chase();
};

#endif