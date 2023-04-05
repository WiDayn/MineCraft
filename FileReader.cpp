#include "FileReader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>

std::string FileReader::readFile(std::string filePath)
{
    std::string resultString;
    std::ifstream File;
    // ensure ifstream objects can throw exceptions:
    File.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        File.open(filePath);
        std::stringstream fileStream;
        // read file's buffer contents into streams
        fileStream << File.rdbuf();
        // close file handlers
        File.close();
        // convert stream into string
        resultString = fileStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
	return resultString;
}

bool FileReader::isFileExists(std::string filePath)
{
    std::ifstream file(filePath.c_str());
    return file.good();
}