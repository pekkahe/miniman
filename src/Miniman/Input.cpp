#include "Locator.h"
#include "Input.h"
#include "GUI.h"
#include "Player.h"
#include "Game.h"
#include "AudioManager.h"

void Input::read(const sf::Keyboard::Key keyCode)
{
	switch (Locator::game()->gameState())
	{
		case GAMESTATE_MAINMENU:
		case GAMESTATE_PAUSED:
		case GAMESTATE_GAMEOVER:
			readMenuInput(keyCode);
			break;

		case GAMESTATE_GAMEPLAY:
			readGameplayInput(keyCode);
			break;
	}
}

void Input::readMenuInput(const sf::Keyboard::Key keyCode)
{
	if (Locator::gui()->menu() == nullptr)
		return;

	switch (keyCode)
	{
		case sf::Keyboard::Escape:
		case sf::Keyboard::P:
			Locator::game()->unpause();
			break;

		case sf::Keyboard::Return:
			Locator::gui()->menu()->select();
			Locator::audio()->playMenuTriggerSound(0.3f);
			break;

		case sf::Keyboard::Up:
			if (Locator::gui()->menu()->moveUp())
				Locator::audio()->playMenuSelectSound(0.3f);
			break;

		case sf::Keyboard::Down:
			if (Locator::gui()->menu()->moveDown())
				Locator::audio()->playMenuSelectSound(0.3f);
			break;

		default:
			break;
	}
}

void Input::readGameplayInput(const sf::Keyboard::Key keyCode)
{
	switch (keyCode)
	{
		case sf::Keyboard::M:
			Locator::audio()->mute(!Locator::audio()->isMuted());
			break;

		case sf::Keyboard::Escape:
		case sf::Keyboard::P:
			Locator::game()->pause();
			break;

		case sf::Keyboard::Up:
			Locator::player()->moveNext(MoveDirection::MOVE_UP);
			break;

		case sf::Keyboard::Down:
			Locator::player()->moveNext(MoveDirection::MOVE_DOWN);
			break;

		case sf::Keyboard::Left:
			Locator::player()->moveNext(MoveDirection::MOVE_LEFT);
			break;

		case sf::Keyboard::Right:
			Locator::player()->moveNext(MoveDirection::MOVE_RIGHT);
			break;

		default:
			break;
	}
}