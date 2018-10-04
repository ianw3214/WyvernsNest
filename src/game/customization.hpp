#pragma once

#include "../engine/core.hpp"

class Customization : public State {

public:

	Customization();
	~Customization();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

private:

};