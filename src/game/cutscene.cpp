#include "cutscene.hpp"

#include <iostream>

Cutscene::Cutscene(State * state) {

	// For testing purposes populate sprites with temp images
	m_sprites.push_back(new Sprite("res/test.png"));
	m_sprites.push_back(new Sprite("res/test2.png"));
	m_sprites.push_back(new Sprite("res/test3.png"));
	m_sprites.push_back(new Sprite("res/test4.png"));
	m_sprites.push_back(new Sprite("res/test5.png"));

	m_position = 0;
	m_curr_img = m_sprites[m_position];
	m_state = state;

}

Cutscene::~Cutscene() {
	
}

void Cutscene::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_a && m_position < sizeof(m_sprites) - 1) {
			m_curr_img = m_sprites[m_position++];
		}
		else if (e.key.keysym.sym == SDLK_a && m_position == sizeof(m_sprites) - 1) {
			changeState(m_state);
		}
	}
}

void Cutscene::update(int delta) {

}

void Cutscene::render() {
	m_curr_img->render();
}
