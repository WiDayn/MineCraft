#pragma once
#include <unordered_map>
#include <string>

class TextureLoader
{
private:
	// asset�洢��map
	std::unordered_map<std::string, unsigned int> assetMap;
public:
	void loadNewAsset(std::string name, std::string fileDir);

	unsigned int getAsset(std::string name);

	int getNumOfAsset();
};

