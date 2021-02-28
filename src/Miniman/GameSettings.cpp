#include <sstream>
#include "GameSettings.h"
#include "DebugTools.h"

const float DEFAULT_PLAYER_SPEED = 0.15f;
const float LEVEL_SPEED_INCREASE = 0.01f;
const float MAX_PLAYER_SPEED = 0.25f;
const float DEFAULT_FRIGHT_TIME = 6.0f;
const float LEVEL_FRIGHT_DECREASE = 1.0f;
const float MIN_FRIGHT_TIME = 1.0f;

GameSettings::GameSettings()
{ 
	playerSpeed_ = DEFAULT_PLAYER_SPEED;
	frightTime_ = sf::seconds(DEFAULT_FRIGHT_TIME);
}

void GameSettings::increaseDifficulty(int level)
{
	playerSpeed_ += (level - 1) * LEVEL_SPEED_INCREASE;
	frightTime_ -= sf::seconds((level - 1) * LEVEL_FRIGHT_DECREASE);

	if (playerSpeed_ > MAX_PLAYER_SPEED)
		playerSpeed_ = MAX_PLAYER_SPEED;

	if (frightTime_ < sf::seconds(MIN_FRIGHT_TIME))
		frightTime_ = sf::seconds(MIN_FRIGHT_TIME);

	#ifdef _DEBUG
		std::stringstream message;
		message << "Difficulty increased for level " << std::to_string(level) << std::endl;
		message << "New player speed: " << std::to_string(playerSpeed_) << std::endl;
		message << "New fright time (s): " << std::to_string(frightTime_.asSeconds());

		debug().info(message.str(), DebugMessage::LEVEL);
	#endif
}

float GameSettings::playerSpeed() const 
{
	return playerSpeed_;
}

float GameSettings::playerEatSpeed() const
{
	return playerSpeed_ * 0.7f; 
}

float GameSettings::enemySpeed() const 
{ 
	return playerSpeed_ * 0.85f; 
}

float GameSettings::enemyFrightenedSpeed() const 
{
	return playerSpeed_ * 0.4f; 
}

float GameSettings::enemyDeadSpeed() const
{
	return playerSpeed_ * 1.4f;
}

sf::Time GameSettings::frightTime() const 
{
	return frightTime_; 
}

GameSettings::~GameSettings()
{ }
