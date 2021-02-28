#ifndef ANIMATOR_CONTROLLER_H_
#define ANIMATOR_CONTROLLER_H_

#include <SFML\Graphics.hpp>
#include <vector>
#include <map>
#include "IAnimatorController.h"

class IMovementMotor;

/// <summary>
/// Base class for a sprite sheet based character animator.
/// </summary>
class AnimatorController : public IAnimatorController
{
public:
	AnimatorController(IMovementMotor* motor, const sf::Time& animationSpeed);
	AnimatorController(const AnimatorController& other);
	virtual ~AnimatorController();

	/// <summary>
	/// Loads the animated sprite sheet from the given file.
	/// </summary>
	virtual bool loadSpriteSheet(const std::string& fileName);

	/// <summary>
	/// Updates the character animator. Called once per frame.
	/// </summary>
	virtual void update();

	/// <summary>
	/// Resets the character animator.
	/// </summary>
	virtual void reset();

	/// <summary>
	/// Returns the active sprite.
	/// </summary>
	const sf::Sprite& sprite() { return sprite_; }

	/// <summary>
	/// Returns the screen size of a single sprite.
	/// </summary>
	const sf::Vector2u& spriteSize() { return spriteSize_; }

	/// <summary>
	/// Returns the total count of horizontal and vertical sprites
	/// in the animated sprite sheet.
	/// </summary>
	const sf::Vector2u& spriteCount() { return spriteCount_; }
	
protected:
	/// <summary>
	/// Animates the sprite sheet by switching to the next designated sprite.
	/// </summary>
	virtual void animate() = 0;

	/// <summary>
	/// Returns the sprite sheet index of the active sprite.
	/// </summary>
	int spriteIndex() const { return spriteIndex_; }

	/// <summary>
	/// Returns the sprite sheet index of the previously active sprite.
	/// </summary>
	int previousSpriteIndex() const { return previousSpriteIndex_; }
		
	/// <summary>
	/// Updates to the previous sprite in the sprite sheet, 
	/// if current sprite is not already the first sprite.
	/// Returns true if update was successful, false otherwise.
	/// </summary> 
	bool setNextSprite();

	/// <summary>
	/// Updates to the next sprite in the sprite sheet,
	/// if current sprite is not already the last sprite.
	/// Returns true if update was successful, false otherwise.
	/// </summary> 
	bool setPreviousSprite();

	/// <summary>
	/// Updates to the given sprite index in the sprite sheet,
	/// if index is valid and the current sprite is a different one.
	/// Returns true if update was successful, false otherwise.
	/// </summary> 
	bool setSprite(unsigned int spriteSheetIndex);
	
	/// <summary>
	/// Chooses the next sprite from the sprite sheet.
	/// </summary>
	void updateSprite();

	/// <summary>
	/// Updates the active sprite by ping ponging the sprites in the specified 
	/// sprite sheet row. Every subsequent call will either increment or decrement
	/// the sprite index, ensuring the selected sprite stays inside the row boundaries.
	/// </summary>
	void pingPongSprite(unsigned int spriteSheetRow);

	void changeSpeed(sf::Time animationSpeed);

private:
	IMovementMotor* motor_;
	sf::Vector2u spriteSize_;
	sf::Vector2u spriteCount_;
	sf::Texture spriteSheet_;
	sf::Sprite sprite_;

	std::map<int, sf::IntRect> sprites_;
	sf::Time animationSpeed_;
	sf::Time elapsed_;

	unsigned int spriteIndex_;
	int previousSpriteIndex_;
};

#endif