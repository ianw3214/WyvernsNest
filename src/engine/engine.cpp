#include "engine.hpp"

// Library includes
#include <GL/glew.h>
#include <SDL_opengl.h>

// Std library includes
#include <iostream>

// Other project includes
#include "renderer.hpp"
#include "state.hpp"

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
		std::cerr << "Error initializing GLEW: "
			<< glewGetErrorString(glewError)
			<< std::endl;
		return false;
	}

	// Initialize the renderer after the OpenGL context is created
	m_renderer = new Renderer();

	// reset m_lastTick for a more accurate first tick
	m_lastTick = SDL_GetTicks();

	return true;
}

bool Engine::running() const {
	return m_running;
}

void Engine::update() {

	// Handle the delta time, only tick when the time threshold is reached
	m_delta = SDL_GetTicks() - m_lastTick;
	if (m_delta < m_msPerTick) return;
	m_lastTick = SDL_GetTicks();

	// Poll for events and update the state accordingly
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
		if (m_state) m_state->handleEvent(event);
	}
	if (m_state) m_state->update(m_delta);
	if (m_state) m_state->render();

	SDL_GL_SwapWindow(m_window);
}

int Engine::getWindowWidth() const {
	return m_windowWidth;
}

int Engine::getWindowHeight() const
{
	return m_windowHeight;
}

Renderer * Engine::getRenderer() {
	return m_renderer;
}

void Engine::setState(State * state) {
	if (m_state) delete m_state;
	m_state = state;
}

Engine::Engine() :
	m_running(true),
	m_delta(0),
	m_lastTick(0),
	m_windowWidth(0),
	m_windowHeight(0),
	m_tickRate(DEFAULT_TICK_RATE),
	m_msPerTick(0)
{
	// Intialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		// throw new std::exception("SDL failed to initialize");
	}
	// Calculate ms per tick depending on tick rate
	m_msPerTick = static_cast<int>(1000.f / static_cast<float>(m_tickRate));
}

Engine::~Engine() {
	delete m_state;

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}