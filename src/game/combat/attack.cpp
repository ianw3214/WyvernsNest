#include "attack.hpp"

#include "../combat.hpp"
#include "unit.hpp"
#include "status.hpp"
#include "../util/particleloader.hpp"

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
	
}

#include <iostream>
Attack::Attack(const Attack & other, Unit * source) :
	name(other.name),
	source(source),
	type(other.type),
	range(other.range),
	effect(other.effect),
	aoe(other.aoe),
	affect_self(other.affect_self),
	effectModifiers(other.effectModifiers),
	particles(other.particles),
	validSprite("res/assets/tiles/valid.png"),
	targetValidSprite("res/assets/tiles/valid_circle.png"),
	targetInvalidSprite("res/assets/tiles/invalid_circle.png")
{

}

#include <iostream>
void Attack::attack(ScreenCoord pos, Combat& combat) {
	if (isValid(pos, combat)) {
		switch (type) {
		case AttackType::SELF: {
			if (affect_self) {
				effect->attack(source->position, combat, *this);
			}
			attackAoE(source->position, combat);
		} break;
		case AttackType::MELEE: {
			effect->attack(pos, combat, *this);
			attackAoE(pos, combat);
		} break;
		case AttackType::RANGED: {
			effect->attack(pos, combat, *this);
			attackAoE(pos, combat);
		} break;
		case AttackType::PIERCE: {
			// TODO: Make peircing attack based on range
			Vec2<int> step = pos - source->position;
			for (int i = 0; i < range; ++i) {
				effect->attack(pos + step * i, combat, *this);
			}
		} break;
		}
	}
	// Apply the particles
	for (ParticleData& particle : particles) {
		if (particle.position == ParticlePosition::TARGET) {
			// Set the target to the center of the target
			int x = pos.x() * combat.grid.tile_width + combat.grid.tile_width / 2;
			int y = pos.y() * combat.grid.tile_height + combat.grid.tile_height / 2;
			int angle;
			if (pos.y() < source->position.y()) angle = 0;
			if (pos.y() > source->position.y()) angle = 180;
			if (pos.x() < source->position.x()) angle = 270;
			if (pos.x() > source->position.x()) angle = 90;
			combat.addEmitter(Particles::get(particle.name, angle, x, y));
		}
		if (particle.position == ParticlePosition::SELF) {
			combat.addEmitter(Particles::get(
				particle.name,
				source->screenPosition.x() + source->getSpriteWidth() / 2,
				source->screenPosition.y() + source->getSpriteHeight() - source->getTileHeight())
			);
		}
	}
}

void Attack::addEffectModifier(EffectModifier modifier) {
	effectModifiers.push_back(modifier);
}

void Attack::addEffectModifier(Stat stat, float multiplier) {
	effectModifiers.emplace_back(stat, multiplier);
}

void Attack::addParticle(std::string name, ParticlePosition pos) {
	particles.push_back(ParticleData{ name, pos });
}

// Display the valid attack tiles on the grid
void Attack::renderValidGrid(int tile_width, int tile_height, const Combat& combat) {

	// Set the tile width/height before rendering
	validSprite.setSize(tile_width, tile_height);

	switch (type) {
	case AttackType::SELF: {
		renderValidSprite(tile_width, tile_height, source->position.x(), source->position.y(), combat);
	} break;
	case AttackType::MELEE: {
		renderValidSprite(tile_width, tile_height, source->position.x() - 1, source->position.y(), combat);
		renderValidSprite(tile_width, tile_height, source->position.x() + 1, source->position.y(), combat);
		renderValidSprite(tile_width, tile_height, source->position.x(), source->position.y() + 1, combat);
		renderValidSprite(tile_width, tile_height, source->position.x(), source->position.y() - 1, combat);
	} break;
	case AttackType::RANGED: {
		for (int y = -range; y < range + 1 ; y++) {
			for (int x = -range; x < range + 1 ; x++) {
				if (!(y == x && y == 0)) {
					if (abs(x) + abs(y) <= range) {
						renderValidSprite(tile_width, tile_height, source->position.x() + x, source->position.y() + y, combat);
					}
				}
			}
		}
	} break;
	case AttackType::PIERCE: {
		renderValidSprite(tile_width, tile_height, source->position.x() - 1, source->position.y(), combat);
		renderValidSprite(tile_width, tile_height, source->position.x() + 1, source->position.y(), combat);
		renderValidSprite(tile_width, tile_height, source->position.x() , source->position.y() + 1, combat);
		renderValidSprite(tile_width, tile_height, source->position.x() , source->position.y() - 1, combat);
	} break;
	}

	// Mouse rendering
	renderValidTarget(tile_width, tile_height, combat);
	
}

