#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Colour.h"
#include "Constants.h"

class Particle {
public:
	Particle();

	bool update(float deltaTime);

	Vec3 position, velocity, acceleration;
	float life, radius;
	Colour startColour, endColour;

private:
	float timeElapsed;
	Colour colour;
};

#endif PARTICLE_H