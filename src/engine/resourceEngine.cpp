#include "resourceEngine.hpp"

// Set resoruces that will be used 
ResourceEngine::ResourceEngine() {
	setSprite("test", "res/test.png");
}

ResourceEngine::~ResourceEngine() {
	for (auto& mapPair : m_spriteMap) {
		delete mapPair.second;
	}
}

void ResourceEngine::setSprite(const std::string& spriteName, const std::string& fileName) {
	delete m_spriteMap[spriteName];
	m_spriteMap[spriteName] = new Sprite(fileName);
}
void ResourceEngine::deleteSprite(const std::string& fileName, const std::string& spriteName) {
	delete m_spriteMap[spriteName];
	m_spriteMap.erase(spriteName);
}
Sprite* ResourceEngine::getSprite(const std::string& spriteName) {
	if (m_spriteMap.count(spriteName) == 0) {
		throw std::runtime_error("Resource not found: " + spriteName);
	}
	return m_spriteMap[spriteName];
}
