#pragma once

#include "../engine/core.hpp"
#include "../math/vec.hpp"

#include "combat/grid.hpp"
#include "../engine/particleSystem.hpp"
#include "util/button.hpp"
#include "unitData.hpp"

#include <vector>

#include <nlohmann/json.hpp>

#define GAME_OVER_TIMER			(2000 / 33)

// Other hard coded values
#define USER_SAVE_LOCATION_COMBAT "res/data/save.json"

// Struct used to hold player data to render at game over screen
struct GameOverData {
	std::string name;
	int gained_exp;
	bool level_up;
};

class Unit;
class Player;
class Enemy;

// TODO: Implement loading unit stats and calculating health, speed, etc. accordingly
// TODO: Somehow merge the selectUnit and nextUnitTurn functions
class Combat : public State {

public:

	Combat(const std::string& gridPath, bool last_level = false);
	~Combat();

	// Default state functions called from the state handler
	void handleEvent(const SDL_Event& e) override;
	void update(int delta) override;
	void render() override;

	// Utility function to get the unit at a grid position
	Unit * getUnitAt(ScreenCoord at);

	// Other utility functions
	std::vector<Player*> getPlayers() const;
	void addPlayer(int x, int y, const nlohmann::json& data);
	void addEnemy(Enemy * enemy, int x, int y);

	Grid grid;
	bool isPosEmpty(Vec2<int> pos) const;

	// Particle System
	ParticleSystem ps;
	void addEmitter(Emitter * emitter);

private:

	// Combat state variables
	bool pause;
	bool render_game_over;
	bool game_over;
	bool game_win;
	Unit * current;
	std::vector<GameOverData> gameOverData;
	int mouseX, mouseY;
	bool mouseDown;
	bool last_level;

	// Counter variable for showing the game over screen after the player wins
	int game_over_counter;
	void startCounter() { game_over_counter = 0; }
	void incrementCounter() { game_over_counter++; }
	bool compareCounter(int num) const { return game_over_counter >= num; }

	// Sprites to display the game over menu
	void initSprites();
	Sprite gameOverBase;
	Sprite pauseBase;
	Sprite cursor;
	Sprite cursorPress;

	// Buttons
	ButtonData continueButton;
	ButtonData resumeButton;
	ButtonData menuButton;
	ButtonData quitButton;

	// Store a reference to all the units in the combat state
	std::vector<Unit*> units;

	// Utility functions for turn ordering
	void nextUnitTurn();
	void selectUnit(Unit * unit);
	void startGame();

	// Reference to the current unit that is taking its action
	int unitIndex;

	// The total experience granted to players upon completing the level
	float experienceReward;
	std::vector<UnitData> unitRewards;

	// Other helper functions
	void updateWinStatus();


};