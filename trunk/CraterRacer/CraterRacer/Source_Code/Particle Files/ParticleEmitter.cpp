#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {}

ParticleEmitter::ParticleEmitter(Vec3 pos) {
	position = pos;
	particlesAdded = 0;
	timeElapsed = 0;
	maxParticles = 1;
	life = 5;
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
	/*for (int i = 0; i < particles.size(); i++) {
		//if the particle is dead, delete it
		if(!particles[i]->update(deltaTime)) {
			particles.erase(particles.begin()+i);
			i--;
		}
	}*/

	timeElapsed += deltaTime;

	//add new particles
	//Particle* particle;
	/*while ((particlesAdded < (timeElapsed/timeBetweenParticles)) && (particles.size() < maxParticles)) {
		//particle = new Particle();
		//particle->colour = D3DXCOLOR(0.3f,0.2f,0.08f,1.0f);
		//particle->position = Vec3(position.x, position.y+5, position.z);
		particles.push_back(new Particle());
	}*/

	/*if (particles.size() < 1) {
		particles.push_back(new Particle());
	}*/
}

vector<Particle*> ParticleEmitter::getParticles() {
	return particles;
}