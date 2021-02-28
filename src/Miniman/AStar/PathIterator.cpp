#include "PathIterator.h"
#include "Path.h"

using namespace astar;

PathIterator::PathIterator(const Path* path) :
	path_(path),
	index_(0)
{ }

PathIterator::PathIterator(const PathIterator& other) :
	path_(other.path_),
	index_(other.index_)
{ }

sf::Vector2u PathIterator::item()
{
	return path_->positions_[index_];
}

PathIterator& PathIterator::operator++()
{
	index_++;
	return *this;
}

PathIterator PathIterator::operator++(int)
{
	PathIterator temp(*this);
	operator++();
	return temp;
}

inline bool PathIterator::operator==(const PathIterator& other)
{
	return this->index_ == other.index_;
}

inline bool PathIterator::operator!=(const PathIterator& other)
{
	return !operator==(other);
}

inline bool PathIterator::operator<(const PathIterator& other)
{
	return this->index_ < other.index_;
}

inline bool PathIterator::operator>(const PathIterator& other)
{
	return this->index_ > other.index_;
}

inline bool PathIterator::operator<=(const PathIterator& other)
{
	return !operator>(other);
}

inline bool PathIterator::operator>=(const PathIterator& other)
{
	return !operator<(other);
}

PathIterator::~PathIterator()
{ }
