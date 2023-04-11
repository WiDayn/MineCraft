#include "Map.h"

Chunk* Map::getChunk(int x, int z)
{
    return &chunks[{x, 0, z}];
}
