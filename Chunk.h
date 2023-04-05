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
	// ����λ��(x,z)������
	void load();
	// ����λ��(x, z)������
	void generate();
	// ����λ��(x,z)������
	void save();

	std::unordered_map<XYZ, int, XYZ::Hash> getCubeList();
	// ���л�����
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("cubeList", cubeList));
	}
};

