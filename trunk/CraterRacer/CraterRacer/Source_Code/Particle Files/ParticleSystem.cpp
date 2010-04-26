#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {}

ParticleSystem::~ParticleSystem() {
	for (int i = 0; i < particles.size(); i++) {
		delete particles[i];
	}
}

void ParticleSystem::addEmitter(Vec3 pos) {
	emitters.push_back(ParticleEmitter(pos));
}

void ParticleSystem::update(float deltaTime) {
	for (int i = 0; i < emitters.size(); i++) {
		emitters[i].update(deltaTime);
	}
}

vector<Particle*> ParticleSystem::getParticles() {
	particles.clear();
	for (int i = 0; i < emitters.size(); i++) {
		vector<Particle*> temp = emitters[i].getParticles();
		for (int j = 0; j < temp.size(); j++) {
			particles.push_back(temp[j]);
		}
	}

	return particles;
}