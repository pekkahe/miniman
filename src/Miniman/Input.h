#ifndef INPUT_H_
#define INPUT_H_

#include <SFML/Graphics.hpp>

/// <summary>
/// Game component responsible for handling the player input.
/// </summary>
class Input
{
public:
	/// <summary>
	/// Handle keyboard input for the specified key code.
	/// </summary>
	void read(const sf::Keyboard::Key keyCode);

private:
	void readMenuInput(const sf::Keyboard::Key keyCode);
	void readGameplayInput(const sf::Keyboard::Key keyCode);
};

#endif