#pragma once
#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Particle.h"
#include "Constants.h"

#include <vector>

class ParticleEmitter {
public:
	ParticleEmitter();
	~ParticleEmitter();
	
	void update(float deltaTime);

private:
	Vec3 position;
	float timeElapsed, life, timeBetweenParticles, radius;
	int maxParticles;
	vector<Particle> particles;

};

#endif PARTICLEEMITTER_H