#include "Chunk.h"
#include "FileReader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Chunk::Chunk()
{
}

Chunk::Chunk(int x, int z)
{
	this->x = x;
	this->z = z;
}

void Chunk::load()
{
	cubeList = std::unordered_map<XYZ, int, XYZ::Hash>();
	std::string chunkFilePath = "./saves/" + std::to_string(x) + "_" + std::to_string(z) + ".cuk";
	FileReader fileReader = FileReader();
	// 如果文件存在，就读文件
	if (fileReader.isFileExists(chunkFilePath)) {
		// 反序列化
		std::ifstream is(chunkFilePath, std::ios::binary);
		cereal::BinaryInputArchive archive(is);
		archive(cubeList);
	} else {
		generate();
	}
}

void Chunk::generate()
{
	int chunkSize = 16;
	for (int x = 0; x < chunkSize; x++) {
		for (int z = 0; z < chunkSize; z++) {
			cubeList[XYZ(x, 0, z)] = 1;
		}
	}
}

void Chunk::save()
{
	std::string chunkFilePath = "./saves/" + std::to_string(x) + "_" + std::to_string(z) + ".cuk";
	std::ofstream os("chunk.bin", std::ios::binary);
	cereal::BinaryOutputArchive archive(os);
	archive(*this);
}

std::unordered_map<XYZ, int, XYZ::Hash> Chunk::getCubeList()
{
	return cubeList;
}
