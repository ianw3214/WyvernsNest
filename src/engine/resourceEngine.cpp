#include "resourceEngine.hpp"

// Set resoruces that will be used 
void ResourceEngine::init() {
	setSprite("test1", "res/test.png");
	setSprite("test2", "res/test2.png");
	setSprite("test3", "res/test3.png");
	setSprite("test4", "res/test4.png");
	setSprite("test5", "res/test5.png");
}

std::unordered_map<std::string, Sprite*> ResourceEngine::m_spriteMap;
