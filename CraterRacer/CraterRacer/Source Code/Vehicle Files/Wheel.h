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

	void  initialize( Device* device, LPWSTR filename, Vec3 pt );
	void   update( Matrix mat );

	float getDiameter( );
	float getAngle();
	float getDisplacement( );
	void  setDisplacement( float );

	NxVec3 getChassisPt( );
	void   setChassisPt( Vec3 pt );

	float  getWheelWidth( );
	Vec3   getSuspensionAxis( );
	void   setSuspensionAxis( NxVec3 sus );



private:
	NxMaterial*	m_mMaterial;

	//Friction
	NxReal		m_rRestitution;
	NxReal		m_rStaticFriction;
	NxReal		m_rDynamicFriction;

	float		m_fAngle;			//degrees
	float		m_fDiameter;		//meters
	float		m_fDisplacement;	//meters
	NxVec3		m_vChassisPt;
	NxVec3	    m_vSuspensionAxis;

};

#endif //WHEEL_H