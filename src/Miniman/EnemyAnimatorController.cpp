#include "EnemyAnimatorController.h"
#include "Global.h"
#include "Locator.h"
#include "Enemy.h"
#include "AI.h"
#include "EnemyMovementMotor.h"

EnemyAnimatorController::EnemyAnimatorController(Enemy& enemy, const sf::Time& animationSpeed) :
	AnimatorController(&enemy.motor(), animationSpeed),
	enemy_(enemy),
	spriteRow_(SPRITE_ROW_UP)
{ }

EnemyAnimatorController::EnemyAnimatorController(const EnemyAnimatorController& other) :
	AnimatorController(other),
	enemy_(other.enemy_),
	spriteRow_(other.spriteRow_)
{ }

EnemyAnimatorController* EnemyAnimatorController::clone() const
{ 
	return new EnemyAnimatorController(*this);
}

void EnemyAnimatorController::update()
{
	auto previousRow(spriteRow_);
	spriteRow_ = getActiveRow();

	if (previousRow != spriteRow_) {
		selectFirstSprite(spriteRow_);
	}

	AnimatorController::update();
}

void EnemyAnimatorController::animate()
{ 
	if (!enemy_.isDead()) {
		AnimatorController::pingPongSprite((unsigned int) spriteRow_);
	}
}

void EnemyAnimatorController::selectFirstSprite(EnemyAnimatorController::SpriteRow row)
{
	// Add zero just to indicate we've taken 
	// the y factor into consideration
	unsigned int index = (unsigned int) row * spriteCount().x + 0;

	setSprite(index);
}

EnemyAnimatorController::SpriteRow EnemyAnimatorController::getActiveRow() const
{
	if (enemy_.isDead()) {
		return SPRITE_ROW_DEAD;
	}
	if (enemy_.isFrightened()) {
		return SPRITE_ROW_FRIGHTENED;
	}
	return mapToRow(enemy_.motor().moveDirection());
}

EnemyAnimatorController::SpriteRow EnemyAnimatorController::mapToRow(MoveDirection moveDirection) const
{
	switch (moveDirection)
	{
		case MOVE_UP:
			return SPRITE_ROW_UP;
		case MOVE_DOWN:
			return SPRITE_ROW_DOWN;
		case MOVE_LEFT:
			return SPRITE_ROW_LEFT;
		case MOVE_RIGHT:
			return SPRITE_ROW_RIGHT;
		default:
			return SPRITE_ROW_UP;
	}
}

EnemyAnimatorController::~EnemyAnimatorController()
{ }
