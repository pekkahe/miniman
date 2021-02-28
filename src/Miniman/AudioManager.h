#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#include "AudioBank.h"

/// <summary>
/// Game component responsible for playing audio.
/// </summary>
class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	/// <summary>
	/// Initializes the game audio and returns true if successful.
	/// </summary>
	bool init();

	/// <summary>
	/// Mutes or unmutes the game audio.
	/// </summary>
	void mute(bool muted = true) { isMuted_ = muted; }

	/// <summary>
	/// Returns true if the game audio is muted.
	/// </summary>
	bool isMuted() const { return isMuted_; }

	void playEatSound(float volume = 1.0);
	void playEnemyDiedSound(float volume = 1.0);
	void playEnergizerSound(float volume = 1.0);
	void playMenuSelectSound(float volume = 1.0);
	void playMenuTriggerSound(float volume = 1.0);
	void playPlayerDiedSound(float volume = 1.0);
	void playGameOverSound(float volume = 1.0);
	void playLevelCompleteSound(float volume = 1.0);
	void playMenuStartSound(float volume = 1.0);

private:
	void playSound(sf::Sound& sound, float volume);
	void startLoop(sf::Sound& sound, float volume);
	void endLoop(sf::Sound& sound);
	inline float getVolumeScale(float volume);

private:
	AudioBank audioBank_;
	bool isMuted_;
};

#endif
