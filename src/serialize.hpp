#pragma once
#define SAVE_FILE_PATH "saveFile.txt"
#include <string>

// opens file, returns int stored at line from file, or default value if can't
int readIntFromFile(const std::string& filePath, int lineNumber, int defaultValue);
