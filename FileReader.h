#pragma once
#include <string>

class FileReader
{
public:
	std::string readFile(std::string filePath);
	bool isFileExists(std::string filePath);
};

