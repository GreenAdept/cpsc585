#pragma once
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <math.h>
#include "Constants.h"

#include "NxPhysics.h"
#include "NxCooking.h"
#include "Stream.h"
#include "Vehicle.h"
#include "Meteor.h"
#include "MeteorGroup.h"
#include "Terrain.h"
#include "DebugWriter.h"

#define PI					3.14159265
#define MAX_VELOCITY		55.0

#include <iostream>
using namespace std;

class Simulator 
{

public:

	Simulator();
	~Simulator();
	
	void InitNx( Terrain* terrain );

	bool pause( bool pause );
	bool isPaused( );

	void simulate( vector<Vehicle*> vehicles, vector<MeteorGroup*> meteorGroups, double elapsedTime );
	void createVehicle( Vehicle* vehicle );
	void createMeteorGroup(MeteorGroup *mg);
	void removeFromSimulation( Entity* entity );

	NxTriangleMeshShapeDesc createTriMeshShape( Mesh* mesh );

	void setForceVec(Vec3 vec);
	void setForceStr(double str);
	void setForceMode(bool mode);
	void setGravity(Vec3 gravity);
	void setRestitution(double res);
	void setSFriction(double friction);
	void setDFriction(double friction);
	void setMaxAngularVelocity(double maxAngle);
	void setMaxWheelAngle(double maxAngle);
	void setSpringScale(double scale);
	void setDamperScale(double scale);

	void printVariables();
	void setSuspensionFlag(bool flag);

private:

	void addTerrainFromX( Terrain* terrain );

	NxActor* createLittleBox( NxVec3 pos );

	void startPhysics(void);
	void getPhysicsResults(void);
	void processForceKeys(NxActor* actor, Vehicle* vehicle);
	NxVec3 normalize(NxVec3 vec);
	NxVec3 rotate(NxVec3 lateral, float angle);
	
	// Physics SDK 
	NxPhysicsSDK*		m_PhysicsSDK;
	NxScene*			m_Scene;
	NxVec3				m_vDefaultGravity;
	bool				m_bPaused;

	//Force 
	NxVec3				m_vForceVec;
	NxReal				m_rForceStrength;
	bool				m_bForceMode;

	//Friction
	NxReal				m_rRestitution;
	NxReal				m_rStaticFriction;
	NxReal				m_rDynamicFriction;
	NxReal				m_rMaxAngularVelocity;
	NxReal				m_rVehicleMass;
	NxReal				m_rSpringK;
	NxReal				m_rSpringC;
	NxReal				m_rMaxWheelDisplacement;
	NxReal				m_rMaxWheelAngle;
	NxReal				m_rWheelRestLength;
	NxReal				m_rMinWheelDisplacement;
	NxReal				m_rDamperScale;
	NxReal				m_rSpringScale;
	NxReal				m_rSteeringPower;

	//Time 
	double				m_dDeltaTime;
	//bool*				m_bInputs;
	//bool*				m_bButtons;

	//Actors
	vector<NxActor*>	m_Actors;

	//Debugging
	DebugWriter			m_Debugger;
	bool				forward;
	bool				m_bSuspension;

};

#endif SIMULATOR_H