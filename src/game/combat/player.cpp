#include "player.hpp"

#include "../combat.hpp"
#include "../util/attackloader.hpp"
#include "../util/util.hpp"

Player::Player(int x, int y, const nlohmann::json& data) :
	Unit(UnitType::PLAYER),
	current_action(PlayerAction::NONE),
	player_state(PlayerState::CHOOSING),
	player_sprite("res/assets/players/FemaleSheet.png", 96, 96),
	valid_tile("res/assets/tiles/valid.png"),
	valid_move("res/assets/tiles/valid_move.png", 32, 32)
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
	if (player_state == PlayerState::ATTACKING) {
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
}

void Player::render()
{
	// Render the actual player
	player_sprite.setPos(screenPosition.x(), screenPosition.y());
	player_sprite.render();
}

void Player::renderTop(Combat * combat) {
	// If it's the players turn, render player related UI
	if (selected && state == UnitState::IDLE && player_state == PlayerState::CHOOSING) {
		renderTurnUI();
	}
	renderHealth();
}

void Player::renderTurnUI() {
	
	// Setup the variables to draw the UI correctly
	ScreenCoord pos = screenPosition;
	int option_height = SubDiv::vSize(16, 1);
	if (pos.x() < Core::windowWidth() / 2) {
		pos.x() += unit_width + (sprite_width - unit_width) / 2;
	} else {
		pos.x() += (sprite_width - unit_width) / 2 - SubDiv::hSize(5, 1);
	}

	// If the turn UI is off the screen, fix it
	if (pos.y() < 0) pos.y() = 0;
	if (pos.y() + option_height * 6 > Core::windowHeight()) pos.y() = Core::windowHeight() - 6 * option_height;

	Colour base = Colour(.7f, .7f, .7f);
	Colour select = Colour(.9f, .9f, .9f);

	// The actual drawing of the UI elements
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
	Core::Renderer::drawRect(pos, SubDiv::hSize(5, 1), option_height, current_action == PlayerAction::PASS ? select : base);
	Core::Text_Renderer::render("PASS", pos + text_offset, 1.f);
}

void Player::renderValidMoves() {

	// Render the base grid of valid moves first
	valid_tile.setSize(tile_width, tile_height);
	for (ScreenCoord pos : possibleMoves) {
		valid_tile.setPos((pos.x()) * tile_width, (pos.y()) * tile_height);
		valid_tile.render();
	}

	// Then render the path to the cursor
	bool first = true;
	valid_move.setSize(tile_width, tile_height);
	for (ScreenCoord pos : path_line) {
		if (first) {
			first = false;
			continue;
		}
		valid_move.setPos(pos.x() * tile_width, pos.y() * tile_height);
		valid_move.renderWithoutUpdate();
	}
	valid_move.updateFrame();
}

