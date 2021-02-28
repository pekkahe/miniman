#include <exception>
#include <iostream>
#include "HighlighterTest.h"
#include "Locator.h"
#include "EnemyManager.h"
#include "Global.h"
#include "Level.h"

tests::HighlighterTest& tests::highlighter()
{
	static HighlighterTest test(Locator::level());
	return test;
}

tests::HighlighterTest::HighlighterTest(Level* const level) : 
	level_(level)
{ }

void tests::HighlighterTest::highlight(const sf::Vector2u& position, HighlightColor color)
{
	// Skip if we're debugging with more than one enemy
	if (Locator::enemyManager()->enemies().size() > 1) {
		return;
	}

	// Skip special tiles, so we don't break game logic
	auto levelTile = level_->getTile(position);
	if (levelTile == Tile::TILE_TELEPORT || levelTile == Tile::TILE_GHOST_PEN_DOOR) {
		return;
	}

	level_->validate(position);

	int index = position.x + position.y * level_->width_;
	int tile = static_cast<Tile>(color);

	highlighted_[index] = color;

	// Don't override the original unhighlighted value,
	// if the index is being highlighted twice
	if (preHighlight_.count(index) == 0) {
		preHighlight_[index] = level_->tileMap_[index];
	}

	level_->tileMap_[index] = tile;
	level_->updateTexture(index);
}

void tests::HighlighterTest::clear(HighlightColor highlightColor)
{
	// Skip if we're debugging with more than one enemy
	if (Locator::enemyManager()->enemies().size() > 1) {
		return;
	}

	std::vector<int> indexes;

	for (auto it = highlighted_.begin(); it != highlighted_.end(); ++it) {
		auto index = it->first;
		auto color = it->second;

		if (color == highlightColor) {
			level_->tileMap_[index] = preHighlight_[index];
			level_->updateTexture(index);

			preHighlight_.erase(index);
			indexes.push_back(index);
		}
	}

	for (std::size_t i = 0; i < indexes.size(); ++i) {
		highlighted_.erase(indexes[i]);
	}
}

tests::HighlighterTest::~HighlighterTest()
{ }
