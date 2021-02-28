#include "Path.h"
#include "Node.h"
#include "PathIterator.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <algorithm>

using namespace astar;

Path::Path() :
	index_(0)
{ }

Path::Path(const Path& other) :
	index_(other.index_),
	positions_(other.positions_)
{ }

Path::Path(const std::deque<Node*>& nodes) :
	index_(0)
{
	for (auto it = nodes.begin(); it != nodes.end(); ++it) {
		positions_.push_back((*it)->position());
	}
}

sf::Vector2u Path::current() const
{
	assert(!positions_.empty());

	return positions_[index_];
}

sf::Vector2u Path::first() const
{
	return positions_.front();
}

sf::Vector2u Path::last() const
{
	return positions_.back();
}

sf::Vector2u Path::at(size_t index) const
{
	assert(index < positions_.size());

	return positions_[index];
}

int Path::count() const
{
	return positions_.size();
}

void Path::clear()
{
	positions_.clear();
	index_ = 0;
}

void Path::addToBeginning(const sf::Vector2u& position)
{
	positions_.push_front(position);
}

void Path::addToEnd(const sf::Vector2u& position)
{
	positions_.push_back(position);
}

void Path::removeFromBeginning(int count)
{
	for (int i = 0; i < count; ++i) {
		removeFromBeginning();
	}
}

void Path::removeFromBeginning()
{
	increment();
}

void Path::removeFromEnd(int count)
{
	for (int i = 0; i < count; ++i) {
		removeFromEnd();
	}
}

void Path::removeFromEnd()
{
	positions_.pop_back();
}

bool Path::increment() 
{
	if (index_ < positions_.size() - 1) {
		index_++;
		return true;
	}
	return false;
}

bool Path::endReached() const 
{
	return index_ >= positions_.size() - 1;
}

PathIterator Path::begin() const
{
	return iterator();
}

PathIterator Path::end() const
{
	auto it = iterator();
	for (std::size_t i = 0; i < positions_.size() - 1; i++) {
		it++;
	}
	return it;
}

PathIterator Path::iterator() const
{
	return PathIterator(this);
}