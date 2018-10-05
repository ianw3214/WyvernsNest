#pragma once

#include <vector>

#include "entity.hpp"

class State {

public:

	State();
	virtual ~State();

	virtual void handleEvent(const SDL_Event& e);
	virtual void update(int delta);
	virtual void render();

	// Methods to modify entities to the state
	void addEntity(Entity * entity);
	void removeEntity(Entity * entity);

protected:
	// The list of entities in the state
	std::vector<Entity*> entities;

	// State change helper methods
	void changeState(State * state);
};