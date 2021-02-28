#ifndef HELPERS_H_
#define HELPERS_H_

#include <deque>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <SFML/Graphics.hpp>

namespace util
{
	/// <summary>
	/// Returns the string representation of the given position.
	/// </summary>
	std::string toString(const sf::Vector2u& position);
	
	/// <summary>
	/// Returns the string representation of the given position.
	/// </summary>
	std::string toString(const sf::Vector2f& position);

	/// <summary>
	/// Returns a random integer between the given boundaries.
	/// </summary>
	int randomize(int min, int max);

	/// <summary>
	/// Returns the distance between the two vectors.
	/// </summary>
	float distance(const sf::Vector2u& from, const sf::Vector2u& to);

	/// <summary>
	/// Returns the distance between the two vectors.
	/// </summary>
	float distance(const sf::Vector2f& from, const sf::Vector2f& to);

	/// <summary>
	/// Splits the given string into multiple strings using the specified delimiter.
	/// </summary>
	std::vector<std::string> split(const std::string& source, char delimiter);

	/// <summary>
	/// Splits the given string into multiple strings using the specified delimiter,
	/// inserting the elements into the given collection.
	/// </summary>
	void split(const std::string& source, char delimiter, std::vector<std::string> &elements);
	
	/// <summary>
	/// Returns true if the vector contains the given item, false otherwise.
	/// </summary>
	template<typename T>
	bool contains(const std::vector<T>& list, const T& item)
	{
		return std::find(list.begin(), list.end(), item) != list.end();
	}

	/// <summary>
	/// Returns true if the stack contains the given item, false otherwise.
	/// </summary>
	template<typename T>
	bool contains(const std::stack<T>& list, const T& item)
	{
		std::stack<T> copy(list);
		while (!copy.empty()) {
			if (copy.top() == item) {
				return true;
			}

			copy.pop();
		}

		return false;
	}

	/// <summary>
	/// Returns true if the deque contains the given item, false otherwise.
	/// </summary>
	template<typename T>
	bool contains(const std::deque<T>& list, const T& item)
	{
		return std::find(list.begin(), list.end(), item) != list.end();
	}
}

#endif