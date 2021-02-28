#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include "SFML\System.hpp"
#include <map>

class Enemy;

/// <summary>
/// Game component responsible for collision detection between the player and enemies.
/// </summary>
class CollisionDetection
{
public:
	CollisionDetection();
	~CollisionDetection();

	/// <summary>
	/// Setup collision detection for the upcoming game behaviour update.
	/// </summary>
	/// <remarks>
	/// Should be called before the frame's game behaviour update and <c>detect</c>.
	/// </remarks>
	void preDetect();

	/// <summary>
	/// Detect collision between the player and enemies.
	/// </summary>
	/// <remarks>
	/// Should be called after the frame's game behaviour update and <c>preDetect</c>.
	/// </remarks>
	void detect();

private:
	bool doesPlayerCollideWithEnemy(Enemy* enemy);

private:
	sf::Vector2u oldPlayerPosition_;
	std::map<Enemy*, sf::Vector2u> oldEnemyPositions_;
};

#endif
