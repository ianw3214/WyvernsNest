#pragma once

// Includes of core classes
#include "engine.hpp"
#include "renderer.hpp"
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

	// Wrappers around renderer functionalities
	namespace Renderer {

		inline void drawLine(ScreenCoord v1, ScreenCoord v2, Colour colour) {
			Engine::get_instance().getRenderer()->drawLine(v1, v2, colour);
		}

		inline void drawTexture(ScreenCoord v, int width, int height, const Texture& texture) {
			Engine::get_instance().getRenderer()->drawTexture(v, width, height, texture);
		}

		inline void clear() {
			Engine::get_instance().getRenderer()->clear();

		}

	}

}