#pragma once

// Includes of core classes
#include "engine.hpp"
#include "renderer.hpp"
#include "text/textRenderer.hpp"
#include "textureManager.hpp"
#include "mixer.hpp"
#include "entity.hpp"
#include "state.hpp"
#include "sprite.hpp"

// Wrappers around core engine functionalities
namespace Core {

	inline bool init(const char * name, int window_width, int window_height) {
		return Engine::get_instance().init(name, window_width, window_height);
	}

	inline void setState(State * state) {
		Engine::get_instance().setState(state);
	}

	inline bool running() {
		return Engine::get_instance().running();
	}

	inline void update() {
		Engine::get_instance().update();
	}

	inline int windowWidth() {
		return Engine::get_instance().getWindowWidth();
	}

	inline int windowHeight() {
		return Engine::get_instance().getWindowHeight();
	}

	inline float getSeconds() {
		return Engine::get_instance().getTicks()/1000.0f;
	}

	inline void setDebugMode(bool mode) {
		Engine::get_instance().setDebugMode(mode);
	}

	inline bool getdebugMode(bool mode) {
		return Engine::get_instance().getDebugMode();
	}

	inline Texture * getTexture(const std::string& fileName) {
		return Engine::get_instance().getTextureManager()->getTexture(fileName);
	}

	// Wrappers around renderer functionalities
	namespace Renderer {

		inline void drawLine(ScreenCoord v1, ScreenCoord v2, Colour colour) {
			Engine::get_instance().getRenderer()->drawLine(v1, v2, colour);
		}

		inline void drawRect(ScreenCoord v, int width, int height, Colour colour) {
			Engine::get_instance().getRenderer()->drawRect(v, width, height, colour);
		}

		inline void drawTexture(ScreenCoord v, int width, int height, const Texture& texture) {
			Engine::get_instance().getRenderer()->drawTexture(v, width, height, texture);
		}

		inline void drawSprite(const Sprite& sprite) {
			Engine::get_instance().getRenderer()->drawSprite(sprite);
		}

		inline void clear() {
			Engine::get_instance().getRenderer()->clear();

		}

		inline void setAlpha(float a) {
			Engine::get_instance().getRenderer()->setAlpha(a);

		}

	}

	// Wrappers around text renderer functionalities
	namespace Text_Renderer {

		inline void render(std::string s, ScreenCoord pos, float scale = 1.f) {
			Engine::get_instance().getTextRenderer()->render(s, pos, scale);
		}

		inline void setAlignment(TextRenderer::hAlign ha, TextRenderer::vAlign va) {
			Engine::get_instance().getTextRenderer()->setAlignment(ha, va);
		}
		inline void setColour(Colour colour) { 
			Engine::get_instance().getTextRenderer()->setColour(colour); 
		}

	}

	// Wrappers around mixer functionalities
	namespace Mixer {
		inline void loadAudio(const std::string &filePath, AudioType audioType) {
			Engine::get_instance().getMixer()->loadAudio(filePath, audioType);
		}

		inline void playAudio(const std::string &filePath, int loops, float volume = 1) {
			Engine::get_instance().getMixer()->playAudio(filePath, loops, volume);
		}

		inline void pauseAllAudio() {
			Engine::get_instance().getMixer()->pauseAllAudio();
		}

		inline void resumeAllAudio() {
			Engine::get_instance().getMixer()->resumeAllAudio();
		}

		inline void fadeOutAllMusic(int ms) {
			Engine::get_instance().getMixer()->fadeOutAllMusic(ms);
		}

		inline void fadeInAllMusic(const std::string &filePath, int ms, int loops = 0) {
			Engine::get_instance().getMixer()->fadeInMusic(filePath, ms, loops);
		}
		
	}

}