#ifndef IENEMY_MOVEMENT_MOTOR_H_
#define IENEMY_MOVEMENT_MOTOR_H_

///
/// This interface is not used anywhere yet. Should we implement it or would it add an excessive abstaction layer?
///

#include "IMovementMotor.h"

class IEnemyMovementMotor : public IMovementMotor
{
public:
	virtual ~IEnemyMovementMotor() { }

	void startTracking() = 0;
	bool isTracking() const = 0;

	void setPath() = 0;

	void allowMovement() = 0;
	void restrictMovement() = 0;

	void onPathEnd() = 0;
	void onTile() = 0;
}

#endif