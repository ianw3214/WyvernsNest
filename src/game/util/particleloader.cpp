#include "particleloader.hpp"
#pragma once

#ifdef _DEBUG
#include <iostream>
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif

#include <fstream>

Emitter * ParticleLoader::get(const std::string & name, int x, int y) {
	if (emitters.find(name) == emitters.end()) return nullptr;
	Emitter * emitter = new Emitter(x, y,
		emitters[name].angle,
		emitters[name].spread,
		emitters[name].max_y,
		emitters[name].lifespan,
		emitters[name].spawnrate,
		emitters[name].burst,
		emitters[name].speed,
		emitters[name].particle_lifespan,
		emitters[name].spawn_radius,
		emitters[name].gravity);
	emitter->SetSprite(emitters[name].source_x, emitters[name].source_y, emitters[name].source_w, emitters[name].source_h);
	return emitter;
}

Emitter * ParticleLoader::get(const std::string & name, int x, int y, int angle) {
	if (emitters.find(name) == emitters.end()) return nullptr;
	Emitter * emitter = new Emitter(x, y,
		angle,
		emitters[name].spread,
		emitters[name].max_y,
		emitters[name].lifespan,
		emitters[name].spawnrate,
		emitters[name].burst,
		emitters[name].speed,
		emitters[name].particle_lifespan,
		emitters[name].spawn_radius,
		emitters[name].gravity);
	emitter->SetSprite(emitters[name].source_x, emitters[name].source_y, emitters[name].source_w, emitters[name].source_h);
	return emitter;
}

ParticleLoader::ParticleLoader() {
	loadParticles();
}

ParticleLoader::~ParticleLoader() {

}

void ParticleLoader::loadParticles() {
	std::ifstream inp(PARTICLE_FILE_LOCATION);
	// Use the provided overloaded operators to load the json data
	json data;
	inp >> data;

	// Load all the attacks individually
	for (const json& particle : data["particles"]) {
		loadParticle(particle);
	}
}

void ParticleLoader::loadParticle(const json & data) {
	std::string name = data["name"];
	emitters[name] = EmitterData();
	emitters[name].angle = data["angle"];
	emitters[name].spread = data["spread"];
	emitters[name].max_y = data["max_y"];
	emitters[name].lifespan = data["lifespan"];
	emitters[name].spawnrate = data["spawnrate"];
	emitters[name].burst = data["burst"];
	emitters[name].speed = data["speed"];
	emitters[name].particle_lifespan = data["particle_lifespan"];
	emitters[name].spawn_radius = data["spawn_radius"];
	emitters[name].gravity = data["gravity"];

	emitters[name].source_x = data["image_data"]["x"];
	emitters[name].source_y = data["image_data"]["y"];
	emitters[name].source_w = data["image_data"]["width"];
	emitters[name].source_h = data["image_data"]["height"];
}
