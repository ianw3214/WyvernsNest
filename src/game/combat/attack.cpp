#include "attack.hpp"

#include "unit.hpp"
#include "../combat.hpp"

// Construct an invalid attack by default
Attack::Attack() :
	name("INVALID"),
	source(nullptr),
	type(AttackType::INVALID),
	range(0),
	effect(nullptr),
	aoe(0),
	affect_self(false),
	validSprite("res/assets/valid.png"),
	targetValidSprite("res/assets/valid_circle.png"),
	targetInvalidSprite("res/assets/invalid_circle.png")
{

}

Attack::Attack(std::string name,
	Unit * source,
	AttackType type,
	AttackRange range,
	AttackEffect * effect,
	AttackAoE aoe,
	bool affect_self)
	:
	name(name),
	source(source),
	type(type),
	range(range),
	effect(effect),
	aoe(aoe),
	affect_self(affect_self),
	validSprite("res/assets/tiles/valid.png"),
	targetValidSprite("res/assets/tiles/valid_circle.png"),
	targetInvalidSprite("res/assets/tiles/invalid_circle.png")
{
	if (type == AttackType::HEAL) {
		dynamic_cast<DamageEffect*>(effect)->damage = -1;
	}
}

Attack::Attack(const Attack & other, Unit * source) :
	name(other.name),
	source(source),
	type(other.type),
	range(other.range),
	effect(other.effect),
	aoe(other.aoe),
	affect_self(other.affect_self),
	validSprite("res/assets/tiles/valid.png"),
	targetValidSprite("res/assets/tiles/valid_circle.png"),
	targetInvalidSprite("res/assets/tiles/invalid_circle.png")
{

}

void Attack::attack(ScreenCoord pos, Combat& combat) {
	if (isValid(pos)) {


		switch (type) {
		case AttackType::SELF: {
			if (affect_self) {
				effect->attack(source->position, combat, source->getINT(), 1.0);
			}
			attackAoE(source->position, combat);
		} break;
		case AttackType::MELEE: {
			// TODO: Make sure the attack is valid before running it
			effect->attack(pos, combat, source->getSTR(), 0.5);
			attackAoE(pos, combat);
		} break;
		case AttackType::RANGED: {
			// TODO: Make sure the attack is valid before running it
			effect->attack(pos, combat, source->getINT(), 1.0);
			attackAoE(pos, combat);
		} break;
		case AttackType::PIERCE: {
			effect->attack(pos, combat, source->getSTR(), 1.0);
			effect->attack(pos + pos - source->position, combat, source->getSTR(), 0.5);
		} break;
		case AttackType::SPIN: {
			effect->attack(source->position + ScreenCoord(0, 1), combat, source->getSTR(), 0.7);
			effect->attack(source->position + ScreenCoord(1, 0), combat, source->getSTR(), 0.7);
			effect->attack(source->position + ScreenCoord(0, -1), combat, source->getSTR(), 0.7);
			effect->attack(source->position + ScreenCoord(-1, 0), combat, source->getSTR(), 0.7);
		} break;
		case AttackType::HEAL: {
			// TODO: Make sure the attack is valid before running it
			effect->attack(pos, combat, source->getINT(), 2.0);
		} break;
		case AttackType::BIG_AOE: {
			// TODO: Make sure the attack is valid before running it
			effect->attack(pos, combat, source->getINT(), 2.0);
			effect->attack(pos + ScreenCoord(0, 1), combat, source->getINT(), 1.0);
			effect->attack(pos + ScreenCoord(0, -1), combat, source->getINT(), 1.0);
			effect->attack(pos + ScreenCoord(1, 0), combat, source->getINT(), 1.0);
			effect->attack(pos + ScreenCoord(-1, 0), combat, source->getINT(), 1.0);
		} break;
		}
	}
	// TODO: figure out how to apply the attack effect to the surrounding aoe
}

// Display the valid attack tiles on the grid
void Attack::renderValidGrid() {
	switch (type) {
	case AttackType::SELF: {
		// TODO: Somehow display the valid sprite
	} break;
	case AttackType::MELEE: {
		validSprite.setPos((source->position.x() - 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() - 1) * tile_height);
		validSprite.render();
		validSprite.setPos((source->position.x() + 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() + 1) * tile_height);
		validSprite.render();
	} break;
	case AttackType::RANGED: {
		for (int y = -range; y < range + 1 ; y++) {
			for (int x = -range; x < range + 1 ; x++) {
				if (!(y == x && y == 0)) {
					if (abs(x) + abs(y) <= range) {
						validSprite.setPos((source->position.x() + x) * tile_width, (source->position.y() + y) * tile_height);
						validSprite.render();
					}
				}
			}
		}
	} break;
	case AttackType::PIERCE: {
		validSprite.setPos((source->position.x() - 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() - 1) * tile_height);
		validSprite.render();
		validSprite.setPos((source->position.x() + 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() + 1) * tile_height);
		validSprite.render();
	} break;
	case AttackType::SPIN: {
		validSprite.setPos((source->position.x() - 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() - 1) * tile_height);
		validSprite.render();
		validSprite.setPos((source->position.x() + 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() + 1) * tile_height);
		validSprite.render();
	} break;
	case AttackType::HEAL: {
		validSprite.setPos((source->position.x() - 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() - 1) * tile_height);
		validSprite.render();
		validSprite.setPos((source->position.x() + 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() + 1) * tile_height);
		validSprite.render();
	} break;
	case AttackType::BIG_AOE: {
		for (int y = -range; y < range + 1; y++) {
			for (int x = -range; x < range + 1; x++) {
				if (!(y == x && y == 0)) {
					if (abs(x) + abs(y) <= range) {
						validSprite.setPos((source->position.x() + x) * tile_width, (source->position.y() + y) * tile_height);
						validSprite.render();
					}
				}
			}
		}
	} break;
	}

	//mouse rendering
	renderValidTarget();
	
}

