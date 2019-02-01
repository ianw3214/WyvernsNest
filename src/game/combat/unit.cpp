#include "unit.hpp"

#include "../util/attackloader.hpp"
#include "../combat.hpp"

// TODO: Design better constructors
Unit::Unit(UnitType type) :
	type(type),
	state(UnitState::IDLE),
	sprite_width(DEFAULT_SPRITE_WIDTH),
	sprite_height(DEFAULT_SPRITE_HEIGHT),
	top_margin(0),
	shadow("res/assets/shadow.png"),
	unit_width(-1),
	unit_height(-1)
{
	generateDefaultUnitData();
	loadPropertiesFromUnitData();
}

int Unit::getStat(Stat stat) const {
	if (stat == Stat::STR) return getSTR();
	if (stat == Stat::DEX) return getDEX();
	if (stat == Stat::INT) return getINT();
	if (stat == Stat::CON) return getCON();
	return 0;
}

int Unit::getSTR() const {
	float percent_modifier = 1.f;
	for (const Status * status : statusList) {
		percent_modifier += status->getStatModifier(Stat::STR);
	}
	return static_cast<int>(static_cast<float>(data.strength) * percent_modifier);
}

int Unit::getDEX() const {
	float percent_modifier = 1.f;
	for (const Status * status : statusList) {
		percent_modifier += status->getStatModifier(Stat::DEX);
	}
	return static_cast<int>(static_cast<float>(data.dexterity) * percent_modifier);
}

int Unit::getINT() const {
	float percent_modifier = 1.f;
	for (const Status * status : statusList) {
		percent_modifier += status->getStatModifier(Stat::INT);
	}
	return static_cast<int>(static_cast<float>(data.intelligence) * percent_modifier);
}

int Unit::getCON() const {
	float percent_modifier = 1.f;
	for (const Status * status : statusList) {
		percent_modifier += status->getStatModifier(Stat::CON);
	}
	return static_cast<int>(static_cast<float>(data.constitution) * percent_modifier);
}

void Unit::setTileSize(int width, int height) {
	tile_width = width;
	tile_height = height;
	// Set the size of common sprites
	shadow.setSize(width, height / 2);
	// Recalculate the screen position based on the tile size
	calculateScreenPosition();

	// Call the tile size callback
	setTileSizeCallback(width, height);
}

void Unit::setTopMargin(int margin) {
	top_margin = margin;
}

std::vector<ScreenCoord> Unit::getPath(Combat & combat, ScreenCoord to) {
	std::vector<std::vector<ScreenCoord>> open;

	std::vector<ScreenCoord> root;
	root.push_back(position);

	// Check to see if the target position is valid first
	if (!combat.grid.isPosValid(to)) return std::vector<ScreenCoord>();

	open.push_back(root);
	while (!(open.empty())) {
		std::vector<ScreenCoord> n = heuristic(&open);
		std::vector<std::vector<ScreenCoord>>::iterator index = std::find(open.begin(), open.end(), n);
		open.erase(index);

		if (n.size() <= static_cast<unsigned int>(getMoveSpeed() + 1)) {

			ScreenCoord end_position = n.back();
			if (end_position.x() == to.x() && end_position.y() == to.y()) {
				//n is solution
				return n;
			}

			std::vector<ScreenCoord> successors = getValidNeighbours(end_position, combat);
			for (ScreenCoord succ : successors) {
				std::vector<ScreenCoord> s(n);
				s.push_back(succ);
				open.push_back(s);
			}
		}
	}

	// No solution found
	std::vector<ScreenCoord> result;
	return result;
}

std::vector<ScreenCoord> Unit::heuristic(std::vector<std::vector<ScreenCoord>>* open) {
	std::vector<ScreenCoord> smallest;
	int smallest_dist = 99999;
	for (std::vector<ScreenCoord> node : *open) {
		ScreenCoord end_position = node.back();
		int x_diff = std::abs(end_position.x() - moveTarget.x());
		int y_diff = std::abs(end_position.y() - moveTarget.y());
		ScreenCoord v(x_diff, y_diff);
		if (v.norm() < smallest_dist) {
			smallest_dist = v.norm();
			smallest = node;
		}
	}
	return smallest;
}

