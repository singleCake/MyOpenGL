#include <atlimage.h>

#include "TextureManager.h"
#include <iostream>

#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1

std::map<std::string, GLuint> TextureManager::textureMap;

GLuint TextureManager::getTexture(const char* ID) {
	auto it = textureMap.find(ID);
    if (it != textureMap.end()) {
		return it->second;
    }
    else {
		std::cerr << "Texture not found: " << ID << std::endl;
		return 0; // 返回0表示未找到纹理
	}
}

static const char* allTexture[] = {
    "anon_head", "anon_front", "anon_back", "anon_arm", "anon_leg",
    "soyo_head", "soyo_front", "soyo_back", "soyo_arm", "soyo_leg",
    "mercury", "venus", "earth", "mars", "jupiter",
    "saturn", "uranus", "neptune", "sun", "moon",
    "saturn_ring",
    "stone", "plank", "brick"
};

bool TextureManager::init() {
    for (const char* textureID : allTexture) {
        if (!loadTexture(textureID)) {
            std::cerr << "Failed to load texture." << std::endl;
            return false;
        }
    }
	return true;
}

void TextureManager::close() {
    for (auto& pair : textureMap) {
		glDeleteTextures(1, &pair.second);
	}
	textureMap.clear();
}

bool TextureManager::loadTexture(const char* ID) {
    CImage img;
    HRESULT hResult = img.Load(("img/" + std::string(ID) + ".bmp").c_str());
    if (FAILED(hResult))
    {
        return false;
    }
    glGenTextures(1, &textureMap[ID]);					// Create The Texture
    glBindTexture(GL_TEXTURE_2D, getTexture(ID));
    // Generate The Texture
    int pitch = img.GetPitch();
    if (pitch < 0)
        glTexImage2D(GL_TEXTURE_2D, 0, img.GetBPP() / 8, img.GetWidth(), img.GetHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, img.GetPixelAddress(0, img.GetHeight() - 1));
    else
        glTexImage2D(GL_TEXTURE_2D, 0, img.GetBPP() / 8, img.GetWidth(), img.GetHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, img.GetBits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtering
    //glDisable(GL_CULL_FACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glFrontFace(GL_CCW);

    img.Destroy();

    return true;
}