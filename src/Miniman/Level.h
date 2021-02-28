#ifndef LEVEL_H_
#define LEVEL_H_

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "AStar\AStar.h"
#include "Global.h"
#include "HighlighterTest.h"

/// <summary>
/// Level tiles defined by their tile map indexes. 
/// </summary>
enum Tile
{
	TILE_DOT = 0,							// Adds score. Changes to empty tile.
	TILE_ENERGIZER = 1,						// Adds extra score and triggers ghost frightening. Changes to empty tile.
	TILE_EMPTY = 2,							// Empty tile with no special features.
	TILE_TELEPORT = 3,						// Triggers teleportation; moves a character into another teleport tile.
	TILE_GHOST_PEN_ENTRY = 4,				// Ghost pen entry; has special movement for dead ghosts.
	TILE_TOP_WALL = 10,						// Top wall. Non-movable.
	TILE_BOTTOM_WALL = 11,					// Bottom wall. Non-movable.
	TILE_RIGHT_WALL = 12,					// Right wall. Non-movable.
	TILE_LEFT_WALL = 13,					// Left wall. Non-movable.
	TILE_FULL_WALL = 14,					// Full wall. Non-movable.
	TILE_TOPLEFT_OUTER_WALL = 20,			// Top-left outer corner. Non-movable.
	TILE_TOPRIGHT_OUTER_WALL = 21,			// Top-right outer corner. Non-movable.
	TILE_BOTTOMLEFT_OUTER_WALL = 22,		// Bottom-left outer corner. Non-movable.
	TILE_BOTTOMRIGHT_OUTER_WALL = 23,		// Bottom-right outer corner. Non-movable.
	TILE_TOPLEFT_INNER_WALL = 30,			// Top-left inner corner. Non-movable.
	TILE_TOPRIGHT_INNER_WALL = 31,			// Top-right inner corner. Non-movable.
	TILE_BOTTOMLEFT_INNER_WALL = 32,		// Bottom-left inner corner. Non-movable.
	TILE_BOTTOMRIGHT_INNER_WALL = 33,		// Bottom-right inner corner. Non-movable.
	TILE_GHOST_PEN_DOOR = 40,				// Ghost pen door. Only movable by ghosts.
	TILE_GHOST_PEN = 41,					// Ghost pen tile.
	TILE_DEBUG_TRACK = 42,					// Debug helper. Draws a brown cross.
	TILE_DEBUG_YELLOW = 43,					// Debug helper. Draws a yellow cross.
	TILE_DEBUG_RED = 44,					// Debug helper. Draws a red cross.
};

class Character;

/// <summary>
/// Game component for level management. The active level is stored in a tile map which
/// is defined in an external file. Each element in the tile map is represented by a
/// <c>Tile</c> enumeration.
/// </summary>
class Level : public sf::Drawable, public sf::Transformable
{
	friend class tests::HighlighterTest;
	friend class astar::Graph;

public:
	Level();
	~Level();
	
	/// <summary>
	/// Initializes the level from the tile map and takes ownership of the array.
	/// Returns true if successful.
	/// </summary>
	/// <param name="tileset">The file name of the tileset texture.</param>
	/// <param name="tileSize">The size of a tile in pixels.</param>
	/// <param name="tileMap">The tile map array.</param>
	/// <param name="width">The width of the map in tiles.</param>
	/// <param name="height">The height of the map in tiles.</param>
	bool load(const std::string& tileset, sf::Vector2u tileSize, int* tileArray, unsigned int width, unsigned int height);

	/// <summary>
	/// Resets the level to its original state.
	/// </summary>
	void reset();

	/// <summary>
	/// Notifies the level that a dot has been eaten from the tile at the given position.
	/// </summary>
	/// <exception cref="OutOfBoundsException">If position is out of tile map bounds.</exception>
	bool eatDot(const sf::Vector2u& position);

	/// <summary>
	/// Returns the tile for the given position.
	/// </summary>		
	/// <exception cref="OutOfBoundsException">If position is out of tile map bounds.</exception>
	Tile getTile(const sf::Vector2u& position) const;
	
	/// <summary>
	/// Returns the center screen pixel position for the tile at the given position.
	/// </summary>
	sf::Vector2f getTileCenter(const sf::Vector2u& position) const;

	/// <summary>
	/// Returns the top left screen pixel position for the tile at the given position.
	/// </summary>
	sf::Vector2f mapToScreenPoint(const sf::Vector2u& position) const;

	/// <summary>
	/// Returns the tile position for the given screen pixel position.
	/// </summary>
	sf::Vector2u mapToTilePosition(const sf::Vector2f& screenPoint) const;

	/// <summary>
	/// Returns the target position for the teleport tile at the given position,
	/// or if the tile is not a teleport tile, returns <c>NO_SUCH_POSITION</c>.
	/// </summary>
	/// <exception cref="OutOfBoundsException">If position is out of tile map bounds.</exception>
	sf::Vector2u getTeleportTarget(const sf::Vector2u& position) const;

	/// <summary>
	/// Returns the position for the ghost pen door tile.
	/// </summary>
	sf::Vector2u getGhostPenDoor() const;

	/// <summary>
	/// Returns the position for the ghost pen entry tile.
	/// </summary>
	sf::Vector2u getGhostPenEntry() const;

	/// <summary>
	/// Returns the tile size of the level.
	/// </summary>
	sf::Vector2u size() const;

	/// <summary>
	/// Returns the pixel size of the level.
	/// </summary>
	sf::Vector2f screenSize() const { return screenSize_; }

	/// <summary>
	/// Returns the number of dots left in the level.
	/// </summary>
	int dotsLeft() const;

	/// <summary>
	/// Returns true if the specified tile position is valid, i.e. inside bounds.
	/// </summary>
	bool isValid(const sf::Vector2u& position) const;

	/// <summary>
	/// Returns true if the tile in the specified position is a teleport tile.
	/// </summary>
	bool isTeleport(const sf::Vector2u& position) const;

private:
	/// <summary>
	/// Returns the tile map array index for the tile at the given position.
	/// </summary>
	int getIndex(const sf::Vector2u& position) const;

	/// <summary>
	/// Returns the position for the tile at the given tile map array index.
	/// </summary>
	sf::Vector2u getPosition(int index) const;

	/// <summary>
	/// Throws an exception if the specified tile position is out of tile map bounds.
	/// </summary>
	/// <exception cref="OutOfBoundsException">If position is out of tile map bounds.</exception>
	void validate(const sf::Vector2u& position) const;

	void updateTexture(int index);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
private:
	int* tileMap_;
	int* tileMapCopy_;
	
	sf::VertexArray vertices_;
	sf::Texture tileset_;
	sf::Vector2u tileSize_;
	sf::Vector2f screenSize_;
	unsigned int width_;
	unsigned int height_;
	size_t size_;
	
	/// <summary>
	/// Consumable dots and energizers represented by their tile map indexes
	/// and whether they have been eaten or not.
	/// </summary>
	std::map<int, bool> dots_;

	/// <summary>
	/// Teleport tiles represented by their tile map indexes and the target
	/// tile where to teleport into.
	/// </summary>
	std::map<int, int> teleports_;

	/// <summary>
	/// Ghost pen doors represented by their tile map indexes.
	/// </summary>
	std::vector<int> ghostPenDoors_;

	/// <summary>
	/// Ghost pen door entries represented by their tile map indexes.
	/// </summary>
	std::vector<int> ghostPenEntry_;
};

#endif