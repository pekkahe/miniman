#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <cmath>
#include "Level.h"
#include "Locator.h"
#include "ResourceManager.h"
#include "Global.h"
#include "GUI.h"
#include "Character.h"
#include "EnemyManager.h"
#include "Game.h"

Level::Level() :
	tileMap_(nullptr),
	tileMapCopy_(nullptr)
{ }
	
bool Level::load(const std::string& tileset, sf::Vector2u tileSize, int* tileMap, unsigned int width, unsigned int height)
{
	// Load the tileset texture
	if (!Locator::resource()->loadTexture(tileset, &tileset_))
		return false;

	// Store the tile map dimensions
	width_ = width;
	height_ = height;
	size_ = width * height;
	tileSize_ = tileSize;
	screenSize_ = sf::Vector2f(static_cast<float>(width * tileSize.x), static_cast<float>(height * tileSize.y));

	// Take ownership of the tile map array
	tileMap_ = tileMap;

	// Create a copy of the tile map so we can easily 
	// reset the level to its original state
	tileMapCopy_ = new int[size_];
	std::copy(tileMap_, tileMap_ + size_, tileMapCopy_);

	// Resize the vertex array to fit the level size
	vertices_.setPrimitiveType(sf::Quads);
	vertices_.resize(width * height * 4);

	ghostPenDoors_.clear();
	ghostPenEntry_.clear();
	teleports_.clear();
	dots_.clear();
	std::vector<int> teleports;

	// Populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			// Get the array index for the current tile
			int tileIndex = i + j * width;

			// Get a pointer to the tile's quad
			sf::Vertex* quad = &vertices_[tileIndex * 4];

			// Define the tile's four position coordinates (corners)
			quad[0].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>(j * tileSize.y));
			quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>(j * tileSize.y));
			quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>((j + 1) * tileSize.y));
			quad[3].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>((j + 1) * tileSize.y));

			// Update the tile texture
			updateTexture(tileIndex);

			// Store the tile to its designated collection type
			auto tile = static_cast<Tile>(tileMap_[tileIndex]);
			switch (tile)
			{
				case TILE_DOT:
				case TILE_ENERGIZER:
					dots_[tileIndex] = false;
					break;
				case TILE_TELEPORT:
					teleports.push_back(tileIndex);
					break;
				case TILE_GHOST_PEN_ENTRY:
					ghostPenEntry_.push_back(tileIndex);
					break;
				case TILE_GHOST_PEN_DOOR:
					ghostPenDoors_.push_back(tileIndex);
					break;
				default:
					break;
			}
		}
	}

	if (dots_.size() == 0) {
		debug().error("No dots in level.");
		return false;
	}

	if (ghostPenDoors_.size() == 0 || ghostPenEntry_.size() == 0) {
		debug().error("No ghost pen doors or entries in level.");
		return false;
	}

	if (teleports.size() > 2) {
		debug().error("Too many teleports in level. Support is limited to two.");
		return false;
	}

	// Map the teleport tiles to each other
	if (teleports.size() == 2) {
		teleports_[teleports[0]] = teleports[1];
		teleports_[teleports[1]] = teleports[0];
	} 

	return true;
}

void Level::reset()
{
	// Copy the original values back to the tile map
	std::copy(tileMapCopy_, tileMapCopy_ + size_, tileMap_);

	// Update tile textures to reflect the changed tile map
	for (unsigned int i = 0; i < width_; ++i)
	{
		for (unsigned int j = 0; j < height_; ++j)
		{
			int tileIndex = i + j * width_;
			updateTexture(tileIndex);
		}
	}

	// Reset dot states to "not eaten"
	for (auto it = dots_.begin(); it != dots_.end(); ++it) {
		it->second = false;
	}
}

void Level::updateTexture(int index)
{
	// Get the tile number for the index
	int tileNumber = tileMap_[index];

	// Get a pointer to the tile's quad
	sf::Vertex* quad = &vertices_[index * 4];

	// The sprite indexes in our sprite sheet go in intervals of 10 for each row:
	// 1st row defines sprites 0..<10, 2nd row defines sprites 10..<20, etc.
	// These index values correspond to the tile map values.
	const int maxSpritesPerRow = 10;

	// Find the tile's sprite in the spritesheet;
	int i = tileNumber % maxSpritesPerRow;
	int j = tileNumber / maxSpritesPerRow;

	// Define the tile's four texture coordinates
	quad[0].texCoords = sf::Vector2f(static_cast<float>(i * tileSize_.x), static_cast<float>(j * tileSize_.y));
	quad[1].texCoords = sf::Vector2f(static_cast<float>((i + 1) * tileSize_.x), static_cast<float>(j * tileSize_.y));
	quad[2].texCoords = sf::Vector2f(static_cast<float>((i + 1) * tileSize_.x), static_cast<float>((j + 1) * tileSize_.y));
	quad[3].texCoords = sf::Vector2f(static_cast<float>(i * tileSize_.x), static_cast<float>((j + 1) * tileSize_.y));
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform
	states.transform *= getTransform();

	// Apply the tileset texture
	states.texture = &tileset_;

	// Draw the vertex array
	target.draw(vertices_, states);
}

