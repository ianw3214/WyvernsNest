#include "player.hpp"

#include "../combat.hpp"
#include "../util/attackloader.hpp"
#include "../util/util.hpp"

// Deprecated, should not be called
// TODO: remove
Player::Player() :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	player_sprite("res/assets/players/FemaleSheet.png", 96, 96),
	valid_tile("res/assets/tiles/valid.png")
{
	init();
}

// Deprecated, should not be called
// TODO: remove
Player::Player(int x, int y) :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	player_sprite("res/assets/players/FemaleSheet.png", 96, 96),
	valid_tile("res/assets/tiles/valid.png")
{
	position.x() = x;
	position.y() = y;

	init();
}

Player::Player(int x, int y, const nlohmann::json& data) :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	player_sprite("res/assets/players/FemaleSheet.png", 96, 96),
	valid_tile("res/assets/tiles/valid.png"),
	attack1(Attacks::get("PUNCH", this)),
	attack2(Attacks::get("RANGED", this))
{
	position.x() = x;
	position.y() = y;

	init();

	// Load the unit data from the file
	UnitData unitData;
	unitData.strength		= data["STR"];
	unitData.dexterity		= data["DEX"];
	unitData.intelligence	= data["INT"];
	unitData.constitution	= data["CON"];
	unitData.experience		= data["experience"];
	unitData.level			= data["level"];
	setData(unitData);

	// TODO: Load attacks from here
	attack1 = Attacks::get(data["attack1"], this);
	attack2 = Attacks::get(data["attack2"], this);
	attack3 = Attacks::get(data["attack3"], this);
	attack4 = Attacks::get(data["attack4"], this);
}

Player::~Player() {}

void Player::renderBottom(Combat * combat) {
	shadow.render();
	// Render the corresponding UI elements depending on the players current action
	if (current_action == PlayerAction::MOVE && state == UnitState::IDLE) {
		renderValidMoves();
	}
	if (current_action == PlayerAction::ATTACK_1) {
		attack1.renderValidGrid(tile_width, tile_height, *combat);
	}
	if (current_action == PlayerAction::ATTACK_2) {
		attack2.renderValidGrid(tile_width, tile_height, *combat);
	}
	if (current_action == PlayerAction::ATTACK_3) {
		attack3.renderValidGrid(tile_width, tile_height, *combat);
	}
	if (current_action == PlayerAction::ATTACK_4) {
		attack4.renderValidGrid(tile_width, tile_height, *combat);
	}
}

void Player::render()
{
	// Render the actual player
	player_sprite.setPos(screenPosition.x(), screenPosition.y());
	player_sprite.render();
}

void Player::renderTop(Combat * combat) {
	// If it's the players turn, render player related UI
	if (state == UnitState::IDLE && selected) {
		renderTurnUI();
	}
	renderHealth();
}
#include <iostream>
void Player::renderTurnUI() {
	
	// Setup the variables to draw the UI correctly
	ScreenCoord pos = screenPosition;
	if (pos.x() >= Core::windowWidth() / 2) pos.x() -= SubDiv::hSize(5, 1);
	else {
		pos.x() += SubDiv::hSize(5, 1);
	}
	if (pos.y() < 0) pos.y() = 0;
	Colour base = Colour(.7f, .7f, .7f);
	Colour select = Colour(.9f, .9f, .9f);

	// The actual drawing of the UI elements
	int option_height = SubDiv::vSize(16, 1);
	// The text offset is not even because the font is rendered a little below the center
	ScreenCoord text_offset = ScreenCoord(10, SubDiv::vSize(36, 1));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::middle);
	// Render the move if the player hasn't moved
	if (!moved) {
		Core::Renderer::drawRect(pos, SubDiv::hSize(5, 1), option_height, current_action == PlayerAction::MOVE ? select : base);
		Core::Text_Renderer::render("MOVE", pos + text_offset, 1.f);
	}
	pos.y() += option_height;
	// Render the attacks
	Core::Renderer::drawRect(pos, SubDiv::hSize(5, 1), option_height, current_action == PlayerAction::ATTACK_1 ? select : base);
	Core::Text_Renderer::render(attack1.getName(), pos + text_offset, 1.f);
	pos.y() += option_height;
	Core::Renderer::drawRect(pos, SubDiv::hSize(5, 1), option_height, current_action == PlayerAction::ATTACK_2 ? select : base);
	Core::Text_Renderer::render(attack2.getName(), pos + text_offset, 1.f);
	pos.y() += option_height;
	Core::Renderer::drawRect(pos, SubDiv::hSize(5, 1), option_height, current_action == PlayerAction::ATTACK_3 ? select : base);
	Core::Text_Renderer::render(attack3.getName(), pos + text_offset, 1.f);
	pos.y() += option_height;
	Core::Renderer::drawRect(pos, SubDiv::hSize(5, 1), option_height, current_action == PlayerAction::ATTACK_4 ? select : base);
	Core::Text_Renderer::render(attack4.getName(), pos + text_offset, 1.f);
	pos.y() += option_height;
	// Render the pass option
	Core::Renderer::drawRect(pos, SubDiv::hSize(5, 1), option_height, base);
	Core::Text_Renderer::render("PASS", pos + text_offset, 1.f);
}

