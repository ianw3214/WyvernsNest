#pragma once

#include <SDL.h>
#include "resourceEngine.hpp"

class Entity {

public:

	Entity();
	virtual ~Entity();

	virtual void handleEvent(const SDL_Event& event);
	virtual void update(int delta);
	virtual void render();

	void remove();
	bool shouldRemove() const;

protected:
	bool m_remove;
};