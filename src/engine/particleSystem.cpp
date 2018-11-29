#include "particleSystem.hpp"

ParticleSystem::ParticleSystem() :
	sprite("res/assets/tiles/particles.png")
{
	sprite.setSize(30, 30);
}

void ParticleSystem::render()
{
	for (Emitter * e : emitters) {
		e->render(sprite);
	}
}

void ParticleSystem::update()
{
	for (std::vector<Emitter *>::iterator it = emitters.begin(); it != emitters.end();) {

		(*it)->update();
		if ((*it)->counter2 > (*it)->lifespan && (*it)->particles.empty()) {
			Emitter * toDelete = (*it);
			it = emitters.erase(it);
			delete toDelete;
		}

		else {
			it++;
		}
	}
}

void ParticleSystem::addEmitter(Emitter * newEmitter)
{
	emitters.push_back(newEmitter);
}


Emitter::Emitter(int x, int y, int angle, int spread, int maxY, int lifespan, int spawnrate, bool burst, int speed, int pLifeSpan, int spawnRadius, bool gravity) :
	position(ScreenCoord(x, y)),
	angle(angle),
	spread(spread),
	maxY(maxY),
	lifespan(lifespan),
	spawnRate(spawnrate),
	burst(burst),
	speed(speed),
	pLifeSpan(pLifeSpan),
	spawnRadius(spawnRadius),
	gravity(gravity)
{
	if (burst) {
		for (int i = 0; i < spawnRate; i++) {
			ScreenCoord origin;
			int pAngle;
			int pMaxY;

			if (spawnRadius == 0) {
				origin = position;
			}
			else {
				origin = position + ScreenCoord(rand() % (spawnRadius * 2) - spawnRadius, rand() % (spawnRadius * 2) - spawnRadius);
			}

			if (spread == 0) {
				pAngle = angle;
			}
			else {
				pAngle = angle + (rand() % (spread * 2) - spread);
			}

			if (maxY == 0) {
				pMaxY = 0;
			}
			else {
				pMaxY = origin.y() + maxY + (rand() % (maxY / 2) - (maxY / 4));
			}
			int pSpeed = speed + (rand() % speed - (speed / 2));
			Particle * newParticle = new Particle(origin, pAngle, pMaxY, pSpeed, pLifeSpan, gravity);
			particles.push_back(newParticle);
		}
	}
}

void Emitter::render(Sprite s)
{
	for (Particle * p : particles) {
		p->render(s);
	}
}

void Emitter::update()
{
	for (std::vector<Particle *>::iterator it = particles.begin(); it != particles.end();) {
		(*it)->update();
		if ((*it)->counter > (*it)->lifespan) {
			Particle * toDelete = (*it);
			it = particles.erase(it);
			delete toDelete;
		}

		else {
			it++;
		}
	}
	// Regardless of whether the particle was burst or not, create more particles
	counter++;
	if (counter > spawnRate && counter2 < lifespan) {
		counter = 0;
		ScreenCoord origin;
		int pAngle;
		int pMaxY;

		if (spawnRadius == 0) {
			origin = position;
		}
		else {
			origin = position + ScreenCoord(rand() % (spawnRadius * 2) - spawnRadius, rand() % (spawnRadius * 2) - spawnRadius);
		}

		if (spread == 0) {
			pAngle = angle;
		}
		else {
			pAngle = angle + (rand() % (spread * 2) - spread);
		}

		if (maxY == 0) {
			pMaxY = 0;
		}
		else {
			pMaxY = origin.y() + maxY + (rand() % (maxY / 2) - (maxY / 4));
		}

		int pSpeed = speed + (rand() % speed - (speed / 2));
		Particle * newParticle = new Particle(origin, pAngle, pMaxY, pSpeed, pLifeSpan, gravity);
		newParticle->SetSprite(sourceX, sourceY, sourceW, sourceH);
		particles.push_back(newParticle);
	}
	counter2++;
}

void Emitter::SetSprite(int spx, int spy, int ssw, int ssh)
{
	sourceX = spx;
	sourceY = spy;
	sourceW = ssw;
	sourceH = ssh;

	for (Particle * p : particles) {
		p->SetSprite(spx, spy, ssw, ssh);
	}
}

void Particle::SetSprite(int spx, int spy, int ssw, int ssh)
{
	sourceX = spx;
	sourceY = spy;
	sourceW = ssw;
	sourceH = ssh;
}


Particle::Particle(ScreenCoord origin, int angle, int maxY, int speed, int lifespan, bool gravity) :
	position(origin),
	approxX(origin.x()),
	approxY(origin.y()),
	maxY(maxY),
	speed(speed),
	lifespan(lifespan),
	gravity(gravity)
{
	double rad = angle * (M_PI / 180);
	velocity = Vec2<double>(sin(rad), -cos(rad));
	counter = 0;
}

void Particle::render(Sprite s)
{
	s.setPos(position.x(), position.y());
	s.setSourcePos(sourceX, sourceY);
	s.setSourceSize(sourceW, sourceH);
	//s.setSize(100, 100);
	s.render();
}

void Particle::update()
{
	if (!gravity) {
		counter++;
		approxX += velocity.x()*speed;
		approxY += velocity.y()*speed;

		position.x() = static_cast<int>(floor(approxX));
		position.y() = static_cast<int>(floor(approxY));
	}
	else {
		if (position.y() < maxY) {
			approxX += velocity.x()*speed;
			approxY += velocity.y()*speed;

			velocity += G;

			position.x() = static_cast<int>(floor(approxX));
			position.y() = static_cast<int>(floor(approxY));
		}
		else {
			counter++;
		}
	}
}
