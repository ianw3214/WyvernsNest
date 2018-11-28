#include "engine.hpp"

// Library includes
#include <GL/glew.h>
#include <SDL_opengl.h>

// Std library includes
#include <iostream>
#include <ctime>

// Other project includes
#include "renderer.hpp"
#include "text/textRenderer.hpp"
#include "textureManager.hpp"
#include "state.hpp"
#include "mixer.hpp"

bool Engine::init(const char * name, int window_width, int window_height) {

	// Set engine configuration variables
	m_windowWidth = window_width;
	m_windowHeight = window_height;

	// Setup openGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

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

	// Enable vsync
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

	// Initialize the text renderer after the OpenGL context is created
	m_textRenderer = new TextRenderer("res/test_font.ttf", 32, Vec2<int>(m_windowWidth, m_windowHeight));

	// Initialize the texture manager
	m_textureManager = new TextureManager();

	// Initialize the mixer
	m_mixer = new Mixer();

	// reset m_lastTick for a more accurate first tick
	m_lastTick = SDL_GetTicks();

	mac_fix = 0;

	// Seed random
	srand(static_cast<unsigned int>(time(0)));
	
	return true;
}

bool Engine::running() const {
	return m_running;
}

void Engine::update() {
	#ifdef __APPLE__
	// 200 is an arbitrary number but it consistently works for me
	if(mac_fix < 200) {
		mac_fix += 1;
		SDL_SetWindowSize(m_window, 1279, 720);
	}	
		
	if(mac_fix == 200) {
		SDL_SetWindowSize(m_window, 1280, 720);
		mac_fix += 1;
	}
	#endif

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
		if (m_state) m_state->handleEvent(event);
	}

	getRenderer()->clear();
	if (m_state) m_state->update(m_delta);
	if (m_state) m_state->render();
	if (getDebugMode()) {
		getTextRenderer()->render("FPS: " + std::to_string(round(1000.0 / m_delta)), ScreenCoord(0, 0));
	}
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

TextRenderer * Engine::getTextRenderer() {
	return m_textRenderer;
}

TextureManager * Engine::getTextureManager() {
	return m_textureManager;
}

Mixer * Engine::getMixer() {
	return m_mixer;
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

	delete m_renderer;
	delete m_textRenderer;
	delete m_textureManager;

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}