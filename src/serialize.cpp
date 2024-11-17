#include "serialize.hpp"
#include <fstream>
#include <sstream>

int Serialize::readIntFromFile(const std::string& filePath, int lineNumber, int defaultValue) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		return defaultValue;
	}

	std::string line;
	int currentLine = 0;

	while (std::getline(file, line)) {
		if (currentLine == lineNumber) {
			std::istringstream iss(line);
			int value;
			if (iss >> value) {
				return value;
			}
			return defaultValue;
		}
		currentLine++;
	}
	return defaultValue;
}

const std::string Serialize::BoolToString(bool b)
{
	return b ? "true" : "false";
}