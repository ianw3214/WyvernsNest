#pragma once

#include <SDL2/SDL.h>

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