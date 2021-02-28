#ifndef DEBUGINPUT_H
#define DEBUGINPUT_H

#include <bitset>
#include <string>
#include <SFML/Graphics.hpp>

/// <summary>
/// Defines the type of the logged debug message.
/// </summary>
enum DebugMessage
{
	GENERAL,
	ERROR,
	WARNING,
	INITIALIZATION,
	LEVEL,
	AI_TRACKING,
	AI_MOVEMENT,
	AI_GENERAL,
	AI_PATHFINDING,
	AI_BEHAVIOUR,
	TRANSFORMATIONS
};

/// <summary>
/// Defines debugging related helpers.
/// </summary>
class DebugTools
{
public:
	DebugTools();
	~DebugTools();

	/// <summary>
	/// Adds the given message type into the list of loggable message types.
	/// </summary>
	void logMessages(DebugMessage messageType);

	/// <summary>
	/// Writes a message to the console if its message type is set loggable.
	/// </summary>
	void log(const std::string& message, DebugMessage messageType);

	/// <summary>
	/// Writes an info message to the console if its message type is set loggable.
	/// </summary>
	void info(const std::string& message, DebugMessage messageType);

	/// <summary>
	/// Writes an error message to the console.
	/// </summary>
	void error(const std::string& message);

	/// <summary>
	/// Writes a warning message to the console.
	/// </summary>
	void warning(const std::string& message);

	/// <summary>
	/// Handles debugging specific input for the given key code.
	/// </summary>
	void readInput(const sf::Keyboard::Key keyCode);

private:
	std::bitset<32> flags_;
	sf::Clock clock_;
};

/// <summary>
/// Returns the instance of the debugging tools helper class.
/// </summary>
DebugTools& debug();

#endif
