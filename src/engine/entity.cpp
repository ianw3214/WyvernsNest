#include "entity.hpp"

Entity::Entity() {

}

Entity::~Entity() {

}

void Entity::handleEvent(SDL_Event e) {

}

void Entity::update(int delta) {

}

void Entity::render() {

}

void Entity::remove() {
	m_remove = true;
}

bool Entity::shouldRemove() const {
	return m_remove;
}
