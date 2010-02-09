#pragma once
#ifndef WHEEL_H
#define WHEEL_H


#include "NxPhysics.h"
#include "Entity.h"

//---------------------------------------------------------
//		CLASS: Wheel
//
//		Each vehicle has 4 wheels.
//---------------------------------------------------------

class Wheel: public Entity
{

public:
	Wheel();
	~Wheel();

	void  initialize( Device* device, LPWSTR filename );
	float getDiameter();
	float getAngle();

private:
	NxMaterial*	m_mMaterial;

	//Friction
	NxReal		m_rRestitution;
	NxReal		m_rStaticFriction;
	NxReal		m_rDynamicFriction;

	float		m_fAngle;		//degrees
	float		m_fDiameter;	//meters
};

#endif //WHEEL_H