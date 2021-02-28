#ifndef IMOVEMENT_MOTOR_H_
#define IMOVEMENT_MOTOR_H_

class IMovementMotor
{
public:
	virtual ~IMovementMotor() { }

	virtual void update() = 0;

	virtual void speed(float speed) = 0;

	virtual float speed() const = 0;

	virtual sf::Vector2f velocity() const = 0;

	virtual bool isMoving() const = 0;

	virtual void reset() = 0;

	virtual void setPosition(const sf::Vector2u& tile) = 0;

	virtual void move(MoveDirection direction) = 0;

	virtual sf::Vector2f position() const = 0;

	virtual sf::Vector2u tile() const = 0;

	virtual sf::Vector2u previousTile() const = 0;

	virtual sf::Vector2f direction() const = 0;

	virtual MoveDirection moveDirection() const = 0;
};

#endif