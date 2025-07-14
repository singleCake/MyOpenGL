#pragma once

#include "../geometry/Geometry.h"

#include <map>

class TextureManager {
public:
	static bool init();
	static void close();

	static GLuint getTexture(const char* ID);

private:
	static bool loadTexture(const char* filename);

	static std::map<std::string, GLuint> textureMap;
};