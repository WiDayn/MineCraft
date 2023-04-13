#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"

void TextureLoader::loadNewAsset(std::string name, std::string fileDir)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileDir.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	if (!data) {
		std::cout << "Failed to load texture: " << name << std::endl;
	}
	glGenTextures(1, &assetMap[name]);
	glBindTexture(GL_TEXTURE_2D, assetMap[name]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	// 生成Mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	// 释放data内存
	stbi_image_free(data);
	std::cout << "Load " << name << " texture Successfully." << std::endl;
};

unsigned int TextureLoader::getAsset(std::string name)
{
	return assetMap[name];
}

int TextureLoader::getNumOfAsset()
{
	return assetMap.size();
}
