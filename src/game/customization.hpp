#pragma once

#include "../engine/core.hpp"

class Customization : public State {

public:

	Customization();
	~Customization();

	void handleEvent(SDL_Event e);
	void update(int delta);
	void render();

private:

};