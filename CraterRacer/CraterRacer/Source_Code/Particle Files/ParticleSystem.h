#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "ParticleEmitter.h"
#include "Constants.h"

#include <vector>

using namespace std;

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();
	
	void update(float deltaTime);
	void addEmitter(Vec3 pos);
	vector<Particle*> getParticles();

private:
	vector<ParticleEmitter> emitters;
	vector<Particle*> particles;

};

#endif PARTICLESYSTEM_H