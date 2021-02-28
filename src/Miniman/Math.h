#ifndef MATH_H_
#define MATH_H_

#include <SFML\Graphics.hpp>

namespace math
{
	/// <summary>
	/// Returns the dot product for the two vectors.
	/// </summary>
	float dot(const sf::Vector2f& a, const sf::Vector2f& b);

	/// <summary>
	/// Returns the dot product for the two vectors.
	/// </summary>
	float dot(const sf::Vector3f& a, const sf::Vector3f& b);

	/// <summary>
	/// Returns the cross product for the two vectors.
	/// </summary>
	sf::Vector3f cross(const sf::Vector3f& a, const sf::Vector3f& b);

	/// <summary>
	/// Returns the normalized vector of the given vector.
	/// </summary>
	sf::Vector2f normalize(const sf::Vector2f& vector);

	/// <summary>
	/// Returns the magnitude of the given vector.
	/// </summary>
	float magnitude(const sf::Vector2f& v);

	/// <summary>
	/// Returns the magnitude of the given vector.
	/// </summary>
	float magnitude(const sf::Vector2u& v);

	/// <summary>
	/// Returns the angle in degrees between vectors a and b.
	/// </summary>
	float angle(const sf::Vector2f& a, const sf::Vector2f& b);

	/// <summary>
	/// Transforms the given degrees into radians.
	/// </summary>
	float toRadians(float degrees);

	/// <summary>
	/// Transforms the given radians into degrees.
	/// </summary>
	float toDegrees(float radians);

	/// <summary>
	/// Returns the given floating point number rounded up to the nearest integer.
	/// </summary>
	int round(float value);

	/// <summary>
	/// Returns -1 when a is to the left of b, 1 when to the right, and 0 when ahead or behind.
	/// </summary>
	int orientation(const sf::Vector2f& a, const sf::Vector2f& b);

	/// <summary>
	/// Returns the sign of the given number.
	/// </summary>
	/// <param name="allowZeroSignum">If false, returns 1 for zero signums.</param>
	template <typename T> int sign(T val, bool allowZeroSignum = false) {
		int signum = (T(0) < val) - (val < T(0));

		if (signum == 0) {
			return allowZeroSignum ? signum : 1;
		}

		return signum;
	}
}

#endif
