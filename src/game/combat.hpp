#pragma once

#include "../engine/core.hpp"

class Combat : public State {

public:

	Combat();
	~Combat();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

private:

};