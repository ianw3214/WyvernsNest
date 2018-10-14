#include "resourceEngine.hpp"

// Set resoruces that will be used 
void ResourceEngine::init() {
	setSprite("testt", "res/test.png");
}

std::unordered_map<std::string, Sprite*> ResourceEngine::m_spriteMap;
