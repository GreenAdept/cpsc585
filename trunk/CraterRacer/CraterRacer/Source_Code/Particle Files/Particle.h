#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Constants.h"

class Particle {
public:
	Particle();

	bool update(float deltaTime);

	Vec3 position, velocity, acceleration;
	float life, size;
	//D3DXCOLOR startColour, endColour;
	D3DXCOLOR colour;

private:
	float timeElapsed;
};

#endif PARTICLE_H