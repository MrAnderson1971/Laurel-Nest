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
	HEART_POWER_UP_0,
	HEART_POWER_UP_1,
	SWORD_POWER_UP_0,
	IS_CHICKEN_DEAD,
	START_FROM_CHECKPOINT,
	SAVED_THIS_INSTANCE,
	SAVEFILE_LINES
};

struct SAVEFILE {
	int player_max_health;
	int player_current_health;
	int health_flask_uses;
	bool heart_power_up_0;
	bool heart_power_up_1;
	bool sword_power_up_0;
	bool is_chicken_dead;
	bool start_from_checkpoint;
	bool saved_this_instance;


};
constexpr int line_count = static_cast<int>(SAVEFILE_LINES::SAVEFILE_LINES);