void Player::renderValidMoves() {

	valid_tile.setSize(tile_width, tile_height);
	for (ScreenCoord pos : possibleMoves) {
		valid_tile.setPos((pos.x()) * tile_width, (pos.y()) * tile_height);
		valid_tile.render();
	}

	int i = 0;
	for (ScreenCoord pos : path_line) {
		ScreenCoord start;
		ScreenCoord end;
		if (i == 0) {
			i++;
			continue;
		}
		else {
			start = pos;
			end = path_line[i - 1];
		}

		start.x() *= tile_width;
		end.x() *= tile_width;
		start.y() *= tile_height;
		end.y() *= tile_height;

		start.x() += tile_width / 2;
		end.x() += tile_width / 2;
		start.y() += tile_height / 2;
		end.y() += tile_height / 2;

		Core::Renderer::drawLine(start, end, Colour(1, 0, 0));

		i++;
	}
}

void Player::handleEvent(const SDL_Event & event)
{
	// Only handle events for the entity if it is selected
	if (selected && state == UnitState::IDLE) {
		if (event.type == SDL_KEYDOWN) {
			// Move Key
			if (event.key.keysym.sym == SDLK_KP_1 || event.key.keysym.sym == SDLK_1) {
				if (!moved) {
					current_action = PlayerAction::MOVE;
					updatePossibleMoves();
				}
			}
			// Attack 1 key
			if (event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP_2) {
				current_action = PlayerAction::ATTACK_1;
			}
			// Attack 2 key
			if (event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_KP_3) {
				current_action = PlayerAction::ATTACK_2;
			}
			// Attack 3 key
			if (event.key.keysym.sym == SDLK_4 || event.key.keysym.sym == SDLK_KP_4) {
				current_action = PlayerAction::ATTACK_3;
			}
			// Attack 4 key
			if (event.key.keysym.sym == SDLK_5 || event.key.keysym.sym == SDLK_KP_5) {
				current_action = PlayerAction::ATTACK_4;
			}
			// Pass the turn
			if (event.key.keysym.sym == SDLK_KP_6 || event.key.keysym.sym == SDLK_6) {
				current_action = PlayerAction::NONE;
				state = UnitState::DONE;
				player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
			}
		}
		
	}
}

void Player::update(int delta) {
	// TODO: Use a better solution than this, perhaps virtual functions w/ custom callbacks
	Unit::update(delta);
	// Update the player based on its current state	
	switch (state) {
		case UnitState::IDLE: {
			// Do nothing when idling
		} break;
		case UnitState::MOVE: {
			// Move the player towards its destination
			if (compareCounter(PLAYER_DEFAULT_MOVE_COUNTER)) {
				startCounter();
				incrementMovement();
				// If the player reaches the target destination, stop moving it
				if (position.x() == moveTarget.x() && position.y() == moveTarget.y()) {
					state = UnitState::IDLE;
					current_action = PlayerAction::NONE;
					position = moveTarget;
					calculateScreenPosition();
				}
			}
			else {
				incrementCounter();
				calculateScreenPositionMovement();
			}
		} break;
		case UnitState::ATTACK: {
			if (compareCounter(PLAYER_DEFAULT_ATTACK_COUNTER)) {
				state = UnitState::DONE;
				moved = false;
			} else {
				incrementCounter();
			}
		} break;
		case UnitState::DONE: {
			moved = false;
		}
		default: {
			// Do nothing by default
		} break;
	}
}

