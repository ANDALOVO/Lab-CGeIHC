#pragma once
#include<glew.h>
class Texture
{
public:
	Texture();
	Texture(const char* FileLoc);
	bool LoadTexture();
	bool LoadTextureA();
	bool LoadFromMemory(const unsigned char* data, int size);
	bool LoadRGBA(const unsigned char* data, int imageWidth, int imageHeight);
	void UseTexture();
	void ClearTexture();
	~Texture();
private: 
	GLuint textureID;
	int width, height, bitDepth;
	const char *fileLocation;

};

