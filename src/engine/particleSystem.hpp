#pragma once
#pragma once

#include "../engine/core.hpp"
#include <math.h>
#include <stdlib.h>


class Particle {
public:
	//Particle(ScreenCoord origin, int angle, int maxY, int speed);
	Particle(ScreenCoord origin, int angle, int maxY, int speed, int lifespan, bool gravity);

	void render(Sprite s);
	void update();
	void SetSprite(int spx, int spy, int ssw, int ssh);

	ScreenCoord position;

	double approxX;
	double approxY;

	Vec2<double> velocity;
	Vec2<double> G = Vec2<double>(0, .1);
	double speed;

	int lifespan;
	int counter;

	int maxY;

	int sourceX;
	int sourceY;
	int sourceW;
	int sourceH;

	bool gravity;

};

class Emitter {
public:
	//Emitter(int x, int y, int angle, int spread, int maxY, int lifespan, int spawnrate, bool burst);
	Emitter(int x, int y, int angle, int spread, int maxY, int lifespan, int spawnrate, bool burst, int speed, int pLifeSpan, int spawnRadius, bool gravity);

	void render(Sprite s);
	void update();

	std::vector<Particle *> particles;
	void SetSprite(int spx, int spy, int ssw, int ssh);



	ScreenCoord position;
	int angle;
	int spread;
	int maxY;
	int lifespan;
	int counter2;
	int spawnRate;
	int counter;
	bool burst;
	int speed;
	int pLifeSpan;
	int spawnRadius;

	int sourceX;
	int sourceY;
	int sourceW;
	int sourceH;

	bool gravity;

};


class ParticleSystem {
public:
	ParticleSystem();
	//~ParticleSystem();

	void render();
	void update();

	std::vector<Emitter *> emitters;
	void addEmitter(Emitter * newEmitter);

	Sprite sprite;
};

