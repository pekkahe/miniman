#ifndef MOVEMENTMOTOR_H_
#define MOVEMENTMOTOR_H_

#include <bitset>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Global.h"
#include "IMovementMotor.h"

/// <summary>
/// Base class for character movement and position management.
/// </summary>
class MovementMotor : public IMovementMotor
{
public:
	~MovementMotor();
	MovementMotor();
	MovementMotor(const MovementMotor& other);

	/// <summary>
	/// Updates movement behaviour. Called once per frame.
	/// </summary>
	virtual void update();

	/// <summary>
	/// Moves instantly to the specified tile.
	/// </summary>
	virtual void setPosition(const sf::Vector2u& tile);

	/// <summary>
	/// Sets the adjacent tile on the given direction as the movement target.
	/// </summary>
	virtual void move(MoveDirection direction);

	/// <summary>
	/// Resets behaviour and stops moving.
	/// </summary>
	virtual void reset();
	
	/// <summary>
	/// Sets the current speed. 
	/// </summary>
	virtual void speed(float speed) { speed_ = speed; }

	/// <summary>
	/// Returns the current speed.
	/// </summary>
	virtual float speed() const { return speed_; }

	/// <summary>
	/// Returns the current velocity.
	/// </summary>
	virtual sf::Vector2f velocity() const;

	/// <summary>
	/// Returns the current screen position of the character.
	/// </summary>
	virtual sf::Vector2f position() const { return position_; }

	/// <summary>
	/// Returns the current tile position of the character.
	/// </summary>
	virtual sf::Vector2u tile() const { return tile_; }

	/// <summary>
	/// Returns the previous tile position of the character.
	/// </summary>
	virtual sf::Vector2u previousTile() const { return previousTile_; }

	/// <summary>
	/// Returns the movement direction vector.
	/// </summary>
	virtual sf::Vector2f direction() const;

	/// <summary>
	/// Returns the movement direction enumeration.
	/// </summary>
	virtual MoveDirection moveDirection() const { return moveDirection_; }
	
	/// <summary>
	/// Returns true if the character moved this frame.
	/// </summary>
	virtual bool isMoving() const { return isMoving_; }

	/// <summary>
	/// Returns true if the adjacent tile at the given direction is movable.
	/// </summary>
	virtual bool canMoveTowards(MoveDirection direction) const;

	/// <summary>
	/// Returns true if the tile at the specified position is movable.
	/// </summary>
	virtual bool canMoveTo(const sf::Vector2u& position) const;

protected:
	MoveDirection currentDirection() const { return moveDirection_; }

protected:
	sf::Vector2u tile_;
	sf::Vector2u previousTile_;
	std::vector<Tile> movableTiles_;

private:
	/// <summary>
	/// Updates position towards movement target and returns true if target is reached.
	/// </summary>
	bool moveTowardsTarget();

	void setTarget(const sf::Vector2u& tile);
	virtual MoveDirection getNextDirection() = 0;
	virtual sf::Vector2u getNextTile(MoveDirection direction) const;

private:
	sf::Vector2f position_;
	sf::Vector2f target_;
	MoveDirection moveDirection_;
	float speed_;
	bool isMoving_;
};

#endif
