#pragma once
#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Particle.h"
#include "Constants.h"

#include <vector>

using namespace std;

class ParticleEmitter {
public:
	ParticleEmitter();
	ParticleEmitter(Vec3 pos);
	~ParticleEmitter();
	
	void update(float deltaTime);
	vector<Particle*> getParticles();

private:
	Vec3 position;
	float timeElapsed, life, timeBetweenParticles, radius;
	int maxParticles, particlesAdded;
	vector<Particle*> particles;

};

#endif PARTICLEEMITTER_H