#pragma once
class XYZ
{
public:
	int x, y, z;

    XYZ() : x(0), y(0), z(0) {}

    XYZ(int x, int y, int z) : x(x), y(y), z(z) {}

    bool operator==(const XYZ& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(x, y, z);
    }

    struct Hash {
        std::size_t operator()(const XYZ& xyz) const {
            return std::hash<int>()(xyz.x) ^ std::hash<int>()(xyz.y) ^ std::hash<int>()(xyz.z);
        }
    };
};

