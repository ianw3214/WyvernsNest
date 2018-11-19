#pragma once
#include <unordered_map>

// File JSON handling
#include <nlohmann/json.hpp>
using json = nlohmann::json;


#include "../combat/attack.hpp"
class Unit;

// The path to the location where the attack data is stored
#define ATTACK_FILE_LOCATION	"res/data/attacks.json"

/*	A utility class that loads all attack data and stores it in a static container
		- The class is stored as a singleton, it can and should never be instantiated by any other method

	TODO:
		- Handle load failures better
*/
class AttackLoader {

public:

	// Accessor method for singleton pattern
	static AttackLoader& get_instance() {
		static AttackLoader loader;
		return loader;
	}

	// Delete copy and move constructors and assign operators
	AttackLoader(AttackLoader const&) = delete;             // Copy construct
	AttackLoader(AttackLoader&&) = delete;                  // Move construct
	AttackLoader& operator=(AttackLoader const&) = delete;  // Copy assign
	AttackLoader& operator=(AttackLoader&&) = delete;		// Move assign

	// The central get function to get the attack data and associate it with a unit
	Attack get(const std::string& name, Unit * unit);

protected:
	// Protected constructor/destructor for singleton pattern
	AttackLoader();
	~AttackLoader();
private:

	// Utility functions to help load the file data
	void loadAttacks();
	bool loadAttack(const json& data);
	AttackType getTypeFromString(const std::string& str) const;
	AttackEffect * parseEffect(const json& data) const;
	EffectModifier parseModifier(const json& data) const;

	// The actual storage of the attacks
	std::unordered_map<std::string, Attack> attacks;

};

// Utility functions to interact with the attack loader more easily
namespace Attacks {
	inline Attack get(const std::string& name, Unit * unit) {
		return AttackLoader::get_instance().get(name, unit);
	}
}