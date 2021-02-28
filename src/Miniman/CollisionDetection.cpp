#include "CollisionDetection.h"
#include "Locator.h"
#include "Player.h"
#include "EnemyManager.h"
#include "AudioManager.h"
#include "Global.h"

CollisionDetection::CollisionDetection()
{ }

void CollisionDetection::preDetect()
{
	oldPlayerPosition_ = Locator::player()->tile();
	
	auto enemies = Locator::enemyManager()->enemies();
	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		auto enemy = *it;
		oldEnemyPositions_[enemy] = enemy->tile();
	}
}

void CollisionDetection::detect()
{
	auto enemies = Locator::enemyManager()->enemies();
	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		auto enemy = *it;
		if (enemy->isDead())
			continue;

		if (doesPlayerCollideWithEnemy(enemy))
		{
			debug().info("Collision with enemy.", DebugMessage::GENERAL);

			if (enemy->isFrightened())
			{
				enemy->die(sf::seconds(2));
				
				Locator::audio()->playEnemyDiedSound(0.5);
			}
			else
			{
				#ifdef GOD_MODE
					return;
				#endif

				Locator::player()->die();

				if (Locator::player()->lives() > 0)
					Locator::audio()->playPlayerDiedSound(0.4f);
			}
		}
	}
}

bool CollisionDetection::doesPlayerCollideWithEnemy(Enemy* enemy)
{
	auto playerPosition = Locator::player()->tile();
	auto enemyPosition = enemy->tile();

	bool samePosition = playerPosition == enemyPosition;
	bool passedEachOther = playerPosition == oldEnemyPositions_[enemy] &&
						   enemyPosition == oldPlayerPosition_;

	return samePosition || passedEachOther;
}

CollisionDetection::~CollisionDetection()
{ }
