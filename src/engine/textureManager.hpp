#pragma once

#include <string>
#include <unordered_map>

class Texture;

class TextureManager {
public:

	TextureManager();
	~TextureManager();

	Texture* getTexture(const std::string& fileName);
	void deleteTexture(const std::string& fileName);

private:
	std::unordered_map<std::string, Texture*> m_textures;
};