#pragma once
#define SAVE_FILE_PATH "saveFile.txt"
#include <string>

// opens file, returns int stored at line from file, or default value if can't
int readIntFromFile(const std::string& filePath, int lineNumber, int defaultValue);

// opens file, returns bool stored at line from file, or default value if can't
bool readBoolFromFile(const std::string& filePath, int lineNumber, bool defaultValue);
const std::string BoolToString(bool b);

enum class SAVEFILE_LINES {
	PLAYER_MAX_HEALTH,
	PLAYER_CURRENT_HEALTH,
	HEALTH_FLASK_USES,
	HEART_POWER_UP,
	IS_CHICKEN_DEAD,
	START_FROM_CHECKPOINT,
	SAVED_THIS_INSTANCE,
	SAVEFILE_LINES
};
constexpr int line_count = static_cast<int>(SAVEFILE_LINES::SAVEFILE_LINES);
