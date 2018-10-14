#pragma once
#include "sprite.hpp"
#include <string>
#include <unordered_map>
#include <exception>

class ResourceEngine {
public:
	static void init();

	~ResourceEngine() {
		for (auto mapPair : m_spriteMap) {
			delete mapPair.second;
		}
	}

	static void setSprite(const std::string& spriteName, const std::string& fileName) {
		delete m_spriteMap[spriteName];
		m_spriteMap[spriteName] = new Sprite(fileName);
	}
	static void deleteSprite(const std::string& fileName, const std::string& spriteName) {
		delete m_spriteMap[spriteName];
		m_spriteMap.erase(spriteName);
	}
	static Sprite* getSprite(const std::string& spriteName) {
		if (m_spriteMap.count(spriteName) == 0) {
			throw std::runtime_error("Resource not found: " + spriteName);
		}
		return m_spriteMap[spriteName];
	}
private:
	ResourceEngine();
	static std::unordered_map<std::string, Sprite*> m_spriteMap;
	static Sprite* m_nullSprite;
};
