#pragma once
#define SAVE_FILE_PATH "saveFile.txt"
#include <string>

class Serialize {
public:
	// opens file, returns int stored at line from file, or default value if can't
	static int readIntFromFile(const std::string& filePath, int lineNumber, int defaultValue);
	const static std::string BoolToString(bool b);
};

