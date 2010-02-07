// ===============================================================================
//						  NVIDIA PHYSX SDK TRAINING PROGRAMS
//						     LESSON 101 : PRIMARY SHAPE
//
//						    Written by QA BJ, 6-2-2008
// ===============================================================================

#pragma once
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include "Constants.h"

#include "NxPhysics.h"
#include "NxCooking.h"
#include "Stream.h"
#include "Vehicle.h"
#include "DebugWriter.h"

#define MAX_KEYS 256

#include <iostream>
using namespace std;

class Simulator 
{

public:

	Simulator();
	~Simulator();

	void simulate(vector<Vehicle*> entities, double elapsedTime);
	void addActor( Mesh* mesh, Vec3& pos );
	void createVehicle( Vec3 pos, BoundingBox b );

private:

	void InitNx();

	NxVec3* getVertsFromDXMesh( Mesh* mesh );
	NxActor* createMeshActor( Mesh* mesh,  Vec3& pos );
	NxActor* createGroundPlane(void);
	void startPhysics(void);
	void getPhysicsResults(void);
	void processForceKeys(void);
	void processForceKeys(NxActor* actor, Vehicle* vehicle);
	NxVec3 applyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength);
	
	// Physics SDK 
	NxPhysicsSDK*		m_PhysicsSDK;
	NxScene*			m_Scene;
	NxVec3				m_vDefaultGravity;

	//Force 
	NxVec3				m_vForceVec;
	NxReal				m_rForceStrength;
	bool				m_bForceMode;

	//Friction
	NxReal				m_rRestitution;
	NxReal				m_rStaticFriction;
	NxReal				m_rDynamicFriction;

	//Time 
	double				m_dDeltaTime;
	bool*				m_bInputs;

	//Actors
	NxActor*			m_GroundPlane;
	vector< NxActor* >	m_Vehicles;

	Vec3				m_vP1Dir;

	//Debugging
	DebugWriter			m_Debugger;
	bool				forward;

};

#endif SIMULATOR_H
