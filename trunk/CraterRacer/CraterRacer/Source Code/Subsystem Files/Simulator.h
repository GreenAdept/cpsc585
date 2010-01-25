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
#include "Entity.h"

#include <iostream>

using namespace std;

class Simulator 
{

public:

	Simulator();
	~Simulator();

	void simulate(vector<Entity*> entities, double elapsedTime);
	void addActor( Mesh* mesh, Vec3& pos );

private:

	void InitNx();

	NxVec3* getVertsFromDXMesh( Mesh* mesh );
	NxActor* createMeshActor( Mesh* mesh,  Vec3& pos );
	NxActor* createBox(Vec3 pos, double size);
	NxActor* createGroundPlane(void);
	void startPhysics(void);
	void getPhysicsResults(void);
	void processInput(void);
	void processForceKeys(void);
	NxVec3 applyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength);
};

/*#include "DrawObjects.h"
#include "HUD.h"

#include "DebugRenderer.h"
#include "CommonCode.h"*/

/*void PrintControls();
void ProcessCameraKeys();
void SetupCamera();
void RenderActors(bool shadows);
void DrawForce(NxActor* actor, NxVec3& forceVec, const NxVec3& color);

NxVec3 ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength);
void ProcessForceKeys();
void ProcessInputs();
void SelectNextActor();
bool IsSelectable(NxActor* actor);

void RenderCallback();
void ReshapeCallback(int width, int height);
void IdleCallback();
void KeyboardCallback(unsigned char key, int x, int y);
void KeyboardUpCallback(unsigned char key, int x, int y);
void SpecialCallback(int key, int x, int y);
void MouseCallback(int button, int state, int x, int y);
void MotionCallback(int x, int y);
void ExitCallback();
void InitGlut(int argc, char** argv);

NxActor* CreateGroundPlane();
NxActor* CreateBox();
NxActor* CreateSphere();
NxActor* CreateCapsule();
NxActor* CreateBounds3();
NxActor* CreateSegment();
NxActor* CreateRay();

//void InitializeHUD();

void InitNx();
void ReleaseNx();
void ResetNx();

void StartPhysics();
void GetPhysicsResults();*/

//int main(int argc, char** argv);

#endif SIMULATOR_H
