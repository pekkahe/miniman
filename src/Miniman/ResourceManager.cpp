#include "ResourceManager.h"
#include "Global.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "Util.h"
#include "DebugTools.h"

ResourceManager::ResourceManager()
{ }

bool ResourceManager::init()
{
	// Initialize file system via the PhysicsFS library
	PHYSFS_init(NULL);

	// Setup archives that will be read on
	if (!PHYSFS_mount("assets/gfx.pac", "/gfx/", 1)) {
		debug().error("PHYSFS_mount: assets/gfx.pac " + std::string(PHYSFS_getLastError()));
		return false;
	}

	if (!PHYSFS_mount("assets/sfx.pac", "/sfx/", 1)) {
		debug().error("PHYSFS_mount: assets/sfx.pac " + std::string(PHYSFS_getLastError()));
		return false;
	}

	if (!PHYSFS_mount("assets/misc.pac", "/misc/", 1)) {
		debug().error("PHYSFS_mount: assets/misc.pac " + std::string(PHYSFS_getLastError()));
		return false;
	}

	return true;
}

std::string ResourceManager::getPath(const std::string& fileName)
{
	auto extension = getFileExtension(fileName);

	// For levels and high score file, we're not using PhysicsFS, since we 
	// need line by line reading (level) and writing back to the file (high score).
	// PhysicsFS doesn't support this out of the box, so we're using regular ifstream
	// and need to return the relative file system path.
	if (extension == "lvl" || extension == "sc")
		return "assets/" + fileName;

	if (extension == "wav")
		return "/sfx/" + fileName;

	if (extension == "png")
		return "/gfx/" + fileName;
	
	return "/misc/" + fileName;
}

std::string ResourceManager::getFileExtension(const std::string& fileName)
{
	return fileName.substr(fileName.find_last_of(".") + 1);
}

bool ResourceManager::loadFont(const std::string& virtualFilePath, sf::Font* font)
{
	PHYSFS_File* file = PHYSFS_openRead(virtualFilePath.c_str());
	if (file == NULL) {
		debug().error(std::string(PHYSFS_getLastError()));
		return false;
	}

	std::size_t fileSize;
	void* fileBuffer = readFile(file, fileSize);

	if (!font->loadFromMemory(fileBuffer, static_cast<std::size_t>(fileSize)))
		return false;

	PHYSFS_close(file);
	
	return true;
}

bool ResourceManager::loadTexture(const std::string& virtualFilePath, sf::Texture* texture)
{
	PHYSFS_File* file = PHYSFS_openRead(virtualFilePath.c_str());
	if (file == NULL) {
		debug().error(std::string(PHYSFS_getLastError()));
		return false;
	}

	std::size_t fileSize;
	char* fileBuffer = readFile(file, fileSize);

	if (!texture->loadFromMemory(fileBuffer, static_cast<std::size_t>(fileSize)))
		return false;

	PHYSFS_close(file);

	return true;
}

bool ResourceManager::loadSound(const std::string& virtualFilePath, sf::SoundBuffer* soundBuffer)
{
	PHYSFS_File* file = PHYSFS_openRead(virtualFilePath.c_str());
	if (file == NULL) {
		debug().error(std::string(PHYSFS_getLastError()));
		return false;
	}

	std::size_t fileSize;
	char* fileBuffer = readFile(file, fileSize);

	if (!soundBuffer->loadFromMemory(fileBuffer, static_cast<std::size_t>(fileSize)))
		return false;

	PHYSFS_close(file);

	return true;
}

bool ResourceManager::saveHighScore(const std::string& fileName, unsigned int score)
{
	std::fstream file;
	file.open(fileName, std::fstream::out);

	if (!file.is_open())
		return false;

	file << score;
	file.close();

	return true;
}

unsigned int ResourceManager::loadHighScore(const std::string& fileName)
{
	std::fstream file;
	file.open(fileName, std::fstream::in);

	// First try opening the file if it exists,
	// then create it if it doesn't
	if (!file.is_open())
	{
		file.open(fileName, std::fstream::out | std::fstream::trunc);

		// If we still can't open the file, return zero score
		if (!file.is_open())
			return 0;
	}

	// If file is empty, use zero high score
	if (file.eof()) {
		return 0;
	}

	// Read first (and only) line to get high score
	std::string line;
	std::getline(file, line);

	// Do some validation for the score, since player could
	// have modified the text file manually...
	long int score = std::strtol(line.c_str(), NULL, 10);
	if (score < 0 || score > UINT32_MAX) {
		score = 0;
	}

	return score;
}

int* ResourceManager::loadTileMap(const std::string& fileName, int& tileMapWidth, int& tileMapHeight)
{
	std::ifstream file;
	file.open(fileName, std::fstream::in);

	if (!file.is_open())
		return false;

	// Read first line to get tile map width and height
	std::string line;
	std::getline(file, line);

	std::vector<std::string> params = util::split(line, ';');
	int width = std::stoi(params[0]);
	int height = std::stoi(params[1]);

	// Instantiate new tile map based on width and height
	int* tileMap = new int[width * height];
	int j = 0;

	// Read rest of the file line by line for each tile number
	while (!file.eof())
	{
		std::getline(file, line);
		std::vector<std::string> tileNumbers = util::split(line, ',');

		// Populate tile map for this line
		for (std::size_t i = 0; i < tileNumbers.size(); i++) {
			int tileNumber = std::stoi(tileNumbers[i]);

			tileMap[j] = tileNumber;
			j++;
		}
	}

	file.close();

	tileMapWidth = width;
	tileMapHeight = height;

	return tileMap;
}

char* ResourceManager::readFile(PHYSFS_File* fileHandle, std::size_t& fileSize)
{
	std::size_t fileLenght = static_cast<std::size_t>(PHYSFS_fileLength(fileHandle));
	fileSize = fileLenght;

	// Use 1-byte char buffer
	char* fileBuffer = new char[fileLenght];

	// Read file to char buffer
	if (!PHYSFS_read(fileHandle, fileBuffer, 1, fileLenght)) {
		debug().error(std::string(PHYSFS_getLastError()));
		return nullptr;
	}

	return fileBuffer;
}

ResourceManager::~ResourceManager()
{ 
	PHYSFS_deinit();
}
