#include "AnimatorController.h"
#include <iostream>
#include "Global.h"
#include "Game.h"
#include "Locator.h"
#include "ResourceManager.h"
#include "IMovementMotor.h"

AnimatorController::AnimatorController(IMovementMotor* motor, const sf::Time& animationSpeed) :
	motor_(motor),
	spriteIndex_(0),
	previousSpriteIndex_(-1), // Must be lower than sprite index
	spriteSize_(32, 32),
	animationSpeed_(animationSpeed),
	elapsed_(sf::Time::Zero)
{ }

AnimatorController::AnimatorController(const AnimatorController& other) :
	motor_(other.motor_),
	spriteIndex_(other.spriteIndex_),
	previousSpriteIndex_(other.previousSpriteIndex_),
	spriteSize_(other.spriteSize_),
	spriteCount_(other.spriteCount_),
	animationSpeed_(other.animationSpeed_),
	elapsed_(other.elapsed_),
	spriteSheet_(other.spriteSheet_),
	sprite_(other.sprite_),
	sprites_(other.sprites_)
{ }

bool AnimatorController::loadSpriteSheet(const std::string& fileName)
{
	// Load sprite sheet texture
	if (!Locator::resource()->loadTexture(fileName, &spriteSheet_)) {
		debug().error("Failed to load texture " + fileName);
		return false;
	}

	// Verify format is 32 x 32, or what ever we have defined for sprite size
	auto size = spriteSheet_.getSize();
	if (size.x % spriteSize_.x != 0 || size.y % spriteSize_.y != 0) {
		debug().error("Spritesheet format is invalid: " + fileName);
		return false;
	}

	// Map sprite sheet texture to a Sprite
	sprite_.setTexture(spriteSheet_);
	
	// Calculate how many sprites we have in the sheet
	spriteCount_ = sf::Vector2u(size.x / spriteSize_.x, size.y / spriteSize_.y);
	if (spriteCount_.x == 0 || spriteCount_.y == 0) {
		debug().error("Spritesheet is empty: " + fileName);
		return false;
	}

	sprites_.clear();
	
	// Map the sprites into rectangles describing their position in the sheet
	for (unsigned int i = 0; i < spriteCount_.x; ++i) {
		for (unsigned int j = 0; j < spriteCount_.y; ++j) {
			unsigned int spriteIndex = i + j * spriteCount_.x;
			sprites_[spriteIndex] = sf::IntRect(i * spriteSize_.x, j * spriteSize_.y, spriteSize_.x, spriteSize_.y);;
		}
	}

	// Set first sprite
	spriteIndex_ = 0;
	sprite_.setTextureRect(sprites_[spriteIndex_]);

	return true;
}

void AnimatorController::update()
{ 
	elapsed_ += Locator::game()->deltaTime();

	if (elapsed_ > animationSpeed_)
	{
		animate();
		elapsed_ = sf::Time::Zero;
	}
}

void AnimatorController::reset()
{
	spriteIndex_ = 0;
	previousSpriteIndex_ = -1;
	elapsed_ = sf::Time::Zero;

	updateSprite();
}

void AnimatorController::changeSpeed(sf::Time animationSpeed)
{ 
	animationSpeed_ = animationSpeed;
}

void AnimatorController::updateSprite()
{
	sprite_.setTextureRect(sprites_[spriteIndex_]);
}

bool AnimatorController::setSprite(unsigned int spriteSheetIndex)
{
	if (spriteSheetIndex != spriteIndex_ && spriteSheetIndex >= 0 && spriteSheetIndex < sprites_.size()) {
		spriteIndex_ = spriteSheetIndex;
		updateSprite();

		return true;
	}

	return false;
}

bool AnimatorController::setNextSprite()
{
	if (spriteIndex_ < sprites_.size() - 1) {
		spriteIndex_++;
		updateSprite();

		return true;
	}

	return false;
}

bool AnimatorController::setPreviousSprite()
{
	if (spriteIndex_ > 0) {
		spriteIndex_--;
		updateSprite();

		return true;
	}

	return false;
}

void AnimatorController::pingPongSprite(unsigned int spriteSheetRow)
{
	unsigned int minIndex = spriteSheetRow * spriteCount_.x;
	unsigned int maxIndex = spriteSheetRow * spriteCount_.x + (spriteCount_.x - 1);

	bool increasing = (int) spriteIndex_ > previousSpriteIndex_;
	previousSpriteIndex_ = spriteIndex_;

	unsigned int nextIndex = spriteIndex_;
	if (spriteIndex_ < maxIndex && increasing) {
		nextIndex++;
	} else if (spriteIndex_ > minIndex) {
		nextIndex--;
	} else if (spriteIndex_ < maxIndex) {
		nextIndex++; // Single cycle
	}

	setSprite(nextIndex);
}

AnimatorController::~AnimatorController()
{ }