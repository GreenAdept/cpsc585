#pragma once
#ifndef WHEEL_H
#define WHEEL_H


#include "NxPhysics.h"

//---------------------------------------------------------
//		CLASS: Wheel
//
//		Each vehicle has 4 wheels.
//---------------------------------------------------------

class Wheel 
{

public:
	Wheel();
	~Wheel();

	float getDiameter();
	float getAngle();

private:
	NxMaterial*	m_mMaterial;

	//Friction
	NxReal		m_rRestitution;
	NxReal		m_rStaticFriction;
	NxReal		m_rDynamicFriction;

	float		m_fAngle; //degrees
	float		m_fDiameter; //meters
};

#endif //WHEEL_H