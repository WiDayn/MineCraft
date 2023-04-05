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

	int getX() { return x; };

	int getZ() {return z; };
	// 加载位于(x,z)的区块
	void load();
	// 生成位于(x, z)的区块
	void generate();
	// 保存位于(x,z)的区块
	void save();

	std::unordered_map<XYZ, int, XYZ::Hash> getCubeList();
	// 序列化函数
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("cubeList", cubeList));
	}
};