void Player::click(Vec2<int> to)
{
	if (state != UnitState::IDLE) return;
	switch (current_action) {
		case PlayerAction::NONE: {
			// do nothing
		} break;
		case PlayerAction::MOVE: {
			// Move the player using the base unit move function
			if (move(*combat, to)) moved = true;
			current_action = PlayerAction::NONE;
		} break;
			// TODO: determine if an attack is valid, and don't execute the attack if it isn't
		case PlayerAction::ATTACK_1: {
			// do the action here
			if (attack1.isValid(to, *combat)) {
				attack1.attack(to, *combat);
				current_action = PlayerAction::NONE;
				state = UnitState::ATTACK;
				startCounter();
				// TODO: Play animation based on attack type
				player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_MELEE));
				player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
			}
		} break;
		case PlayerAction::ATTACK_2: {
			// do the action here
			if (attack2.isValid(to, *combat)) {
				attack2.attack(to, *combat);
				current_action = PlayerAction::NONE;
				state = UnitState::ATTACK;
				startCounter();
				// TODO: Play animation based on attack type
				player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_RANGED));
				player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
			}
		} break;
		case PlayerAction::ATTACK_3: {
			// do the action here
			if (attack3.isValid(to, *combat)) {
				attack3.attack(to, *combat);
				current_action = PlayerAction::NONE;
				state = UnitState::ATTACK;
				startCounter();
				// TODO: Play animation based on attack type
				player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_RANGED));
				player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
			}
		} break;
		case PlayerAction::ATTACK_4: {
			// do the action here
			if (attack4.isValid(to, *combat)) {
				attack4.attack(to, *combat);
				current_action = PlayerAction::NONE;
				state = UnitState::ATTACK;
				startCounter();
				// TODO: Play animation based on attack type
				player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_RANGED));
				player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
			}
		} break;
		default: {
			// do nothing
		} break;
	}
}

void Player::setPathLine(Vec2<int> dest) {
	path_line = getPath(*combat, dest);
}

std::vector<ScreenCoord> Player::getPossibleMoves() {
	std::vector<std::vector<ScreenCoord>> open;
	std::vector<ScreenCoord> seen;

	std::vector<ScreenCoord> root;
	root.push_back(position);

	open.push_back(root);
	while (!(open.empty())) {
		std::vector<ScreenCoord> n = open[0];
		open.erase(open.begin());

		if (n.size() <= static_cast<unsigned int>(getMoveSpeed() + 1)) {
			//continue;

			ScreenCoord end_position = n.back();
			if (std::find(seen.begin(), seen.end(), end_position) != seen.end()) {
				continue;
			}

			seen.push_back(end_position);

			std::vector<ScreenCoord> successors = getValidNeighbours(end_position, *combat);
			for (ScreenCoord succ : successors) {
				std::vector<ScreenCoord> s(n);
				s.push_back(succ);
				open.push_back(s);
			}
		}
	}

	return seen;
}

void Player::updatePossibleMoves()
{
	possibleMoves = getPossibleMoves();
}

void Player::takeDamageCallback(int damage) {
	player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::TAKE_DAMAGE));
	// Decide what the next animation is based on whether the player is still alive or not
	if (health <= 0) player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::DEAD));
	else player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
}

void Player::selectCallback() {
	player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::SELECTED));
}

void Player::init() {
	// TODO: not sure why this is broken??? -> player rendering not centered
	player_sprite.setSize(sprite_width, sprite_height);
	player_sprite.setSourceSize(96, 96);

	player_sprite.addAnimation(1, 1);		// IDLE
	player_sprite.addAnimation(0, 0);		// SELECTED
	player_sprite.addAnimation(2, 16);		// ATK MELEE
	player_sprite.addAnimation(17, 31);		// ATK RANGED	
	player_sprite.addAnimation(32, 34);		// TAKE DAMAGE
	player_sprite.addAnimation(35, 35);		// DEAD
}
