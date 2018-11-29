#pragma once
#include <unordered_map>

// File JSON handling
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../engine/particleSystem.hpp"

// The path to the location where the attack data is stored
#define PARTICLE_FILE_LOCATION	"res/data/particles.json"

// Utility structs
struct EmitterData {
	int angle;
	int spread;
	int max_y;
	int lifespan;
	int spawnrate;
	bool burst;
	int speed;
	int particle_lifespan;
	int spawn_radius;
	bool gravity;
	
	// Source sprite data
	int source_x;
	int source_y;
	int source_w;
	int source_h;
};

/*	A utility class that loads all particle data and stores it in a static container
		- The class is stored as a singleton, it can and should never be instantiated by any other method

	TODO:
		- Handle load failures better
*/
class ParticleLoader {

public:

	// Accessor method for singleton pattern
	static ParticleLoader& get_instance() {
		static ParticleLoader loader;
		return loader;
	}

	// Delete copy and move constructors and assign operators
	ParticleLoader(ParticleLoader const&) = delete;             // Copy construct
	ParticleLoader(ParticleLoader&&) = delete;                  // Move construct
	ParticleLoader& operator=(ParticleLoader const&) = delete;  // Copy assign
	ParticleLoader& operator=(ParticleLoader&&) = delete;		// Move assign

	// The central get function to get the attack data and associate it with a unit
	Emitter * get(const std::string& name, int x, int y);
	Emitter * get(const std::string& name, int x, int y, int angle);

protected:
	// Protected constructor/destructor for singleton pattern
	ParticleLoader();
	~ParticleLoader();
private:

	// Utility functions to help load the file data
	void loadParticles();
	void loadParticle(const json& data);

	// The actual storage of the attacks
	std::unordered_map<std::string, EmitterData> emitters;

};

// Utility functions to interact with the particle loader more easily
namespace Particles {
	inline Emitter * get(const std::string& name, int x, int y) {
		return ParticleLoader::get_instance().get(name, x , y);
	}
	inline Emitter * get(const std::string& name, int angle, int x, int y) {
		return ParticleLoader::get_instance().get(name, x, y, angle);
	}
}