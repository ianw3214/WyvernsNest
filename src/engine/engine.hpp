#pragma once

#include <SDL.h>
#include "renderer.hpp"

class Engine {

public:
	static Engine& get_instance() {
		static Engine instance;
		return instance;
	}

	// delete copy and move constructors and assign operators
	Engine(Engine const&) = delete;             // Copy construct
	Engine(Engine&&) = delete;                  // Move construct
	Engine& operator=(Engine const&) = delete;  // Copy assign
	Engine& operator=(Engine &&) = delete;      // Move assign

	// Functions to make the game function properly
	bool init(const char * name, int window_width, int window_height);
	bool running() const;
	void update();

	// Getter functions to make life easier
	int getWindowWidth() const;
	int getWindowHeight() const;

protected:
	Engine();
	~Engine();

private:
	// System objects
	Renderer * renderer;

	SDL_Window * m_window;
	SDL_GLContext m_context;

	bool m_running;

	// Other configuration variables
	int m_windowWidth;
	int m_windowHeight;
};