#include "cutscene.hpp"

#include <iostream>
#include <ctime>

Cutscene::Cutscene(State * state, int scene_time, bool timed_scene) :
	cursor("res/assets/UI/cursor.png"),
	cursorPress("res/assets/UI/cursorPress.png")
{

	m_scene_time = scene_time;
	m_position = 0;
	m_state = state;
	m_start = std::clock();
	m_timed_scene = timed_scene;
}

Cutscene::~Cutscene() {
	
}

void Cutscene::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_SPACE && m_position < m_sprites.size() - 1) {
			m_position++;
		}
		else if (e.key.keysym.sym == SDLK_SPACE && m_position == m_sprites.size() - 1) {
			changeState(m_state);
		}
		if (e.key.keysym.sym == SDLK_ESCAPE) {
			// Skip the cutscene if the escape key is pressed
			changeState(m_state);
		}
	}
	// Also cycle through the image on mouse press
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (m_position == m_sprites.size() - 1) changeState(m_state);
		else m_position++;
	}
	// Update the mouse position/state
	SDL_GetMouseState(&mouseX, &mouseY);
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		mouseDown = true;
	}
	if (e.type == SDL_MOUSEBUTTONUP) {
		mouseDown = false;
	}
}

void Cutscene::update(int delta) {
	if (((std::clock() - m_start) / (double)CLOCKS_PER_SEC) > m_scene_time && m_timed_scene) {
		if (m_position >= m_sprites.size() - 1) {
			changeState(m_state);
		} else {
			m_start = std::clock();
		}
	}
}

void Cutscene::render() {
	m_sprites[m_position].render();

	// Render the cursor
	if (mouseDown) {
		cursorPress.setPos(mouseX, mouseY);
		cursorPress.render();
	} else {
		cursor.setPos(mouseX, mouseY);
		cursor.render();
	}
}

void Cutscene::addSprite(std::string path) {
	m_sprites.push_back(Sprite(path));
	m_sprites[m_sprites.size() - 1].setSize(Core::windowWidth(), Core::windowHeight());
}
