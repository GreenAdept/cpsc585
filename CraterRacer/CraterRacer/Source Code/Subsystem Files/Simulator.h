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
#include "DebugWriter.h"

#define PI					3.14159265

#include <iostream>
using namespace std;

class Simulator 
{

public:

	Simulator();
	~Simulator();
	
	void InitNx( Mesh* terrainMesh );

	bool pause( bool );
	bool isPaused( );

	void simulate(vector<Vehicle*> entities, double elapsedTime);
	void createVehicle( Vec3 pos, BoundingBox b );
	void createVehicle( Mesh* mesh, Vec3 pos, BoundingBox b  );

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

	void addTerrainFromX( Mesh* mesh, NxVec3 pos );

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

	//Time 
	double				m_dDeltaTime;
	//bool*				m_bInputs;
	//bool*				m_bButtons;

	//Actors
	NxActor*			m_GroundPlane;
	vector< NxActor* >	m_Vehicles;
	vector< NxActor* >  m_Wheels;
	NxActor*			m_Terrain;

	//Debugging
	DebugWriter			m_Debugger;
	bool				forward;
	bool				m_bSuspension;

};

#endif SIMULATOR_H
