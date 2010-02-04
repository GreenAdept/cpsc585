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
#include "NxWheel.h"
#include "NxWheelDesc.h"
#include "Stream.h"
#include "Vehicle.h"
#include "DebugWriter.h"

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
	void processInput(void);
	void processForceKeys(void);
	NxVec3 applyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength);
};

#endif SIMULATOR_H
