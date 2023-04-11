#pragma once
#include<unordered_map>

#include "XYZ.h"
#include "Chunk.h"

class Map
{
protected:
	std::unordered_map<XYZ, Chunk, XYZ::Hash> chunks;
public:
	Chunk* getChunk(int x, int z);
};

