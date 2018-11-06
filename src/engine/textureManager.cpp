#include "textureManager.hpp"

#include "opengl/texture.hpp"

TextureManager::TextureManager() {

}

TextureManager::~TextureManager() {

}

Texture * TextureManager::getTexture(const std::string & fileName) {
	if (m_textures.find(fileName) == m_textures.end()) {
		m_textures[fileName] = new Texture(fileName);
	}
	return m_textures[fileName];
}

void TextureManager::deleteTexture(const std::string & fileName) {
	delete m_textures[fileName];
	m_textures.erase(fileName);
}