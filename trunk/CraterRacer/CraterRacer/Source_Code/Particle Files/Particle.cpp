#include "Particle.h"

Particle::Particle() {
	timeElapsed = 0;
}

bool Particle::update(float deltaTime) {
	if (timeElapsed >= life) {
		return false; //particle needs to be deleted
	}

	//update the life time
	timeElapsed += deltaTime;

	//update the position
	position += velocity * deltaTime;

	//update the velocity
	velocity += acceleration * deltaTime;

	//update the colour
	float ratio = timeElapsed/life;
	colour = (startColour * (1 - ratio)) + (endColour * ratio);

	return true;
}