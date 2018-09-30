#include "engine.hpp"

// Library includes
#include <GL/glew.h>
#include <SDL_opengl.h>

// Std library includes
#include <iostream>

#include "renderer.hpp"

bool Engine::init(const char * name, int window_width, int window_height) {

	// Set engine configuration variables
	m_windowWidth = window_width;
	m_windowHeight = window_height;

	// Create the window and setup an openGL context
	m_window = SDL_CreateWindow(name,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		window_width,
		window_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (m_window == NULL) {
		SDL_Log("Unable to create SDL window: %s", SDL_GetError());
		return false;
	}
	m_context = SDL_GL_CreateContext(m_window);
	if (m_context == NULL) {
		SDL_Log("Unable to create openGL context: %s", SDL_GetError());
		return false;
	}

	// Setup openGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetSwapInterval(1);

	// Setup blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std::cout << "Error initializing GLEW: "
			<< glewGetErrorString(glewError)
			<< std::endl;
		return false;
	}

	// Initialize the renderer after the OpenGL context is created
	renderer = new Renderer();

	return true;
}

bool Engine::running() const {
	return m_running;
}

void Engine::update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			m_running = false;
		}
		if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				m_running = false;
			}
		}
	}

	// TESTING CODE
	renderer->drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(1.0, 0.0, 0.0));
	Texture texture("res/test.png");
	renderer->drawTexture(ScreenCoord(500, 500), 100, 100, texture);

	SDL_GL_SwapWindow(m_window);
}

int Engine::getWindowWidth() const {
	return m_windowWidth;
}

int Engine::getWindowHeight() const
{
	return m_windowHeight;
}

Engine::Engine() : m_running(true) {
	// Intialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		throw new std::exception("SDL failed to initialize");
	}
}

Engine::~Engine() {
	SDL_GL_DeleteContext(m_context);
	SDL_Quit();
}