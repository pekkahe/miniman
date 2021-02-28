#include "AudioManager.h"
#include "Locator.h"
#include "ResourceManager.h"
#include "DebugTools.h"

AudioManager::AudioManager()
{ }

bool AudioManager::init()
{ 
	if (!audioBank_.loadSound(CLIP_EAT, Locator::resource()->getPath("eat.wav"))) {
		debug().error("Failed to load sound " + Locator::resource()->getPath("eat.wav"));
		return false;
	}
	if (!audioBank_.loadSound(CLIP_ENERGIZER, Locator::resource()->getPath("energizer.wav"))) {
		debug().error("Failed to load sound " + Locator::resource()->getPath("energizer.wav"));
		return false;
	}
	if (!audioBank_.loadSound(CLIP_MENU_SELECT, Locator::resource()->getPath("menu_select.wav"))) {
		debug().error("Failed to load sound " + Locator::resource()->getPath("menu_select.wav"));
		return false;
	}
	if (!audioBank_.loadSound(CLIP_MENU_TRIGGER, Locator::resource()->getPath("menu_trigger.wav"))) {
		debug().error("Failed to load sound " + Locator::resource()->getPath("menu_trigger.wav"));
		return false;
	}
	if (!audioBank_.loadSound(CLIP_MENU_START, Locator::resource()->getPath("menu_start.wav"))) {
		debug().error("Failed to load sound " + Locator::resource()->getPath("menu_start.wav"));
		return false;
	}
	if (!audioBank_.loadSound(CLIP_PLAYER_DIED, Locator::resource()->getPath("player_died.wav"))) {
		debug().error("Failed to load sound " + Locator::resource()->getPath("player_died.wav"));
		return false;
	}
	if (!audioBank_.loadSound(CLIP_GAME_OVER, Locator::resource()->getPath("game_over.wav"))) {
		debug().error("Failed to load sound " + Locator::resource()->getPath("game_over.wav"));
		return false;
	}
	if (!audioBank_.loadSound(CLIP_LEVEL_COMPLETE, Locator::resource()->getPath("level_complete.wav"))) {
		debug().error("Failed to load sound " + Locator::resource()->getPath("level_complete.wav"));
		return false;
	}
	if (!audioBank_.loadSound(CLIP_ENEMY_DIED, Locator::resource()->getPath("enemy_died.wav"))) {
		debug().error("Failed to load sound " + Locator::resource()->getPath("enemy_died.wav"));
		return false;
	}

	return true;
}

void AudioManager::playMenuStartSound(float volume)
{
	playSound(audioBank_.getSound(CLIP_MENU_START), volume);
}

void AudioManager::playLevelCompleteSound(float volume)
{
	playSound(audioBank_.getSound(CLIP_LEVEL_COMPLETE), volume);
}

void AudioManager::playEatSound(float volume)
{
	playSound(audioBank_.getSound(CLIP_EAT), volume);
}

void AudioManager::playEnemyDiedSound(float volume)
{
	playSound(audioBank_.getSound(CLIP_ENEMY_DIED), volume);
}

void AudioManager::playEnergizerSound(float volume)
{
	playSound(audioBank_.getSound(CLIP_ENERGIZER), volume);
}

void AudioManager::playMenuSelectSound(float volume)
{
	playSound(audioBank_.getSound(CLIP_MENU_SELECT), volume);
}

void AudioManager::playMenuTriggerSound(float volume)
{
	playSound(audioBank_.getSound(CLIP_MENU_TRIGGER), volume);
}

void AudioManager::playPlayerDiedSound(float volume)
{
	playSound(audioBank_.getSound(CLIP_PLAYER_DIED), volume);
}

void AudioManager::playGameOverSound(float volume)
{
	playSound(audioBank_.getSound(CLIP_GAME_OVER), volume);
}

void AudioManager::playSound(sf::Sound& sound, float volume)
{ 
	if (isMuted_)
		return;

	sound.setVolume(getVolumeScale(volume));
	sound.setLoop(false);
	sound.play();
}

void AudioManager::startLoop(sf::Sound& sound, float volume)
{
	if (isMuted_)
		return;

	sound.setVolume(getVolumeScale(volume));
	sound.setLoop(true);
	sound.play();
}

void AudioManager::endLoop(sf::Sound& sound)
{
	sound.setLoop(false);
	sound.stop();
}

float AudioManager::getVolumeScale(float volume)
{
	if (volume < 0)
		volume = 0;

	if (volume > 1.0)
		volume = 1.0;

	return volume * 100.0f;
}

AudioManager::~AudioManager()
{ }
