#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <SFML/Graphics.hpp>
#include "Global.h"

class IMovementMotor;
class IAnimatorController;

/// <summary>
/// Base class for game actors.
/// </summary>
class Character : public sf::Drawable, public sf::Transformable
{
public:
	Character();
	Character(const Character& other);
	virtual ~Character();

	/// <summary>
	/// Loads the character's sprite sheet from the given file. 
	/// </summary>
	virtual bool loadSpriteSheet(const std::string& fileName);

	/// <summary>
	/// Returns the character's current screen position.
	/// </summary>
	sf::Vector2f position() const;

	/// <summary>
	/// Returns the character's current tile position.
	/// </summary>
	sf::Vector2u tile() const;

	/// <summary>
	/// Returns the character's current direction vector.
	/// </summary>
	sf::Vector2f direction() const;

	/// <summary>
	/// Returns the character's current direction enumeration.
	/// </summary>
	MoveDirection moveDirection() const;

	/// <summary>
	/// Instantly moves the character to the given tile position.
	/// </summary>
	void setTilePosition(const sf::Vector2u& tile);

	/// <summary>
	/// Sets the character's movement towards to the specified direction.
	/// </summary>
	virtual void move(MoveDirection direction);

	/// <summary>
	/// Adds the specified offset to the character's origin. By default, the origin
	/// is at the center of the sprite.
	/// </summary>	
	/// <remarks>
	/// Can be used to draw the character slightly misplaced, e.g. between two tiles.
	/// </remarks>
	void setOffset(const sf::Vector2f& offset);

	/// <summary>
	/// Resets the character's origin to the center of the sprite.
	/// </summary>
	void resetOffset();

	/// <summary>
	/// Updates character behaviour. Called once per frame.
	/// </summary>
	virtual void update() = 0;

	/// <summary>
	/// Resets character state.
	/// </summary>
	virtual void reset();

protected:
	IMovementMotor* motor_;
	IAnimatorController* animator_;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void resetOrigin();
};

#endif
