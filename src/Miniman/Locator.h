#ifndef LOCATOR_H_
#define LOCATOR_H_

class Game;
class Renderer;
class GUI;
class Level;
class Player;
class Input;
class EnemyManager;
class AudioManager;
class GameSettings;
class ResourceManager;

/// <summary>
/// Service locator for the game components.
/// </summary>
class Locator
{
public:
	static Game* game() { return game_; }
	static Renderer* renderer() { return renderer_; }
	static GUI* gui() { return gui_; }
	static Level* level() { return level_; }
	static Player* player() { return player_; }
	static Input* input() { return input_; }
	static EnemyManager* enemyManager() { return enemyManager_; }
	static AudioManager* audio() { return audioManager_; }
	static GameSettings* settings() { return settings_; }
	static ResourceManager* resource() { return resource_; }

	static void provide(Game* game) { game_ = game; }
	static void provide(Renderer* renderer) { renderer_ = renderer; }
	static void provide(GUI* gui) { gui_ = gui; }
	static void provide(Level* level) { level_ = level; }
	static void provide(Player* player) { player_ = player; }
	static void provide(Input* input) { input_ = input; }
	static void provide(EnemyManager* manager) { enemyManager_ = manager; }
	static void provide(AudioManager* manager) { audioManager_ = manager; }
	static void provide(GameSettings* settings) { settings_ = settings; }
	static void provide(ResourceManager* resource) { resource_ = resource; }

private:
	static Game* game_;
	static Renderer* renderer_;
	static GUI* gui_;
	static Level* level_;
	static Player* player_;
	static Input* input_;
	static EnemyManager* enemyManager_;
	static AudioManager* audioManager_;
	static GameSettings* settings_;
	static ResourceManager* resource_;
};

#endif