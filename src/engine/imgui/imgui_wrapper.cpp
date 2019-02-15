#include "imgui_wrapper.hpp"

#include "core.hpp"

#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

ImGuiWrapper::ImGuiWrapper() {
	// Initialize member variables first
	g_window = Core::getWindow();

	// Init ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

	ImGui_ImplOpenGL3_Init("#version 400");
}

ImGuiWrapper::~ImGuiWrapper() {

}

void ImGuiWrapper::render() {
	// TODO: (Ian) Fix these calculations
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(1280, 720);
	io.DeltaTime = 1.0f / 60.f;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWrapper::onEvent(const SDL_Event& e) {
	ImGuiIO& io = ImGui::GetIO();
	switch (e.type) {
	case SDL_MOUSEWHEEL:        onMouseWheelEvent(e.wheel, io);         break;
	case SDL_MOUSEBUTTONDOWN:   onMousePressedEvent(e.button, io);      break;
	case SDL_MOUSEBUTTONUP:     onMouseReleasedEvent(e.button, io);     break;
	case SDL_MOUSEMOTION:       onMouseMovedEvent(e.motion, io);        break;
	case SDL_TEXTINPUT:         onTextInputEvent(e.text, io);           break;
	case SDL_KEYDOWN:           onKeyDownEvent(e.key, io);              break;
	case SDL_KEYUP:             onKeyUpEvent(e.key, io);                break;
	case SDL_WINDOWEVENT_RESIZED:   onWindowResizedEvent(e.window, io); break;
	}
}

void ImGuiWrapper::onMouseWheelEvent(const SDL_MouseWheelEvent& e, ImGuiIO& io) {
	io.MouseWheel += e.y;
	io.MouseWheelH += e.x;
}

void ImGuiWrapper::onMousePressedEvent(const SDL_MouseButtonEvent& e, ImGuiIO& io) {
	if (e.button == SDL_BUTTON_LEFT)    io.MouseDown[0] = true;
	if (e.button == SDL_BUTTON_MIDDLE)  io.MouseDown[1] = true;
	if (e.button == SDL_BUTTON_RIGHT)   io.MouseDown[2] = true;
}

void ImGuiWrapper::onMouseReleasedEvent(const SDL_MouseButtonEvent& e, ImGuiIO& io) {
	if (e.button == SDL_BUTTON_LEFT)    io.MouseDown[0] = false;
	if (e.button == SDL_BUTTON_MIDDLE)  io.MouseDown[1] = false;
	if (e.button == SDL_BUTTON_RIGHT)   io.MouseDown[2] = false;
}

void ImGuiWrapper::onMouseMovedEvent(const SDL_MouseMotionEvent& e, ImGuiIO& io) {
	io.MousePos = ImVec2(static_cast<float>(e.x), static_cast<float>(e.y));
}

void ImGuiWrapper::onTextInputEvent(const SDL_TextInputEvent& e, ImGuiIO& io) {
	io.AddInputCharactersUTF8(e.text);
}

void ImGuiWrapper::onKeyDownEvent(const SDL_KeyboardEvent& e, ImGuiIO& io) {
	io.KeysDown[e.keysym.scancode] = true;

	io.KeyCtrl = io.KeysDown[SDL_SCANCODE_LCTRL] || io.KeysDown[SDL_SCANCODE_RCTRL];
	io.KeyShift = io.KeysDown[SDL_SCANCODE_LSHIFT] || io.KeysDown[SDL_SCANCODE_RSHIFT];
	io.KeyAlt = io.KeysDown[SDL_SCANCODE_LALT] || io.KeysDown[SDL_SCANCODE_RALT];

	// TODO: (Ian) Implement
	io.KeySuper = false;
}

void ImGuiWrapper::onKeyUpEvent(const SDL_KeyboardEvent& e, ImGuiIO& io) {
	io.KeysDown[e.keysym.scancode] = false;

	io.KeyCtrl = io.KeysDown[SDL_SCANCODE_LCTRL] || io.KeysDown[SDL_SCANCODE_RCTRL];
	io.KeyShift = io.KeysDown[SDL_SCANCODE_LSHIFT] || io.KeysDown[SDL_SCANCODE_RSHIFT];
	io.KeyAlt = io.KeysDown[SDL_SCANCODE_LALT] || io.KeysDown[SDL_SCANCODE_RALT];
}

void ImGuiWrapper::onWindowResizedEvent(const SDL_WindowEvent& e, ImGuiIO& io) {
	io.DisplaySize = ImVec2(static_cast<float>(e.data1), static_cast<float>(e.data2));
	io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
}