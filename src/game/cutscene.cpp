#include "cutscene.hpp"

Cutscene::Cutscene(State * state) {
	nextState = state;
}

Cutscene::~Cutscene() {

}

void Cutscene::handleEvent(const SDL_Event& e) {

}

void Cutscene::update(int delta) {

}

void Cutscene::render() {

}
