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

	void	initialize( Device* device, LPCWSTR filename, Vec3 pt, LPCWSTR effectFilename );
	void	update( Matrix mat );

	float	getDiameter		 ( );
	float	getWheelWidth	 ( );

	void	setAngle		 ( float angle );
	float	getAngle		 ( );

	float	getDisplacement	 ( );
	void	setDisplacement	 ( float );

	NxVec3	getChassisPt	 ( );
	void	setChassisPt	 ( Vec3 pt );

	Vec3	getSuspensionAxis( );
	void	setSuspensionAxis( NxVec3 sus );
	void	setWheelLateral	 ( NxVec3 lateral);
	NxVec3	getWheelLateral	 ( );

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
	NxVec3		m_vWheelLateral;

};

#endif //WHEEL_H