#include "Math.h"
#include <cmath>

#define PI 3.14159265f

float math::dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float math::dot(const sf::Vector3f& a, const sf::Vector3f& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

sf::Vector3f math::cross(const sf::Vector3f& a, const sf::Vector3f& b)
{
	sf::Vector3f cross;
	cross.x = (a.y * b.z) - (a.z * b.y);
	cross.y = (a.z * b.x) - (a.x * b.z);
	cross.z = (a.x * b.y) - (a.y * b.x);
	return cross;
}

sf::Vector2f math::normalize(const sf::Vector2f& vector)
{
	float m = math::magnitude(vector);

	return sf::Vector2f(vector.x / m, vector.y / m);
}

float math::magnitude(const sf::Vector2f& v)
{
	return std::sqrt(math::dot(v, v));
}

float math::magnitude(const sf::Vector2u& v)
{
	return math::magnitude(
		sf::Vector2f(static_cast<float>(v.x), static_cast<float>(v.y)));
}

float math::angle(const sf::Vector2f& a, const sf::Vector2f& b)
{
	float dot = math::dot(a, b);
	float ma = math::magnitude(a);
	float mb = math::magnitude(b);

	float cosa = dot / ma * mb;
	float acos = std::acos(cosa);

	return math::toDegrees(acos);
}

float math::toRadians(float degrees)
{
	return degrees * PI / 180.0f;
}

float math::toDegrees(float radians)
{
	return radians * 180.0f / PI;
}

int math::orientation(const sf::Vector2f& a, const sf::Vector2f& b)
{
	auto cross = math::cross(sf::Vector3f(a.x, a.y, 0), sf::Vector3f(b.x, b.y, 0));
	float dot = math::dot(cross, sf::Vector3f(0, 0, 1));

	if (dot > 0.0) {
        return 1;
    } else if (dot < 0.0) {
        return -1;
    } else {
        return 0;
    }
}

int math::round(float value)
{
	return static_cast<int>(std::floor(value + 0.5f));
}
