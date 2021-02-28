#ifndef IANIMATOR_CONTROLLER_H_
#define IANIMATOR_CONTROLLER_H_

class IAnimatorController
{
public:
	virtual ~IAnimatorController() { }

	virtual void update() = 0;

	virtual bool loadSpriteSheet(const std::string& fileName) = 0;

	virtual const sf::Sprite& sprite() = 0;
	
	virtual const sf::Vector2u& spriteSize() = 0;
};

#endif