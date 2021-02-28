#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include "physfs.h"
#include <string>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>

/// <summary>
/// Game component responsible for managing external file resources.
/// </summary>
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	/// <summary>
	/// Initializes the external game resources and returns true if successful.
	/// </summary>
	bool init();

	/// <summary>
	/// Loads a font file from the virtual file system to the specified sf::Font.
	/// Returns true if operation was successful, false otherwise.
	/// </summary>
	bool loadFont(const std::string& virtualFilePath, sf::Font* font);

	/// <summary>
	/// Loads an image file from the virtual file system to the specified sf::Texture.
	/// Returns true if operation was successful, false otherwise.
	/// </summary>
	bool loadTexture(const std::string& virtualFilePath, sf::Texture* texture);

	/// <summary>
	/// Loads a sound file from the virtual file system to the specified sf::SoundBuffer.
	/// Returns true if operation was successful, false otherwise.
	/// </summary>
	bool loadSound(const std::string& virtualFilePath, sf::SoundBuffer* soundBuffer);

	/// <summary>
	/// Reads a level tile map from the specified file and returns a pointer to the
	/// allocated tile array. Populates the passed tile map with and height parameters.
	/// </summary>
	int* loadTileMap(const std::string& fileName, int& tileMapWidth, int& tileMapHeight);

	/// <summary>
	/// Reads the high score from the given file, or returns 0 
	/// if the score doesn't exist yet or could not be read.
	/// </summary>
	unsigned int loadHighScore(const std::string& fileName);

	/// <summary>
	/// Stores the high score to the specified file. Returns true or false
	/// whether the operation was successful or not.
	/// </summary>
	bool saveHighScore(const std::string& fileName, unsigned int score);

	/// <summary>
	/// Returns the virtual path to the specified file.
	/// </summary>
	std::string getPath(const std::string& fileName);

private:
	char* readFile(PHYSFS_File* fileHandle, std::size_t& fileSize);

	std::string getFileExtension(const std::string& fileName);
};

#endif