void Attack::renderValidTarget() {
	int mouseX; int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int x = static_cast<int>(floor(mouseX / tile_width));
	int y = static_cast<int>(floor(mouseY / tile_height));



	switch (type) {
	case AttackType::MELEE: {
		if (isValid(ScreenCoord(x, y))) {
			targetValidSprite.setPos(x * tile_width, y * tile_height);
			targetValidSprite.render();
		}
		else {
			targetInvalidSprite.setPos(x * tile_width, y * tile_height);
			targetInvalidSprite.render();
		}
	} break;
	case AttackType::RANGED: {
		if (isValid(ScreenCoord(x, y))) {
			targetValidSprite.setPos(x * tile_width, y * tile_height);
			targetValidSprite.render();
		}
		else {
			targetInvalidSprite.setPos(x * tile_width, y * tile_height);
			targetInvalidSprite.render();
		}
	} break;
	case AttackType::PIERCE: {
		if (isValid(ScreenCoord(x, y))) {
			targetValidSprite.setPos(x * tile_width, y * tile_height);
			targetValidSprite.render();
			validSprite.setPos((x + x - source->position.x()) * tile_width, (y + y - source->position.y()) * tile_height);
			validSprite.render();
			targetValidSprite.setPos((x + x - source->position.x()) * tile_width, (y + y - source->position.y()) * tile_height);
			targetValidSprite.render();
		}
		else {
			targetInvalidSprite.setPos(x * tile_width, y * tile_height);
			targetInvalidSprite.render();
		}
	} break;
	case AttackType::SPIN: {
		if (isValid(ScreenCoord(x, y))) {
			targetValidSprite.setPos((source->position.x()+1) * tile_width, (source->position.y()) * tile_height);
			targetValidSprite.render();
			targetValidSprite.setPos((source->position.x() - 1) * tile_width, (source->position.y()) * tile_height);
			targetValidSprite.render();
			targetValidSprite.setPos((source->position.x()) * tile_width, (source->position.y() + 1) * tile_height);
			targetValidSprite.render();
			targetValidSprite.setPos((source->position.x()) * tile_width, (source->position.y() - 1) * tile_height);

			targetValidSprite.render();
		}
		else {
			targetInvalidSprite.setPos(x * tile_width, y * tile_height);
			targetInvalidSprite.render();
		}
	} break;
	case AttackType::HEAL: {
		if (isValid(ScreenCoord(x, y))) {
			targetValidSprite.setPos(x * tile_width, y * tile_height);
			targetValidSprite.render();
		}
		else {
			targetInvalidSprite.setPos(x * tile_width, y * tile_height);
			targetInvalidSprite.render();
		}
	} break;
	case AttackType::BIG_AOE: {
		if (isValid(ScreenCoord(x, y))) {
			targetValidSprite.setPos(x * tile_width, y * tile_height);
			targetValidSprite.render();
			targetValidSprite.setPos((x+1) * tile_width, y * tile_height);
			targetValidSprite.render();
			targetValidSprite.setPos((x-1) * tile_width, y * tile_height);
			targetValidSprite.render();
			targetValidSprite.setPos(x * tile_width, (y+1) * tile_height);
			targetValidSprite.render();
			targetValidSprite.setPos(x * tile_width, (y-1) * tile_height);
			targetValidSprite.render();
		}
		else {
			targetInvalidSprite.setPos(x * tile_width, y * tile_height);
			targetInvalidSprite.render();
		}
	} break;
	}
}

bool Attack::isValid(ScreenCoord pos) {
	switch (type) {
	case AttackType::SELF: {
		return pos == source->position;
	} break;
	case AttackType::MELEE: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff == 1) return true;
		return false;
		/*
		if ((pos.x() == source->position.x() + 1 || pos.x() == source->position.x() - 1) && pos.y() == source->position.y()) {
			return true;
		}
		else if ((pos.y() == source->position.y() + 1 || pos.y() == source->position.y() - 1) && pos.x() == source->position.x()) {
			return true;
		}
		*/
	} break;
	case AttackType::RANGED: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff <= range) return true;
		return false;
	} break;
	case AttackType::PIERCE: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff == 1) return true;
		return false;
	} break;
	case AttackType::SPIN: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff == 1) return true;
		return false;
	} break;
	case AttackType::HEAL: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff == 1) return true;
		return false;

	} break;
	case AttackType::BIG_AOE: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff <= range) return true;
		return false;
	} break;
	}
	return false;
}

void Attack::setTileSize(int width, int height) {
	tile_width = width;
	tile_height = height;
	validSprite.setSize(tile_width, tile_height);
	targetValidSprite.setSize(tile_width, tile_height);
	targetInvalidSprite.setSize(tile_width, tile_height);
}
	
void Attack::attackAoE(ScreenCoord pos, Combat & combat) {
	for (int i = -aoe; i <= aoe; ++i) {
		int height = aoe - std::abs(i);
		for (int j = -height; j <= height; ++j) {
			// Apply the attack effect on grid cell [i, j]
			if (!(Vec2<int>(i, j) == Vec2<int>(0, 0))) {
				effect->attack(source->position + Vec2<int>(i, j), combat, 0, 0.0);
			}
		}
	}
}

void DamageEffect::attack(ScreenCoord pos, Combat & combat, int stat, double mult) {
	Unit * unit = combat.getUnitAt(pos);
	if (unit) {
		unit->takeDamage(static_cast<int>(damage + stat * mult));
		// Do something if the unit dies
	}
}
