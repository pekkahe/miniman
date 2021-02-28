#ifndef GUI_MENU_H_
#define GUI_MENU_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>

class Game;

/// <summary>
/// Function pointer to a parameterless void method which acts as our menu item action.
/// </summary>
/// <remarks>
/// It seems that due to function pointer restrictions (?) we cannot use function
/// implementations from multiple different classes. Therefore, all menu actions
/// here are implemented in the <c>Game</c> class.
/// </remarks>
typedef void(Game::*menu_action)(void);

/// <summary>
/// Defines a drawable GUI menu.
/// </summary>
class GUIMenu : public sf::Drawable
{
public:
	GUIMenu();
	~GUIMenu();
	
	/// <summary>
	/// Loads the specified font file for the menu.
	/// </summary>
	bool loadFont(const std::string& fontFile);

	/// <summary>
	/// Sets the size of the menu.
	/// </summary>
	void setWindowSize(const sf::Vector2u& size);
	
	/// <summary>
	/// Sets the background color of the menu.
	/// </summary>
	void setBackground(sf::Color color);
	
	/// <summary>
	/// Sets the title text for the menu.
	/// </summary>
	void setTitle(const std::string& text, int characterSize, int offset = 0);
	
	/// <summary>
	/// Sets the optional subtitle text for the menu.
	/// </summary>
	void setSubtitle(const std::string& text, int characterSize, int offset = 0);

	/// <summary>
	/// Adds a new item to the menu with the specified text and action.
	/// </summary>
	void addItem(const std::string& displayText, menu_action action);
	
	/// <summary>
	/// Adds plain text to the menu without any triggerable action. 
	/// </summary>
	void addText(const std::string& text, const sf::Vector2f& position, unsigned int characterSize, sf::Color color);
	
	/// <summary>
	/// Selects the active menu item and triggers its action.
	/// </summary>
	void select();

	/// <summary>
	/// Moves up in the menu and updates the active menu item.
	/// </summary>
	bool moveUp();

	/// <summary>
	/// Moves down in the menu and updates the active menu item.
	/// </summary>
	bool moveDown();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateIndicator(const sf::Text& item);
	void setSelected(unsigned int index);

private:
	sf::Font font_;
	sf::Vector2u windowSize_;
	sf::Vector2f windowCenter_;
	
	sf::Text title_;
	sf::Text subtitle_;
	
	sf::RectangleShape indicator_;
	sf::Image backgroundImage_;
	sf::Texture backgroundTexture_;
	sf::Sprite backgroundSprite_;

	std::vector<sf::Text> texts_;
	std::vector<sf::Text> items_;
	std::vector<menu_action> actions_;
	unsigned int selectedItem_;
};

#endif