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
#include "Crater.h"
#include "Terrain.h"
#include "DebugWriter.h"
#include "Sound.h"
#include "ContactReport.h"
#include "UserData.h"

#define PI						3.14159265
#define MAX_FORWARD_VELOCITY	70.0
#define MAX_BACKWARD_VELOCITY	40.0
#define MAX_OFFTRACK_VELOCITY	15.0
#define	MIN_VELOCITY			0.5

#include <iostream>
using namespace std;

class Simulator 
{

public:

	Simulator();
	~Simulator();
	
	void InitNx( vector<Terrain*> terrain );

	void pause( bool pause );
	bool isPaused( );

	void simulate( vector<Vehicle*> vehicles, vector<MeteorGroup*> meteorGroups, double elapsedTime );
	void createVehicle( Vehicle* vehicle );
	void createMeteorGroup( MeteorGroup *mg );
	void createCrater( Crater* crater );
	void simulateMeteorGroup( MeteorGroup* mg, double time, vector<Vehicle*> vehicles );
	Vec3 findTerrainNormal( Vec3 v );
	void removeFromSimulation( Entity* entity );
	void addRamps( vector<Mesh*> meshes );

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
	void setSteeringPower(double steeringPower);
	void setBrakingFriction(double friction);
	void setVehicleMass(double mass);

	void setActorGroup(NxActor *actor, NxCollisionGroup group);

	void printVariables();
	void setSuspensionFlag(bool flag);
	void startOrStopRace(bool race);
	float findAngle(NxVec3 a, NxVec3 b);

	void setContacts();
	void setGroupFlags();

private:

	void addTerrainFromX( Terrain* terrain, int id  );

	NxActor* createLittleBox( NxVec3 pos );

	void startPhysics(void);
	void getPhysicsResults(void);
	void respawn(NxActor* actor, Vehicle* vehicle);
	void processForceKeys(NxActor* actor, Vehicle* vehicle, int index, double time);
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
	NxReal				m_rBrakingFriction;
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
	NxReal				m_rRampForceConstant;

	//Time 
	double				m_dDeltaTime;

	//Actors
	vector<NxActor*>	m_Actors;
	vector<NxActor*>	m_Vehicles;

	//Debugging
	DebugWriter			m_Debugger;
	bool				forward;
	bool				m_bStartRace;

	ContactReport*		m_ContactReport;

};

#endif SIMULATOR_H
