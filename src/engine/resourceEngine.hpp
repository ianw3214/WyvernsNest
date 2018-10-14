#pragma once
#include "sprite.hpp"
#include <string>
#include <unordered_map>
#include <exception>

class ResourceEngine {
public:

	ResourceEngine();
	~ResourceEngine();

	void setSprite(const std::string& spriteName, const std::string& fileName);
	void deleteSprite(const std::string& fileName, const std::string& spriteName);
	Sprite* getSprite(const std::string& spriteName);

private:
	std::unordered_map<std::string, Sprite*> m_spriteMap;
};
