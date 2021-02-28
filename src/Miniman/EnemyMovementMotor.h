#ifndef ENEMYMOVEMENTMOTOR_H_
#define ENEMYMOVEMENTMOTOR_H_

#include <stack>
#include <vector>
#include <array>
#include <functional>
#include <mutex>
#include "MovementMotor.h"
#include "AStar/Path.h"

class AI;

/// <summary>
/// Handles the enemy character movement and position.
/// </summary>
class EnemyMovementMotor : public MovementMotor
{
public:
	EnemyMovementMotor(AI& ai);
	EnemyMovementMotor(const EnemyMovementMotor& other);
	~EnemyMovementMotor();
	EnemyMovementMotor* clone() const;

	/// <summary>
	/// Updates movement behaviour. Called once per frame.
	/// </summary>
	virtual void update();

	/// <summary>
	/// Returns true if currently following a path.
	/// </summary>
	bool isFollowingPath() const;

	/// <summary>
	/// Returns true if tracking is currently enabled.
	/// </summary>
	bool isTracking() const { return isTracking_; }

	/// <summary>
	/// Sets a path to follow.
	/// </summary>
	void setPath(astar::Path* path);

	/// <summary>
	/// Enables tracking by storing visited tiles.
	/// </summary>
	void startTracking();

	/// <summary>
	/// Expands movement by allowing movement to tiles of the given type.
	/// </summary>
	void allowMovement(Tile tile);

	/// <summary>
	/// Expands movement by allowing movement to the given direction.
	/// </summary>
	void allowMovement(MoveDirection direction);

	/// <summary>
	/// Restricts movement by denying movement to tiles of the given type.
	/// </summary>
	void restrictMovement(Tile tile);

	/// <summary>
	/// Restricts movement by denying movement to the given direction.
	/// </summary>
	void restrictMovement(MoveDirection direction);

	/// <summary>
	/// Calls the given function when the end of the path is reached.
	/// </summary>
	void onPathEnd(std::function<void()> function);
	
	/// <summary>
	/// Calls the given function when a tile of the given type is reached.
	/// </summary>
	void onTile(Tile tile, std::function<void()> function);

private:
	virtual MoveDirection getNextDirection();

	MoveDirection getDirectionTowards(const sf::Vector2u& target);
	MoveDirection getDirectionTowards(const sf::Vector2u& source, const sf::Vector2u& target);
	
	/// <summary>
	/// Returns a direction which simulates enemy movement when not following a path.
	/// </summary>
	MoveDirection chooseNextDirection();

	/// <summary>
	/// Returns the opposite direction for the given direction.
	/// </summary>
	MoveDirection getOppositeDirection(MoveDirection direction);

	/// <summary>
	/// Returns true if the enemy can choose the given direction as his next direction.
	/// </summary>
	bool canChooseDirection(MoveDirection direction);

	/// <summary>
	/// Returns a movement direction which follows the tracks.
	/// </summary>
	MoveDirection followTracks();

	/// <summary>
	/// Stores the current tile position as a visited "track".
	/// </summary>
	void storeTracks();

	/// <summary>
	/// Truncates the given path by comparing it with the stored tracks and removing shared
	/// tile positions. Also removes these positions from the tracks, so that the tracks end
	/// where the path starts.
	/// </summary>
	void truncatePath(astar::Path* path);

	/// <summary>
	/// Returns the amount of shared tile positions between the tracks and the given path.
	/// </summary>
	int compareTracks(astar::Path* path);

private:
	AI& ai_;

	astar::Path* path_;
	astar::Path tracks_;
	bool isTracking_;
	bool wasTracking_;

	std::function<void()> onPathReached_;
	std::function<void()> onTile_;
	Tile onTileTarged_;

	std::vector<MoveDirection> moveRestrictions_;
	std::array<MoveDirection, 4> moveDirectionPriority_;
};

#endif