std::vector<ScreenCoord> Unit::getValidNeighbours(ScreenCoord pos, Combat & combat) {
	std::vector<ScreenCoord> neighbours;
	ScreenCoord right(pos.x() + 1, pos.y());
	ScreenCoord left(pos.x() - 1, pos.y());
	ScreenCoord top(pos.x(), pos.y() - 1);
	ScreenCoord bot(pos.x(), pos.y() + 1);

	if (pos.x() > 0) {
		if (combat.isPosEmpty(left)) {
			neighbours.push_back(left);
		}
	}

	if (pos.x() < combat.grid.map_width) {
		if (combat.isPosEmpty(right)) {
			neighbours.push_back(right);
		}
	}

	if (pos.y() > 0) {
		if (combat.isPosEmpty(bot)) {
			neighbours.push_back(bot);
		}
	}

	if (pos.y() < combat.grid.map_height) {
		if (combat.isPosEmpty(top)) {
			neighbours.push_back(top);
		}
	}
	return neighbours;
}

void Unit::calculateScreenPositionMovement() {
	screenPosition.x() += moveNext.x() * tile_width / 12;
	screenPosition.y() += moveNext.y() * tile_height / 12;

	// Make the shadow move during movement
	shadow.setPos(screenPosition.x() - (tile_width - sprite_width) / 2, screenPosition.y() - tile_height / 2 + sprite_height);
}

void Unit::incrementMovement() {
	position += moveNext;

	//nothing left
	if (path.size() <= 0) return;

	ScreenCoord next = path[0];
	path.erase(path.begin());

	moveNext = ScreenCoord(next.x() - position.x(), next.y() - position.y());

	calculateScreenPosition();
}

void Unit::calculateScreenPosition() {
	screenPosition.x() = position.x() * tile_width;
	screenPosition.y() = position.y() * tile_height;
	screenPosition.x() += (tile_width - sprite_width) / 2;
	screenPosition.y() -= sprite_height - tile_height;
	// Calculate shadow positions
	shadow.setPos(position.x() * tile_width, position.y() * tile_height + tile_height / 2);
}

// Not implemented in base unit, should be implemented in specialized classes
void Unit::setTileSizeCallback(int width, int height) {}
void Unit::takeDamageCallback(int damage) {}
void Unit::selectCallback() {}

void Unit::generateDefaultUnitData() {
	// Default name
	data.name = "BOB";
	// Default stats
	data.strength = 10;
	data.dexterity = 10;
	data.intelligence = 10;
	data.constitution = 10;
	// Default traits -> NOT YET IMPLEMENTED
}

void Unit::addStatus(Status * status) {
	statusList.push_back(status);
	status->setTarget(this);
	// The status may affect unit stats, so recalculate utility variables
	loadPropertiesFromUnitData(false);
}

void Unit::select() {
	selected = true;
	selectCallback();
	std::vector<int> removeIndex;
	removeIndex.reserve(statusList.size());
	for (unsigned int i = 0; i < statusList.size(); ++i) {
		if (!(statusList[i]->tick())) {
			removeIndex.push_back(i);
		}
	}
	for (int i : removeIndex) {
		statusList.erase(statusList.begin() + i);
	}
	// Recalculate the stats in case anything was removed
	loadPropertiesFromUnitData(false);
}

void Unit::deselect() {
	selected = false;
}

void Unit::takeDamage(int damage) {
	// Call the virtualized callback function for subclasses to customize
	takeDamageCallback(damage);
	health -= damage;
	if (health <= 0) {
		health = 0;
		state = UnitState::DEAD;
	}
}

void Unit::heal(int health) {
	this->health += health;
	if (this->health > maxHealth) {
		this->health = maxHealth;
	}
}

void Unit::push(int p, ScreenCoord src_pos) {
	Vec2<int> temp_pos;

	if (src_pos[0] - position[0] > 0) {
		for (int i = p; i >= 0; i--) {
			temp_pos = position - Vec2<int>(1 * p, 0);
			if (combat->isPosEmpty(temp_pos)) {
				position = temp_pos;
				calculateScreenPosition();
				break;
			}
		}
	}
	else if (src_pos[0] - position[0] < 0) {
		for (int i = p; i >= 0; i--) {
			temp_pos = position - Vec2<int>(-1 * p, 0);
			if (combat->isPosEmpty(temp_pos)) {
				position = temp_pos;
				calculateScreenPosition();
				break;
			}
		}
	}
	else if (src_pos[1] - position[1] > 0) {
		for (int i = p; i >= 0; i--) {
			temp_pos = position - Vec2<int>(0, 1 * p);
			if (combat->isPosEmpty(temp_pos)) {
				position = temp_pos;
				calculateScreenPosition();
				break;
			}
		}
	}
	else if (src_pos[1] - position[1] < 0) {
		for (int i = p; i >= 0; i--) {
			temp_pos = position - Vec2<int>(0, -1 * p);
			if (combat->isPosEmpty(temp_pos)) {
				position = temp_pos;
				calculateScreenPosition();
				break;
			}
		}
	}
}

