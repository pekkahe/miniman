#ifndef GUI_H_
#define GUI_H_

#include <SFML/Graphics.hpp>
#include "GUIMenu.h"

/// <summary>
/// Game component responsible for drawing the GUI.
/// </summary>
class GUI : public sf::Drawable
{
public:
	/// <summary>
	/// Initializes the GUI and returns true if successful.
	/// </summary>
	bool init(const std::string& fontFile, const std::string& playerTextureFile);

	/// <summary>
	/// Updates the displayed player score to the given amount.
	/// </summary>
	void updateScore(int score);

	/// <summary>
	/// Updates the displayed remaining player lives to the given amount.
	/// </summary>
	void updateLivesLeft(int lives);

	/// <summary>
	/// Updates the displayed remaining dots to the given amount.
	/// </summary>
	/// <remarks>
	/// Only displayed in debug builds.
	/// </remarks>
	void updateDotsLeft(int dots);

	/// <summary>
	/// Updates the displayed high score.
	/// </summary>
	void updateHighScore();

	/// <summary>
	/// Returns the active displayed menu or a <c>nullptr</c> if no menu is active.
	/// </summary>
	GUIMenu* menu();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawGameplayGui(sf::RenderTarget& target, sf::RenderStates states) const;
	
private:
	sf::Font font_;
	sf::Text highScoreTitle_;
	sf::Text highScoreValue_;
	sf::Text playerScoreTitle_;
	sf::Text playerScoreValue_;
	sf::Text dotsLeft_;

	sf::Texture playerTexture_;
	sf::Sprite livesLeft_;

	GUIMenu mainMenu_;
	GUIMenu pauseMenu_;
	GUIMenu gameOverMenu_;
};

#endif