void Player::handleEvent(const SDL_Event & event)
{
	// Only handle events for the entity if it is selected
	if (selected && state == UnitState::IDLE) {
		if (event.type == SDL_KEYDOWN) {
			if (player_state == PlayerState::CHOOSING) {
				// Move Key
				if (event.key.keysym.sym == SDLK_KP_1 || event.key.keysym.sym == SDLK_1) {
					if (!moved) {
						if (current_action == PlayerAction::MOVE) player_state = PlayerState::ATTACKING;
						current_action = PlayerAction::MOVE;
						updatePossibleMoves();
					}
				}
				// Attack 1 key
				if (event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP_2) {
					if (current_action == PlayerAction::ATTACK_1) player_state = PlayerState::ATTACKING;
					current_action = PlayerAction::ATTACK_1;
				}
				// Attack 2 key
				if (event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_KP_3) {
					if (current_action == PlayerAction::ATTACK_2) player_state = PlayerState::ATTACKING;
					current_action = PlayerAction::ATTACK_2;
				}
				// Attack 3 key
				if (event.key.keysym.sym == SDLK_4 || event.key.keysym.sym == SDLK_KP_4) {
					if (current_action == PlayerAction::ATTACK_3) player_state = PlayerState::ATTACKING;
					current_action = PlayerAction::ATTACK_3;
				}
				// Attack 4 key
				if (event.key.keysym.sym == SDLK_5 || event.key.keysym.sym == SDLK_KP_5) {
					if (current_action == PlayerAction::ATTACK_4) player_state = PlayerState::ATTACKING;
					current_action = PlayerAction::ATTACK_4;
				}
				// Pass the turn
				if (event.key.keysym.sym == SDLK_KP_6 || event.key.keysym.sym == SDLK_6) {
					if (current_action == PlayerAction::PASS) execute(Vec2<int>());
					current_action = PlayerAction::PASS;
				}
				// Next attack option
				if (event.key.keysym.sym == SDLK_DOWN) {
					switch (current_action) {
					case PlayerAction::NONE: {
						if (moved) {
							current_action = PlayerAction::ATTACK_1;
						} else {
							current_action = PlayerAction::MOVE;
							updatePossibleMoves();
						}
					} break;
					case PlayerAction::MOVE:		current_action = PlayerAction::ATTACK_1;	break;
					case PlayerAction::ATTACK_1:	current_action = PlayerAction::ATTACK_2;	break;
					case PlayerAction::ATTACK_2:	current_action = PlayerAction::ATTACK_3;	break;
					case PlayerAction::ATTACK_3:	current_action = PlayerAction::ATTACK_4;	break;
					case PlayerAction::ATTACK_4:	current_action = PlayerAction::PASS;		break;
					case PlayerAction::PASS:		current_action = PlayerAction::PASS;		break;
					default:						current_action = PlayerAction::NONE;
					}
				}
				// Previous attack option
				if (event.key.keysym.sym == SDLK_UP) {
					switch (current_action) {
					case PlayerAction::NONE:		current_action = PlayerAction::MOVE;		break;
					case PlayerAction::PASS:		current_action = PlayerAction::ATTACK_4;	break;
					case PlayerAction::ATTACK_4:	current_action = PlayerAction::ATTACK_3;	break;
					case PlayerAction::ATTACK_3:	current_action = PlayerAction::ATTACK_2;	break;
					case PlayerAction::ATTACK_2:	current_action = PlayerAction::ATTACK_1;	break;
					case PlayerAction::ATTACK_1:
					case PlayerAction::MOVE: {
						// ATTACK_1 AND MOVE both fallthrough to move
						if (moved) {
							current_action = PlayerAction::ATTACK_1;
						} else {
							current_action = PlayerAction::MOVE;
							updatePossibleMoves();
						}
					} break;
					default:						current_action = PlayerAction::NONE;
					}
				}
				if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE) {
					if (current_action == PlayerAction::PASS) {
						execute(Vec2<int>());
						player_state = PlayerState::CHOOSING;
					} else {
						if (current_action != PlayerAction::NONE) {
							player_state = PlayerState::ATTACKING;
						}
					}
				}
				return;
			}
			if (player_state == PlayerState::ATTACKING) {
				if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE) {
					int gridX, gridY;
					SDL_GetMouseState(&gridX, &gridY);
					gridX = gridX / tile_width;
					gridY = gridY / tile_height;
					execute(Vec2<int>(gridX, gridY));
					player_state = PlayerState::CHOOSING;
				}
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					player_state = PlayerState::CHOOSING;
				}
				return;
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (player_state == PlayerState::CHOOSING) {
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				if (getActionAtCoord(ScreenCoord(mouseX, mouseY)) != PlayerAction::NONE) {
					player_state = PlayerState::ATTACKING;
					if (current_action == PlayerAction::MOVE) updatePossibleMoves();
				}
				if (current_action == PlayerAction::PASS) {
					execute(Vec2<int>());
					player_state = PlayerState::CHOOSING;
				}
				return;
			}
			if (player_state == PlayerState::ATTACKING) {
				int gridX, gridY;
				SDL_GetMouseState(&gridX, &gridY);
				gridX = gridX / tile_width;
				gridY = gridY / tile_height;
				execute(Vec2<int>(gridX, gridY));
				player_state = PlayerState::CHOOSING;
				return;
			}
		}
	}
}

void Player::update(int delta) {
	// Update the current action based on the mouse position
	if (selected) {	
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		PlayerAction action = getActionAtCoord(ScreenCoord(mouseX, mouseY));
		if (state == UnitState::IDLE && player_state == PlayerState::CHOOSING && action != PlayerAction::NONE) {
			current_action = action;
		}
	}
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
					if (moved) {
						state = UnitState::DONE;
					} else {
						moved = true;
					}
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
			} else {
				incrementCounter();
			}
		} break;
		case UnitState::DONE: {
			
		}
		default: {
			// Do nothing by default
		} break;
	}
}

