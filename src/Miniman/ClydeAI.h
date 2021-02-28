#ifndef CLYDEAI_H_
#define CLYDEAI_H_

#include <vector>
#include <SFML/Graphics.hpp>
#include "AI.h"

/// <summary>
/// Defines the AI for Clyde; the least dangerous ghost.
/// </summary>
class ClydeAI : public AI
{
public:
	ClydeAI();
	ClydeAI(const ClydeAI& other);
	virtual ~ClydeAI();
	virtual AI* clone() const;

	/// <summary>
	/// Switches AI behaviour to chase mode.
	/// </summary>
	virtual void chase();
};

#endif