void Attack::renderValidTarget(int tile_width, int tile_height, const Combat& combat) {

	// Set the tile width/height before rendering
	validSprite.setSize(tile_width, tile_height);
	targetValidSprite.setSize(tile_width, tile_height);
	targetInvalidSprite.setSize(tile_width, tile_height);

	// Get the current mouse position in the game
	int mouseX; int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int x = static_cast<int>(floor(mouseX / tile_width));
	int y = static_cast<int>(floor(mouseY / tile_height));
	
	// Render the valid target based on whether it IS valid or not
	switch (type) {
	case AttackType::MELEE: {
		if (isValid(Vec2<int>(x, y), combat)) {
			targetValidSprite.setPos(x * tile_width, y * tile_height);
			targetValidSprite.render();	
		}
		else {
			targetInvalidSprite.setPos(x * tile_width, y * tile_height);
			targetInvalidSprite.render();
		}
	} break;
	case AttackType::RANGED: {
		if (isValid(Vec2<int>(x, y), combat)) {
			targetValidSprite.setPos(x * tile_width, y * tile_height);
			targetValidSprite.render();
		}
		else {
			targetInvalidSprite.setPos(x * tile_width, y * tile_height);
			targetInvalidSprite.render();
		}
	} break;
	case AttackType::PIERCE: {
		if (isValid(Vec2<int>(x, y), combat)) {
			targetValidSprite.setPos(x * tile_width, y * tile_height);
			targetValidSprite.render();
			Vec2<int> step = Vec2<int>(x, y) - source->position;
			for (int i = 0; i < range; ++i) {
				validSprite.setPos((x + step.x() * i) * tile_width, (y + step.y() * i) * tile_height);
				validSprite.render();
			}
		}
		else {
			targetInvalidSprite.setPos(x * tile_width, y * tile_height);
			targetInvalidSprite.render();
		}
	} break;
	}
}

void Attack::renderValidSprite(int tile_width, int tile_height, int x, int y, const Combat & combat) {
	if (combat.grid.isPosValid(Vec2<int>(x, y))) {
		validSprite.setPos(x * tile_width, y * tile_height);
		validSprite.render();
	}
}

bool Attack::isValid(ScreenCoord pos, const Combat& combat) {
	switch (type) {
	case AttackType::SELF: {
		return pos == source->position;
	} break;
	case AttackType::MELEE: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff == 1) return combat.grid.isPosValid(pos);
		return false;
	} break;
	case AttackType::RANGED: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff <= range) return combat.grid.isPosValid(pos);
		return false;
	} break;
	case AttackType::PIERCE: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff == 1) return combat.grid.isPosValid(pos);
		return false;
	} break;
	}
	return false;
}

void Attack::attackAoE(ScreenCoord pos, Combat & combat) {
	for (int i = -aoe; i <= aoe; ++i) {
		int height = aoe - std::abs(i);
		for (int j = -height; j <= height; ++j) {
			// Apply the attack effect on grid cell [i, j]
			if (!(Vec2<int>(i, j) == Vec2<int>(0, 0))) {
				effect->attack(source->position + Vec2<int>(i, j), combat, *this);
			}
		}
	}
}

void DamageEffect::attack(ScreenCoord pos, Combat & combat, const Attack& attack) {
	Unit * unit = combat.getUnitAt(pos);
	if (unit) {
		int damage = base_damage;
		for (EffectModifier modifier : attack.getEffectModifiers()) {
			damage += static_cast<int>(attack.getSource()->getStat(modifier.stat) * modifier.modifier);
		}
		unit->takeDamage(damage);
	}
}

void HealEffect::attack(ScreenCoord pos, Combat & combat, const Attack& attack) {
	Unit * unit = combat.getUnitAt(pos);
	if (unit) {
		int healing = heal;
		for (EffectModifier modifier : attack.getEffectModifiers()) {
			healing += static_cast<int>(attack.getSource()->getStat(modifier.stat) * modifier.modifier);
		}
		unit->heal(healing);
	}
}

void BurnEffect::attack(ScreenCoord pos, Combat& combat, const Attack& attack) {
	Unit * unit = combat.getUnitAt(pos);
	if (unit) {
		// TODO: Add modifiers (or don't)
		unit->addStatus(new BurnStatus(burn_damage, ticks, infinite, unit));
	}
}

void StatBuffEffect::attack(ScreenCoord pos, Combat& combat, const Attack& attack) {
	Unit * unit = combat.getUnitAt(pos);
	if (unit) {
		// TODO: Add modifiers (or don't)
		unit->addStatus(new StatBuffStatus(stat, percent, ticks, infinite, unit));
	}
}

void PushEffect::attack(ScreenCoord pos, Combat &combat, const Attack &attack) {
	Unit *unit = combat.getUnitAt(pos);
	if (unit) {
		unit->push(distance, attack.getSource()->position);
	}
}

// TODO: Have an attack allow an enemy to move an arbitrary number of units maybe? -> Not sure if good design choice
void MoveEffect::attack(ScreenCoord pos, Combat & combat, const Attack & attack) {
	if (combat.isPosEmpty(pos)) {
		// This is really bad, do not do this in the future
		Unit * unit = const_cast<Unit*>(attack.getSource());
		if (!unit->move(combat, pos)) {
			
		}
	}
}

// TODO: Have an attack allow an enemy to move an arbitrary number of units maybe? -> Not sure if good design choice
void BlinkEffect::attack(ScreenCoord pos, Combat & combat, const Attack & attack) {
	if (combat.isPosEmpty(pos)) {
		// This is really bad, do not do this in the future
		Unit * unit = const_cast<Unit*>(attack.getSource());
		unit->position = pos;
		unit->calculateScreenPosition();
	}
}

// TODO: Have an attack allow an enemy to move an arbitrary number of units maybe? -> Not sure if good design choice
void ResurrectEffect::attack(ScreenCoord pos, Combat & combat, const Attack & attack) {
	if (combat.isPosEmpty(pos)) {
		// This is really bad, do not do this in the future
		Unit * unit = const_cast<Unit*>(attack.getSource());
		if (unit->getState() == UnitState::DEAD) {
			unit->health = 1;
			unit->setState(UnitState::IDLE);
			// TODO: Update animations
		}
	}
}