void Player::execute(Vec2<int> to)
{
	if (state != UnitState::IDLE) return;
	switch (current_action) {
		case PlayerAction::NONE: {
			// do nothing
		} break;
		case PlayerAction::MOVE: {
			// Move the player using the base unit move function
			move(*combat, to);
			current_action = PlayerAction::NONE;
		} break;
			// TODO: determine if an attack is valid, and don't execute the attack if it isn't
		case PlayerAction::ATTACK_1: {
			if (attack1.isValid(to, *combat)) {
				current_action = PlayerAction::NONE;
				state = UnitState::ATTACK;
				startCounter();
				// TODO: Play animation based on attack type
				player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_MELEE));
				player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
				attack1.attack(to, *combat);
			}
		} break;
		case PlayerAction::ATTACK_2: {
			if (attack2.isValid(to, *combat)) {
				current_action = PlayerAction::NONE;
				state = UnitState::ATTACK;
				startCounter();
				// TODO: Play animation based on attack type
				player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_RANGED));
				player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
				attack2.attack(to, *combat);
			}
		} break;
		case PlayerAction::ATTACK_3: {
			if (attack3.isValid(to, *combat)) {
				current_action = PlayerAction::NONE;
				state = UnitState::ATTACK;
				startCounter();
				// TODO: Play animation based on attack type
				player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_RANGED));
				player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
				attack3.attack(to, *combat);
			}
		} break;
		case PlayerAction::ATTACK_4: {
			if (attack4.isValid(to, *combat)) {
				current_action = PlayerAction::NONE;
				state = UnitState::ATTACK;
				startCounter();
				// TODO: Play animation based on attack type
				player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::ATTACK_RANGED));
				player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
				attack4.attack(to, *combat);
			}
		} break;
		case PlayerAction::PASS: {
			current_action = PlayerAction::NONE;
			state = UnitState::DONE;
			player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
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

void Player::setTileSizeCallback(int width, int height) {
	// TODO: include these in file metadata as well
	float width_to_tile = 1.3f;
	float sprite_ratio = 1.8f;
	// Calculate the sprite size based on the width/height
	float width_ratio = static_cast<float>(96 / PLAYER_WIDTH_IN_SOURCE);
	sprite_width = static_cast<int>(width_ratio * width_to_tile * width);
	float height_ratio = static_cast<float>(96 / PLAYER_HEIGHT_IN_SOURCE);
	sprite_height = static_cast<int>(height_ratio * width * sprite_ratio);
	player_sprite.setSize(sprite_width, sprite_height);
	calculateScreenPosition();

	// Also set the units height
	unit_width = static_cast<int>(PLAYER_WIDTH_IN_SOURCE * static_cast<float>(sprite_width) / 96.f);
	unit_height = static_cast<int>(PLAYER_HEIGHT_IN_SOURCE * static_cast<float>(sprite_height) / 96.f);
}

void Player::takeDamageCallback(int damage) {
	player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::TAKE_DAMAGE));
	// Decide what the next animation is based on whether the player is still alive or not
	if (health <= 0) player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::DEAD));
	else player_sprite.queueAnimation(static_cast<unsigned int>(PlayerAnim::IDLE));
}

void Player::selectCallback() {
	moved = false;
	player_sprite.playAnimation(static_cast<unsigned int>(PlayerAnim::SELECTED));
}

void Player::init() {
	player_sprite.setSize(sprite_width, sprite_height);
	player_sprite.setSourceSize(96, 96);

	player_sprite.addAnimation(1, 1);		// IDLE
	player_sprite.addAnimation(0, 0);		// SELECTED
	player_sprite.addAnimation(2, 16);		// ATK MELEE
	player_sprite.addAnimation(17, 31);		// ATK RANGED	
	player_sprite.addAnimation(32, 34);		// TAKE DAMAGE
	player_sprite.addAnimation(35, 35);		// DEAD

	valid_move.setSourceSize(32, 32);
	valid_move.addAnimation(0, 5);
	valid_move.playAnimation(0);
}

PlayerAction Player::getActionAtCoord(ScreenCoord coord) {
	// TODO: Separate this into member variables/functions since a lot of data is shared with the rendering of options
	int option_height = SubDiv::vSize(16, 1);
	ScreenCoord pos = screenPosition;
	if (pos.x() < Core::windowWidth() / 2) {
		pos.x() += unit_width + (sprite_width - unit_width) / 2;
	} else {
		pos.x() += (sprite_width - unit_width) / 2 - SubDiv::hSize(5, 1);
	}

	// If the turn UI is off the screen, fix it
	if (pos.y() < 0) pos.y() = 0;
	// Check move option collision if the player hasn't moved
	if (!moved) {
		if (coord.x() > pos.x() && coord.x() < pos.x() + SubDiv::hSize(5, 1) && coord.y() > pos.y() && coord.y() < pos.y() + option_height) {
			return PlayerAction::MOVE;
		}
	}
	pos.y() += option_height;
	// Render the attacks
	if (coord.x() > pos.x() && coord.x() < pos.x() + SubDiv::hSize(5, 1) && coord.y() > pos.y() && coord.y() < pos.y() + option_height)
		return PlayerAction::ATTACK_1;
	pos.y() += option_height;
	if (coord.x() > pos.x() && coord.x() < pos.x() + SubDiv::hSize(5, 1) && coord.y() > pos.y() && coord.y() < pos.y() + option_height)
		return PlayerAction::ATTACK_2;
	pos.y() += option_height;
	if (coord.x() > pos.x() && coord.x() < pos.x() + SubDiv::hSize(5, 1) && coord.y() > pos.y() && coord.y() < pos.y() + option_height)
		return PlayerAction::ATTACK_3;
	pos.y() += option_height;
	if (coord.x() > pos.x() && coord.x() < pos.x() + SubDiv::hSize(5, 1) && coord.y() > pos.y() && coord.y() < pos.y() + option_height)
		return PlayerAction::ATTACK_4;
	pos.y() += option_height;
	// Render the pass option
	if (coord.x() > pos.x() && coord.x() < pos.x() + SubDiv::hSize(5, 1) && coord.y() > pos.y() && coord.y() < pos.y() + option_height)
		return PlayerAction::PASS;
	return PlayerAction::NONE;
}
