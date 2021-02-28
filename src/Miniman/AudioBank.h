#ifndef AUDIOBANK_H_
#define AUDIOBANK_H_

#include <SFML\Audio.hpp>
#include <map>

enum AudioClip
{
	CLIP_EAT,
	CLIP_MENU_SELECT,
	CLIP_MENU_TRIGGER,
	CLIP_PLAYER_DIED,
	CLIP_GAME_OVER,
	CLIP_LEVEL_COMPLETE,
	CLIP_ENERGIZER,
	CLIP_ENEMY_DIED,
	CLIP_MENU_START
};

/// <summary>
/// Defines a storage for the game's audio clips.
/// </summary>
class AudioBank
{
public:
	AudioBank();
	~AudioBank();

	/// <summary>
	/// Loads and stores a sound clip accessible by the given enumeration
	/// and specified by the given file name. Returns true if successful.
	/// </summary>
	bool loadSound(AudioClip clip, const std::string& fileName);

	/// <summary>
	/// Returns the loaded sound clip for the given enumeration.
	/// </summary>
	sf::Sound& getSound(AudioClip clip);

private:
	std::map<AudioClip, sf::SoundBuffer> buffers_;
	std::map<AudioClip, sf::Sound> sounds_;
};

#endif