bool Unit::move(Combat & combat, Vec2<int> pos) {
	// Only move the player to empty positions
	if (combat.isPosEmpty(pos)) {
		// Also check if the movement is valid first
		int steps = std::abs(pos.x() - position.x()) + std::abs(pos.y() - position.y());
		if (steps <= getMoveSpeed()) {
			moveTarget = pos;
			path = getPath(combat, moveTarget);
			if (path.size() > 0) {
				moveNext = ScreenCoord(0, 0);
				incrementMovement();
				state = UnitState::MOVE;
				startCounter();
				return true;
			}
		}
	}
	return false;
}

void Unit::renderBottom(Combat * combat) {
	// For now, just render the shadow of the unit on the bottom
	shadow.render();
}

void Unit::renderTop(Combat * combat) {
	// Let the unit subclasses handle this one
}

void Unit::renderHealth() {
	// ScreenCoord pos = screenPosition + ScreenCoord((tile_width - sprite_width) / 2, (tile_height - sprite_height) / 2);
	ScreenCoord pos = screenPosition;
	if (unit_height > 0) {
		pos.y() += sprite_height - unit_height;
	}
	pos.x() += (sprite_width - tile_width) / 2;
	if (pos.y() < 0) pos.y() = 10;
	int healthBarWidth = tile_width;
	int tick = lerp(0, healthBarWidth, static_cast<float>(health) / static_cast<float>(maxHealth));
	// TODO: Use rectangle rendering (implement in engine)
	for (int i = 0; i < 10; ++i) {
		if (pos.y() > 0) {
			Core::Renderer::drawLine(pos + ScreenCoord(0, i), pos + ScreenCoord(tick, i), Colour(0.0f, 1.0f, 0.0f));
			Core::Renderer::drawLine(pos + ScreenCoord(tick, i), pos + ScreenCoord(healthBarWidth, i), Colour(1.0f, 0.0f, 0.0f));
		}
		else {
			Core::Renderer::drawLine(pos + ScreenCoord(0, -i), pos + ScreenCoord(tick, -i), Colour(0.0f, 1.0f, 0.0f));
			Core::Renderer::drawLine(pos + ScreenCoord(tick, -i), pos + ScreenCoord(healthBarWidth, -i), Colour(1.0f, 0.0f, 0.0f));
		}
	}
	Core::Renderer::drawLine(pos + ScreenCoord(0, 0), pos + ScreenCoord(healthBarWidth, 0), Colour(0.0f, 0.0f, 0.0f));
	if (pos.y() <= 0) {
		Core::Renderer::drawLine(pos + ScreenCoord(0, 0), pos + ScreenCoord(0, -10), Colour(0.0f, 0.0f, 0.0f));
		Core::Renderer::drawLine(pos + ScreenCoord(healthBarWidth, 0), pos + ScreenCoord(healthBarWidth, -10), Colour(0.0f, 0.0f, 0.0f));
		Core::Renderer::drawLine(pos + ScreenCoord(0, -10), pos + ScreenCoord(healthBarWidth, -10), Colour(0.0f, 0.0f, 0.0f));
	}
	else {
		Core::Renderer::drawLine(pos + ScreenCoord(0, 0), pos + ScreenCoord(0, 10), Colour(0.0f, 0.0f, 0.0f));
		Core::Renderer::drawLine(pos + ScreenCoord(healthBarWidth, 0), pos + ScreenCoord(healthBarWidth, 10), Colour(0.0f, 0.0f, 0.0f));
		Core::Renderer::drawLine(pos + ScreenCoord(0, 10), pos + ScreenCoord(healthBarWidth, 10), Colour(0.0f, 0.0f, 0.0f));
	}
}

void Unit::loadPropertiesFromUnitData(bool resetHealth) {
	// The health of the unit depends on it's constitution
	maxHealth = data.constitution * 2;
	if (resetHealth) {
		health = getCON() * 2;
		if (health > maxHealth) health = maxHealth;
	}
	// The movement speed in terms of grid units of the unit
	move_speed = getDEX() / 5 + 1;
	// TEMPORARY HARD CAP, CHANGE THIS BY OPTIMIZING IN THE FUTURE
	// TODO: Profile pathfinding code to find bottleneck
	if (move_speed > 4) move_speed = 4;
}
