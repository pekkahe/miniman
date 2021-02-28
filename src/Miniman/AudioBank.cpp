#include "AudioBank.h"
#include <iostream>
#include "Locator.h"
#include "ResourceManager.h"

AudioBank::AudioBank()
{ 
	buffers_[CLIP_EAT] = sf::SoundBuffer();
	buffers_[CLIP_MENU_SELECT] = sf::SoundBuffer();
	buffers_[CLIP_MENU_TRIGGER] = sf::SoundBuffer();
	buffers_[CLIP_PLAYER_DIED] = sf::SoundBuffer();
	buffers_[CLIP_GAME_OVER] = sf::SoundBuffer();
	buffers_[CLIP_LEVEL_COMPLETE] = sf::SoundBuffer();
	buffers_[CLIP_ENERGIZER] = sf::SoundBuffer();
	buffers_[CLIP_ENEMY_DIED] = sf::SoundBuffer();
	buffers_[CLIP_MENU_START] = sf::SoundBuffer();

	sounds_[CLIP_EAT] = sf::Sound();
	sounds_[CLIP_MENU_SELECT] = sf::Sound();
	sounds_[CLIP_MENU_TRIGGER] = sf::Sound();
	sounds_[CLIP_PLAYER_DIED] = sf::Sound();
	sounds_[CLIP_GAME_OVER] = sf::Sound();
	sounds_[CLIP_LEVEL_COMPLETE] = sf::Sound();
	sounds_[CLIP_ENERGIZER] = sf::Sound();
	sounds_[CLIP_ENEMY_DIED] = sf::Sound();
	sounds_[CLIP_MENU_START] = sf::Sound();
}

bool AudioBank::loadSound(AudioClip clip, const std::string& fileName)
{
	if (!Locator::resource()->loadSound(fileName, &buffers_[clip])) {
		return false;
	}

	sounds_[clip].setBuffer(buffers_[clip]);

	return true;
}

sf::Sound& AudioBank::getSound(AudioClip clip)
{
	return sounds_[clip];
}

AudioBank::~AudioBank()
{ }