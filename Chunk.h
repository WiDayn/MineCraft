#pragma once
#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <unordered_map>

#include "XYZ.h"

class Chunk
{
private:
	int x, z;
	std::unordered_map<XYZ, int, XYZ::Hash> cubeList;
public:
	Chunk();

	Chunk(int x, int z);

	void setXZ(int x, int z) { this->x = x; this->z = z; }

	int getX() { return x; };

	int getZ() {return z; };
	// 加载位于(x,z)的区块
	void load();
	// 生成位于(x, z)的区块
	void generate();
	// 保存位于(x,z)的区块
	void save();

	bool operator==(const Chunk& other) const {
		return x == other.x && z == other.z;
	}

	struct Hash {
		std::size_t operator()(const Chunk& chunk) const {
			return std::hash<int>()(chunk.x) ^ std::hash<int>()(chunk.z);
		}
	};

	std::unordered_map<XYZ, int, XYZ::Hash> getCubeList();
	// 序列化函数
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("cubeList", cubeList));
	}
};

