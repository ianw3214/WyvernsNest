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
	Cutscene(State * state, int scene_time=5, bool timed_scene=false);
	~Cutscene();

	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

	void addSprite(std::string path);

private:
	bool m_timed_scene;
	State *m_state;
	std::clock_t m_start;
	int m_scene_time;

	// State properties
	int mouseX, mouseY;
	bool mouseDown;

	// Utility sprites
	Sprite cursor;
	Sprite cursorPress;

	unsigned int m_position;
	std::vector<Sprite> m_sprites;
};
