#pragma once
#define SAVE_FILE_PATH "saveFile.txt"
#include <string>

	// opens file, returns int stored at line from file, or default value if can't
	int readIntFromFile(const std::string& filePath, int lineNumber, int defaultValue);

	// opens file, returns bool stored at line from file, or default value if can't
	bool readBoolFromFile(const std::string& filePath, int lineNumber, bool defaultValue);
	const std::string BoolToString(bool b);