inline bool Level::isValid(const sf::Vector2u& position) const
{
	return position.x < width_ && position.y < height_;
}

inline void Level::validate(const sf::Vector2u& position) const 
{
	if (!isValid(position)) {
		throw OutOfBoundsException(getIndex(position));
	}
}

sf::Vector2u Level::getTeleportTarget(const sf::Vector2u& position) const
{
	validate(position);
	
	int index = getIndex(position);

	auto tile = static_cast<Tile>(tileMap_[index]);

	if (tile != Tile::TILE_TELEPORT) {
		return NO_SUCH_POSITION;
	}

	return getPosition(teleports_.at(index));
}

sf::Vector2u Level::getGhostPenDoor() const
{
	// Just use the first parsed tile as our ghost pen door
	return getPosition(ghostPenDoors_[0]);
}

sf::Vector2u Level::getGhostPenEntry() const
{
	// Just use the first parsed tile as our ghost pen door entry
	return getPosition(ghostPenEntry_[0]);
}

sf::Vector2u Level::size() const
{ 
	return sf::Vector2u(width_, height_); 
}

bool Level::isTeleport(const sf::Vector2u& position) const
{
	return getTile(position) == Tile::TILE_TELEPORT;
}

bool Level::eatDot(const sf::Vector2u& position)
{
	validate(position);
	int index = getIndex(position);

	// If there's no dot at the position or it's already eaten, leave
	if (dots_.count(index) == 0 || dots_[index] == true) {
		return false;
	}

	// Update dot state to "eaten"
	dots_[index] = true;

	// Update score and notify game about dot state change
	auto tile = static_cast<Tile>(tileMap_[index]);
	if (tile == Tile::TILE_DOT) 
	{
		Locator::game()->dotEaten(ScoreModifier::SCORE_DOT);
	} 
	else if (tile == Tile::TILE_ENERGIZER) 
	{
		Locator::game()->dotEaten(ScoreModifier::SCORE_ENERGIZER);

		// Eating an energizer triggers the enemies' frightened state
		Locator::enemyManager()->frightened();
	}

	// Update tile state and texture
	tileMap_[index] = static_cast<int>(Tile::TILE_EMPTY);
	updateTexture(index);

	#ifdef _DEBUG
		debug().log("Dot eaten. Dots left: " + std::to_string(dotsLeft()), DebugMessage::LEVEL);
	#endif

	return true;
}

int Level::dotsLeft() const
{
	int dotsEaten = 0;
	for (auto it = dots_.begin(); it != dots_.end(); ++it) {
		if (it->second)
			dotsEaten++;
	}

	return dots_.size() - dotsEaten;
}

Tile Level::getTile(const sf::Vector2u& position) const
{
	validate(position);
	int index = getIndex(position);

	return static_cast<Tile>(tileMap_[index]);
}

inline int Level::getIndex(const sf::Vector2u& position) const
{
	return position.x + position.y * width_;
}

inline sf::Vector2u Level::getPosition(int index) const
{
	return sf::Vector2u(index % width_, index / width_);
}

sf::Vector2f Level::getTileCenter(const sf::Vector2u& position) const
{
	sf::Vector2f screenPosition = mapToScreenPoint(position);

	screenPosition.x += tileSize_.x / 2;
	screenPosition.y += tileSize_.y / 2;

	return screenPosition;
}

sf::Vector2f Level::mapToScreenPoint(const sf::Vector2u& position) const
{
	float x = static_cast<float>(position.x * tileSize_.x);
	float y = static_cast<float>(position.y * tileSize_.y);

	return sf::Vector2f(x, y);
}

sf::Vector2u Level::mapToTilePosition(const sf::Vector2f& screenPoint) const
{
	unsigned int x = static_cast<unsigned int>(screenPoint.x / tileSize_.x);
	unsigned int y = static_cast<unsigned int>(screenPoint.y / tileSize_.y);

	// Mapping should be inclusive, e.g. on a 16x16 tile size the level screen point (16, 16)
	// is tile (0,0) and not (1,1). For this to work, we need to check the borders with modulus.
	int xMod = ((int) screenPoint.x) % tileSize_.x;
	int yMod = ((int) screenPoint.y) % tileSize_.y;

	if(xMod == 0 && x > 0) {
		x--; // Overlapped so decrement
	}
	if(yMod == 0 && y > 0) {
		y--; // Overlapped so decrement
	}

	return sf::Vector2u(x, y);
}

Level::~Level()
{
	delete[] tileMap_; 
	delete[] tileMapCopy_;
}
