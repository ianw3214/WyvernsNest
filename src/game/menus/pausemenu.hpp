#pragma once

#include "../../engine/core.hpp"
#include "../util/button.hpp"

class PauseMenu : public State
{
public:
	PauseMenu();
	~PauseMenu();

	// State functions override
	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

	// Setters and getters
	void setIsShown(bool value);
	bool getIsShown();

private:
	// Sprite/Button init
	void initSprites();

	// Menu Buttons
	ButtonData continueButton;
	ButtonData resumeButton;
	ButtonData menuButton;
	ButtonData quitButton;

	// Pause sprite
	Sprite pauseSprite;

	// Render trigger
	bool isShown = false;
};

