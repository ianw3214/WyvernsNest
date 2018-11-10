/**
  *		Cutscene game state
  *			- Set the next scene to transition to
  *			- Store a list of images(sprites) to show in the cutscene
  *			- Handle user input (mouse click or button press) to move between images
  *
  *		Future goals
  *			- Load cutscene data from file
  *			- Animated sprites in cutscenes
  *			- Text rendering in cutscenes
  *			- Sprite movement in cutscenes
  */
#pragma once

#include "../engine/core.hpp"
#include <ctime>

class Cutscene : public State {

public:

	// The constructor takes the next State to transition to as input
	Cutscene(State * state);
	~Cutscene();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

private:
	State *m_state;
	std::clock_t m_start;
	Sprite *m_curr_img;
	unsigned int m_position;
	std::vector<Sprite *> m_sprites;
};
