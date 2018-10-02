#pragma once

#include "../engine/core.hpp"

class Combat : public State {

public:

	Combat();
	~Combat();

	void handleEvent(SDL_Event e);
	void update(int delta);
	void render();

private:

};