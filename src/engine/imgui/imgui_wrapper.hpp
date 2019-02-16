#pragma once

#include "imgui.h"
#include <SDL2/SDL.h>

class ImGuiWrapper {

public:
	ImGuiWrapper();
	~ImGuiWrapper();

	void render();
	void onEvent(const SDL_Event& e);

private:
	void onMouseWheelEvent(const SDL_MouseWheelEvent& e, ImGuiIO& io);
	void onMousePressedEvent(const SDL_MouseButtonEvent& e, ImGuiIO& io);
	void onMouseReleasedEvent(const SDL_MouseButtonEvent& e, ImGuiIO& io);
	void onMouseMovedEvent(const SDL_MouseMotionEvent& e, ImGuiIO& io);
	void onTextInputEvent(const SDL_TextInputEvent& e, ImGuiIO& io);
	void onKeyDownEvent(const SDL_KeyboardEvent& e, ImGuiIO& io);
	void onKeyUpEvent(const SDL_KeyboardEvent& e, ImGuiIO& io);
	void onWindowResizedEvent(const SDL_WindowEvent& e, ImGuiIO& io);

	SDL_Window * g_window;
};