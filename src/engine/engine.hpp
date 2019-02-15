#pragma once

#include <SDL2/SDL.h>

#define DEFAULT_TICK_RATE	30	// per second

class Renderer;
class TextRenderer;
class TextureManager;
class Mixer;

class State;

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
	Renderer * getRenderer();
	TextRenderer * getTextRenderer();
	TextureManager * getTextureManager();
	Mixer * getMixer();

	// State changing functions
	void setState(State * state);

	inline void setDebugMode(bool mode) { m_debugMode = mode; }
	inline bool getDebugMode() { return m_debugMode; }
	inline int getTicks() { return m_lastTick; }


protected:
	Engine();
	~Engine();
 
private:
	// System objects
	Renderer * m_renderer;
	TextRenderer * m_textRenderer;
	TextureManager * m_textureManager;
	Mixer * m_mixer;

	// SDL/OpenGL specific objects
	SDL_Window * m_window;
	SDL_GLContext m_context;

	// The current state
	State * m_state;

	// engine variables
	bool m_running;
	int m_delta;
	int m_lastTick;

	// Other configuration variables
	int m_windowWidth;
	int m_windowHeight;
	int m_tickRate;
	int m_msPerTick;

	// macOS fix
	int mac_fix;

	// debug mode
	bool m_debugMode = false;
};