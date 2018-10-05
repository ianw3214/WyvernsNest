#include "state.hpp"

#include "engine.hpp"

State::State() {

}

State::~State() {
	for (Entity * e : entities) delete e;
}

void State::handleEvent(const SDL_Event& event) {
	for (Entity * e : entities) e->handleEvent(event);
}

void State::update(int delta) {
	for (Entity * e : entities) e->update(delta);
	// Remove entities if needed
	for (int i = entities.size() - 1; i >= 0; --i) {
		if (entities[i]->shouldRemove()) {
			delete entities[i];
			entities.erase(entities.begin() + i);
		}
	}
}

void State::render() {
	for (Entity * e : entities) e->render();
}

void State::addEntity(Entity * entity) {
	entities.push_back(entity);
}

void State::removeEntity(Entity * entity) {
	entity->remove();
}

void State::changeState(State * state) {
	Engine::get_instance().setState(state);
}
