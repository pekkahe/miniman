#include "GUIMenu.h"
#include "Locator.h"
#include "Game.h"

GUIMenu::GUIMenu()
{
	// Initialize the active menu item indicator
	indicator_.setFillColor(sf::Color::Transparent);
	indicator_.setOutlineColor(sf::Color::White);
	indicator_.setOutlineThickness(4.0);
}

bool GUIMenu::loadFont(const std::string& fontFile)
{
	return Locator::resource()->loadFont(fontFile, &font_);
}

void GUIMenu::setWindowSize(const sf::Vector2u& size)
{
	windowSize_ = size;
	windowCenter_ = sf::Vector2f(static_cast<float>(size.x / 2), static_cast<float>(size.y / 2));
}

void GUIMenu::setBackground(sf::Color color)
{
	// Create a new sf::Image with the specified color to fill the whole menu
	backgroundImage_.create(windowSize_.x, windowSize_.y, color);
	backgroundTexture_.loadFromImage(backgroundImage_);
	backgroundSprite_.setTexture(backgroundTexture_);
}
 
void GUIMenu::setTitle(const std::string& text, int characterSize, int offset)
{
	title_.setFont(font_);
	title_.setCharacterSize(characterSize);
	title_.setColor(sf::Color::Yellow);
	title_.setString(text);
	
	sf::FloatRect bounds(title_.getGlobalBounds());

	title_.setPosition(sf::Vector2f(windowCenter_.x - bounds.width / 2, static_cast<float>(offset)));
}

void GUIMenu::setSubtitle(const std::string& text, int characterSize, int offset)
{
	subtitle_.setFont(font_);
	subtitle_.setCharacterSize(characterSize);
	subtitle_.setColor(sf::Color::Yellow);
	subtitle_.setString(text);

	sf::FloatRect bounds(subtitle_.getGlobalBounds());
	sf::FloatRect titleBounds(title_.getGlobalBounds());

	subtitle_.setPosition(sf::Vector2f(windowCenter_.x - bounds.width / 2, titleBounds.top + titleBounds.height + offset));
}

void GUIMenu::addItem(const std::string& displayText, menu_action action)
{
	sf::Text item;
	item.setFont(font_);
	item.setCharacterSize(60);
	item.setColor(sf::Color::White);
	item.setString(displayText);

	// Add the new item to the end of the collection
	items_.push_back(item);

	// Store the menu action function pointer
	actions_.push_back(action); 

	// Reposition all existing menu items
	int itemCount = items_.size();
	float itemHeight = item.getGlobalBounds().height + 50;
	float menuHeight = itemHeight * itemCount;

	for (int i = 0; i < itemCount; ++i)
	{
		float width = items_[i].getGlobalBounds().width;
		float x = windowCenter_.x - width / 2;
		float itemY = (i * itemHeight) + (itemHeight / 2);
		float y = windowCenter_.y - (menuHeight / 2) + itemY;

		items_[i].setPosition(sf::Vector2f(x, y));
	}

	// Set the first item as the active one
	setSelected(0);
}


void GUIMenu::addText(const std::string& text, const sf::Vector2f& position, unsigned int characterSize, sf::Color color)
{
	sf::Text item;
	item.setFont(font_);
	item.setCharacterSize(characterSize);
	item.setColor(color);
	item.setString(text);
	item.setPosition(position);

	texts_.push_back(item);
}

void GUIMenu::updateIndicator(const sf::Text& item)
{ 
	const int padding = 7;

	auto bounds(item.getLocalBounds());
	auto position(item.getPosition());
	auto size(sf::Vector2f(bounds.width, bounds.height));
	
	size.x += padding * 2;
	size.y += padding * 2;
	position.x -= padding;
	position.y += bounds.top - padding;

	indicator_.setSize(size);
	indicator_.setPosition(position);
}

void GUIMenu::setSelected(unsigned int index)
{
	if (index < 0 || index > items_.size() - 1)
		return;

	selectedItem_ = index;
	updateIndicator(items_[index]);
}

bool GUIMenu::moveUp()
{
	if (selectedItem_ > 0)
	{
		selectedItem_--;
		updateIndicator(items_[selectedItem_]);
		return true;
	}

	return false;
}

bool GUIMenu::moveDown()
{
	if (selectedItem_ < items_.size() - 1)
	{
		selectedItem_++;
		updateIndicator(items_[selectedItem_]);
		return true;
	}

	return false;
}

void GUIMenu::select()
{
	// Call the function on the Game instance for the triggered menu item
	(Locator::game()->*actions_[selectedItem_])();
}

void GUIMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(backgroundSprite_);
	target.draw(title_);
	target.draw(subtitle_);

	// Draw menu items
	for (auto it = items_.begin(); it != items_.end(); ++it) {
		target.draw(*it);
	}

	// Draw plain text elements, if any
	for (auto it = texts_.begin(); it != texts_.end(); ++it) {
		target.draw(*it);
	}

	target.draw(indicator_);
}

GUIMenu::~GUIMenu()
{ }
