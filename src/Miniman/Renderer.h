#ifndef RENDERER_H_
#define RENDERER_H_

namespace sf { class RenderWindow; }
class Level;

/// <summary>
/// Game component responsible for rendering the game objects.
/// </summary>
class Renderer
{
public:
	Renderer(sf::RenderWindow& window);
	~Renderer();

	/// <summary>
	/// Initialize the game viewport for the given level.
	/// </summary>
	void initViewport(const Level& level);

	/// <summary>
	/// Draw the game objects. Called once per frame.
	/// </summary>
	void draw();

	/// <summary>
	/// Returns the size of the game window.
	/// </summary>
	sf::Vector2u getWindowSize() const;
	
	/// <summary>
	/// Returns the screen size and position of the game viewport in pixel coordinates.
	/// </summary>
	sf::IntRect getGameViewRect() const;

private:
	void drawGameObjects();

private:
	sf::RenderWindow& window_;
	sf::View gameView_;
};

#endif