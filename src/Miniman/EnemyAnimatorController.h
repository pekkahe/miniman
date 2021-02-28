#ifndef ENEMY_ANIMATOR_CONTROLLER_H_
#define ENEMY_ANIMATOR_CONTROLLER_H_

#include "AnimatorController.h"

class Enemy;
enum MoveDirection;

/// <summary>
/// Sprite sheet based enemy character animator.
/// </summary>
class EnemyAnimatorController : public AnimatorController
{
public:
	/// <summary>
	/// Defines the sprite sheet row indexes for the various animated states,
	/// e.g. enemy is moving up or enemy is frightened.
	/// </summary>
	enum SpriteRow : unsigned int
	{
		SPRITE_ROW_UP = 0,				
		SPRITE_ROW_RIGHT = 1,			
		SPRITE_ROW_DOWN = 2,			
		SPRITE_ROW_LEFT = 3,
		SPRITE_ROW_FRIGHTENED = 4,
		SPRITE_ROW_DEAD = 5
	};

	EnemyAnimatorController(Enemy& enemy, const sf::Time& animationSpeed);
	EnemyAnimatorController(const EnemyAnimatorController& other);
	virtual ~EnemyAnimatorController();
	EnemyAnimatorController* clone() const;

	/// <summary>
	/// Updates the enemy animator. Called once per frame.
	/// </summary>
	virtual void update();

	/// <summary>
	/// Animates the sprite sheet by switching to the next designated sprite.
	/// </summary>
	virtual void animate();
	
private:
	/// <summary>
	/// Returns the currently animated sprite sheet row.
	/// </summary>
	SpriteRow getActiveRow() const;

	/// <summary>
	/// Maps the given movement direction to a sprite sheet row which
	/// contains its animation.
	/// </summary>
	SpriteRow mapToRow(MoveDirection moveDirection) const;

	/// <summary>
	/// Updates the active sprite to the first sprite on the given row.
	/// </summary>
	void selectFirstSprite(SpriteRow row);

private:
	Enemy& enemy_;
	SpriteRow spriteRow_;
};

#endif