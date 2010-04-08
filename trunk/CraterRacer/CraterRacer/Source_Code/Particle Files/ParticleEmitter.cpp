#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {}

ParticleEmitter::ParticleEmitter(Vec3 pos) {
	position = pos;
	particlesAdded = 0;
	timeElapsed = 0;
	maxParticles = 5000;
	life = 10;
	timeBetweenParticles = 0.001;
	radius = 1;
}

ParticleEmitter::~ParticleEmitter() { 
	for (int i = 0; i < particles.size(); i++) {
		delete particles[i];
	}
}

void ParticleEmitter::update(float deltaTime) {
	//update the particles
	for (int i = 0; i < particles.size(); i++) {
		//if the particle is dead, delete it
		if(!particles[i]->update(deltaTime)) {
			particles.erase(particles.begin()+i);
			i--;
		}
	}

	//add new particles
	while ((particlesAdded < (timeElapsed/timeBetweenParticles)) && (particles.size() < maxParticles)) {
		
	}
}