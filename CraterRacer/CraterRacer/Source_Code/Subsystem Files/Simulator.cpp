#include "Simulator.h"
#include "MessageManager.h"

//--------------------------------------------------------------------------------------
// Function:  Constructor
// Initializes the simulator object
//--------------------------------------------------------------------------------------
Simulator::Simulator() 
{
	//initialize all simulation data members
	m_vForceVec				= NxVec3(0, 0, 0);
	m_PhysicsSDK			= NULL;
	m_Scene					= NULL;
	m_rForceStrength		= 5;
	m_bForceMode			= true;
	m_vDefaultGravity		= NxVec3(0,-10,0);
	m_rRestitution			= NxReal(0.0);
	m_rStaticFriction		= NxReal(0.4);
	m_rDynamicFriction		= NxReal(0.25);
	m_rBrakingFriction		= NxReal(0.5);
	m_rMaxAngularVelocity	= NxReal(2);
	m_rVehicleMass			= 10.0;
	m_rWheelRestLength		= 0.5;
	m_rMinWheelDisplacement = 1.25;
	m_rSpringK				= ( -m_vDefaultGravity.y * m_rVehicleMass ) / ( m_rWheelRestLength * 4 );
	m_rSpringC				= 2 * sqrt( m_rSpringK * m_rVehicleMass );
	m_rMaxWheelDisplacement = 1.0;
	m_rMaxWheelAngle		= 35.0;
	m_bPaused				= false;
	m_rSpringScale			= 20.0;
	m_rDamperScale			= 0.6;
	m_rSteeringPower		= 5;

	forward = false;
}

//--------------------------------------------------------------------------------------
// Function:  InitNx
// Initializes the PhysX engine as well as some other fundamental elements
//--------------------------------------------------------------------------------------
void Simulator::InitNx( Terrain* terrain ) 
{
	//Create the Phyics SDK
	m_PhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION );
	if( !m_PhysicsSDK ) return;

	//Create the scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = m_vDefaultGravity;
	sceneDesc.simType = NX_SIMULATION_SW;
	m_Scene = m_PhysicsSDK->createScene( sceneDesc );

	//Set the physics parameters
	m_PhysicsSDK->setParameter( NX_SKIN_WIDTH, 0.025f );

	//Set the debug visualization parameters
	m_PhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1.0);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_AXES, 1.0f);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_POINT, 1.0f);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_NORMAL, 1.0f);

	//Create the default material
	NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(m_rRestitution);
	defaultMaterial->setStaticFriction(m_rStaticFriction);
	defaultMaterial->setDynamicFriction(m_rDynamicFriction);

	//Create the ground
	addTerrainFromX( terrain );
	
	m_PhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
}

//--------------------------------------------------------------------------------------
// Function:  startPhysics
// Called the first time PhysX simulates
//--------------------------------------------------------------------------------------
void Simulator::startPhysics() 
{
	m_Scene->simulate( m_dDeltaTime );
	m_Scene->flushStream( );
}

//--------------------------------------------------------------------------------------
// Function:  getPhysicsResults
// Updates objects
//--------------------------------------------------------------------------------------
void Simulator::getPhysicsResults() 
{
	while (!m_Scene->fetchResults(NX_RIGID_BODY_FINISHED, true));
}

//--------------------------------------------------------------------------------------
// Function:  simulate
// Simulates the movement of entities based on the elapsed time
//--------------------------------------------------------------------------------------
void Simulator::simulate( vector<Vehicle*> vehicles, vector<MeteorGroup*> meteorGroups, double elapsedTime ) 
{
	if( m_bPaused )
		return;

	m_dDeltaTime = elapsedTime;

	startPhysics();
	getPhysicsResults();

	//Update all the vehicle positions based on PhysX simulated actor positions
	for( int i=0; i < vehicles.size(); i++ )
	{
		NxActor* m_Vehicle = vehicles[i]->getPhysicsObj();
		if (!m_Vehicle) continue;
		processForceKeys(m_Vehicle, vehicles[i], elapsedTime);

		//Get the new position of the vehicle in vector and matrix format
		Matrix m;
		NxVec3 vec = m_Vehicle->getGlobalPosition();
		m_Vehicle->getGlobalPose().getColumnMajor44( m );

		float height = vehicles[i]->getBoundingBox().m_fHeight;
		Matrix mat;
		D3DXMatrixIdentity( &mat );
		D3DXMatrixTranslation( &mat, 0, height/2, 0 );
		D3DXMatrixMultiply( &m, &m, &mat );

		NxVec3 vlc = m_Vehicle->getLinearVelocity();

		//Update the vehicle position in the game
		vehicles[i]->update( Vec3(vec.x, vec.y, vec.z), Vec3(vlc.x, 0, vlc.z), m );

		//set elapsed time in vehicles for speed calculation later
		vehicles[i]->setTimeElapsed( elapsedTime );
	}

	//Initialize the meteors and update all the meteor positions
	for( int i=0; i < meteorGroups.size(); i++ )
	{
		if (meteorGroups[i]->getAI()->getState() == AI::TRIGGERED) {
			//create the meteors for the group
			//createMeteorGroup(meteorGroups[i]);
		}
		else if (meteorGroups[i]->getAI()->getState() == AI::MOVING) {
			//simulate the meteors for the group
			simulateMeteorGroup(meteorGroups[i], elapsedTime);
		}
	}
}

void Simulator::respawn(NxActor* actor, Vehicle* vehicle)
{
	Vec3 respawnPoint = vehicle->lastPassedWP();
	respawnPoint.y += 5;
	Vec3 nextPoint = vehicle->nextWP();
	Vec3 difference = nextPoint - respawnPoint;
	NxVec3 diff = NxVec3(difference.x, difference.y, difference.z);
	diff = normalize(diff);

	NxMat33 reset(NxVec3(difference.z, 0, difference.x), NxVec3(0, 1, 0), NxVec3(0, 0, 1));
	actor->setLinearVelocity(NxVec3(0, 0, 0));
	actor->setAngularVelocity(NxVec3(0, 0, 0));
	actor->setGlobalOrientation(reset);
	actor->setGlobalPosition(NxVec3(respawnPoint.x, respawnPoint.y, respawnPoint.z));
}

//--------------------------------------------------------------------------------------
// Function:  processForceKeys
// Processes all of the force inputs
//--------------------------------------------------------------------------------------
void Simulator::processForceKeys(NxActor* actor, Vehicle* vehicle, double time) 
{
	NxVec3 localWheelForce[4];
	NxVec3 globalWheelForce[4];

	for (int i = 0; i < 4; i++) {
		localWheelForce[i] = globalWheelForce[i] = NxVec3(0, 0, 0);
	}
	
	Input* input = vehicle->getInputObj();
	bool* buttons = input->getButtons();
	const NxVec3 velocity = actor->getLinearVelocity();;
	NxReal friction = m_rDynamicFriction;
	boolean noInput = true;

	//INPUT
	for( int i = 0; i < 7; i++ )
	{
		if( !buttons[i] ) { continue; } 

		switch( i )
		{
			case 0: //A_BUTTON - nothing
			{
				break;
			}
			case 1: //B_BUTTON - nothing
			{
				break;
			}
			case 2: //X_BUTTON - freeze car (should be commented out when submitting)
			{
				actor->setAngularVelocity(NxVec3(0, 0, 0));
				actor->setLinearVelocity(NxVec3(0, 0, 0));
				break;
			}
			case 3: //Y_BUTTON - print waypoint for now
			{
				NxVec3 v = actor->getGlobalPosition();
				m_Debugger.writeToFile(Vec3(v.x, v.y, v.z));
				break;
			}
			case 4: //LT_BUTTON - reversing/braking
			{
				// if velocity is less than 1m/s when pressing LT_BUTTON, reverse
				bool reversing = vehicle->isReversing();
				float pressure = input->getPressure();
				g_audioState.reverse = true;
				if (reversing || (actor->getLinearVelocity().magnitude() < 1))
				{
					vehicle->setReverse(true);
					if(velocity.magnitude() < MAX_BACKWARD_VELOCITY){
						localWheelForce[2] += NxVec3(0, 0, -m_rVehicleMass * m_rForceStrength * 1.5 * (0.5+pressure) );
						localWheelForce[3] += NxVec3(0, 0, -m_rVehicleMass * m_rForceStrength * 1.5 * (0.5+pressure) );
					}
					noInput = false;
					break;
				}
				//else brake
				else {
					friction += m_rBrakingFriction * pressure;
					break;
				}
			}
			case 5: //RT_BUTTON - accelerating
			{
				//if reversing, brake
				float pressure = input->getPressure();
				if (vehicle->isReversing())
				{
					friction = friction + m_rBrakingFriction * pressure;
					vehicle->setReverse(false);
					g_audioState.reverse = -1;
					break;
				}
				//else, accelerate
				if(velocity.magnitude() < MAX_FORWARD_VELOCITY){
					localWheelForce[2] += NxVec3(0, 0, m_rVehicleMass * m_rForceStrength * (0.5+pressure) );
					localWheelForce[3] += NxVec3(0, 0, m_rVehicleMass * m_rForceStrength * (0.5+pressure) );
				}
				noInput = false;
				g_audioState.acceleration = 1;
				break; 
			}
			case 6: //BACK_BUTTON - respawn
			{
				respawn(actor, vehicle);
				noInput = false;
				break;
			}
			default:
			{
				g_audioState.acceleration = 0;
			}
		}
	}

	Wheel*	w; 
	NxVec3	wheelPos,
			upVec,
			wheelForward,
			carForward,
			susAxis;
	float	displacement,
			susForce;
    NxMat34 mat = actor->getGlobalPose();
	NxMat33 orient = actor->getGlobalOrientation();
	BoundingBox BB = vehicle->getBoundingBox();

	//Friction (dynamic friction and breaking)
	if (velocity.magnitude() > 1) 
	{
		NxVec3 velocityDir = normalize(velocity);
	
		NxVec3 frictionForce = velocityDir * ( -m_rVehicleMass * m_rForceStrength * friction);

		globalWheelForce[0] += frictionForce;
		globalWheelForce[1] += frictionForce;
		globalWheelForce[2] += frictionForce;
		globalWheelForce[3] += frictionForce;
	}
	else {
		if (noInput) {
			actor->setAngularVelocity(NxVec3(0, 0, 0));
			actor->setLinearVelocity(NxVec3(0, velocity.y, 0));
		}
	}

	//STEERING
	float angle = vehicle->getInputObj()->getThumbstick() * m_rMaxWheelAngle; //maximum wheel angle

	//get the angle of the normal to the wheel direction
	if (angle > 0) {
		angle = angle + 90;
	}
	else {
		angle = angle - 90;
	}

	//find the x and z components of the tire lateral
	angle = 90 - angle;

	float damperForce,
		  wheelRadius;

	vehicle->m_Wheels[0].setAngle(angle);
	vehicle->m_Wheels[1].setAngle(angle);

	bool inAir = true;
	bool onGround = true;

	//SUSPENSION
	for (int i = 0; i < 4 /*number of wheels*/; i++) 
	{
		w = &vehicle->m_Wheels[ i ];
		wheelRadius = w->getBoundingBox().m_fRadius/2;

		//Calculate world wheel position, used for raycast
		wheelPos = mat * w->getChassisPt();

		//Calculate vehicle up vector
		upVec = normalize( orient * NxVec3( 0, 1, 0 ));
		susAxis = normalize( orient * NxVec3(0,-1,0) );
		w->setSuspensionAxis( susAxis );

		//Raycast to ground to find distance
		NxRay ray( wheelPos, susAxis );
		NxRaycastHit hit;
		NxShape* hitObject;
		hitObject = m_Scene->raycastClosestShape( ray, NX_ALL_SHAPES, hit );

		//NEW STEERING
		NxMat33 localOrientation;
		actor->getGlobalOrientation().getInverse(localOrientation);
		NxVec3 pointVelocity = localOrientation * actor->getLocalPointVelocity(w->getChassisPt());

		NxVec3 normal = rotate(w->getWheelLateral(), w->getAngle());

		NxVec3 applied = (pointVelocity.dot(normal) / normal.dot(normal))*normal;
		applied = -applied*m_rSteeringPower;
		
		localWheelForce[i] += applied;
		//END NEW STEERING
		
		//apply forces to wheel if it is on the ground
		if( hit.distance < ( m_rMaxWheelDisplacement + wheelRadius )) 
		{
			inAir = false;

			/*if (hitObject == wall) {
				apply force downwards on wheel
			}*/

			//set the translation distance for the renderer
			w->setDisplacement( hit.distance - wheelRadius*2 );
			
			//calculate the spring force
			susForce = -1 * m_rSpringScale * m_rSpringK * ( hit.distance - wheelRadius - m_rWheelRestLength );

			//if the car is close to bottoming out, double the force
			if (w->getDisplacement()/m_rMaxWheelDisplacement >= 0.75) {
				susForce = susForce * susForce;
			}

			//calculate the damping force
			NxVec3 ptVelocity = actor->getLocalPointVelocity( w->getChassisPt() );
			damperForce = m_rSpringC * m_rDamperScale * ptVelocity.dot( susAxis );
			
			//add suspension force to current wheel
			localWheelForce[i] += NxVec3(0, susForce, 0) + NxVec3(0, damperForce, 0);

			//apply all accumulated forces to current wheel
			actor->addLocalForceAtLocalPos(localWheelForce[i], w->getChassisPt() - NxVec3(0, 2, 0));
			actor->addForceAtLocalPos(globalWheelForce[i], w->getChassisPt() - NxVec3(0, 2, 0));
		}
		else
		{
			onGround = false;

			//add an additional force downwards, make sure it's not too big of a force or 
			//else vehicles can't get over the craters
			double downwardForce;
			if (hit.distance < 20) // if not over a crater then give it a big force down
				downwardForce = hit.distance*70;
			else //over craters we want to float more
				downwardForce = 50;
			actor->addLocalForce(NxVec3(0, -downwardForce, 0));

			//set maximum translation for renderer
			float maxWheelSpeed = 7;
			w->setDisplacement(min((float)m_rMaxWheelDisplacement, (w->getDisplacement() + maxWheelSpeed * (float)time)));
		}
	}

	//Try to keep the vehicle level
	if (inAir) {
		//get global orientation, second row for y axis
		//NxVec3 upVec = actor->getGlobalOrientation()->getRow(1);
		
	}
	//Try to keep the vehicle from sticking to walls
	if (onGround) {
		
	}

	input->reset();

	//Check to see if the vehicle is falling
	if (actor->getGlobalPosition().y < -40) {
		vehicle->getInputObj()->setDir(Input::BACK_BUTTON);
	}
}

//--------------------------------------------------------------------------------------
// Function:  pause
// pause=true simulation is paused
//--------------------------------------------------------------------------------------
void Simulator::pause(bool pause)
{
	m_bPaused = pause;
}

//--------------------------------------------------------------------------------------
// Function:  pause
// pause=true simulation is paused
//--------------------------------------------------------------------------------------
bool Simulator::isPaused( )
{
	return m_bPaused;
}

//--------------------------------------------------------------------------------------
// Function:  normalize
//--------------------------------------------------------------------------------------
NxVec3 Simulator::normalize(NxVec3 vec) 
{
	NxReal mag = vec.magnitude();
	return NxVec3(vec.x/mag, vec.y/mag, vec.z/mag);
}

//--------------------------------------------------------------------------------------
// Function:  rotate
//--------------------------------------------------------------------------------------
NxVec3 Simulator::rotate(NxVec3 lateral, float angle) {
	float radians = angle*PI/180;

	float z = lateral.x * sin(radians) + lateral.z * cos(radians);
	float x = lateral.x * cos(radians) - lateral.z * sin(radians);

	return NxVec3(x, 0, z);
}

//--------------------------------------------------------------------------------------
// Function:  createVehicle
// Creates a vehicle based on a mesh
//--------------------------------------------------------------------------------------
void Simulator::createVehicle( Vehicle* vehicle ) 
{
	Mesh*       mesh = vehicle->getRenderable()->m_pMesh;
	Vec3        pos = vehicle->getPosition();
	BoundingBox b = vehicle->getBoundingBox();

	//NxTriangleMeshShapeDesc ShapeDesc = this->createTriMeshShape( mesh );
	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping	= 2.5f;
	bodyDesc.mass = m_rVehicleMass;

	// Create terrain and add to scene
	NxActorDesc actorDesc;

	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set( b.m_fWidth/2, b.m_fHeight/2, b.m_fLength/2 );
	actorDesc.shapes.pushBack( &boxDesc );

	//Initialize the vehicle's parameters
	actorDesc.body = &bodyDesc;	
	//actorDesc.shapes.pushBack( &ShapeDesc );
	actorDesc.globalPose.t = NxVec3(pos.x, pos.y, pos.z);

	//Create the vehicle in the scene
	NxActor* pActor = m_Scene->createActor( actorDesc );
	pActor->setMaxAngularVelocity(m_rMaxAngularVelocity);
	pActor->setCMassOffsetGlobalPosition( NxVec3(pos.x, pos.y-b.m_fHeight, pos.z) );
	assert( pActor );

	//Add the vehicle to global list of all vehicles
	vehicle->setPhysicsObj( pActor );
	m_Actors.push_back( pActor );
}

void Simulator::createMeteorGroup(MeteorGroup* mg) {
	//m_Debugger.writeToFile("create group");
	for (int i = 0; i < mg->numMeteors; i++) {
		mg->meteors[i]->update(Vec3(-400, 100, 500));
		Vec3 position = mg->meteors[i]->getPosition();
		
		Matrix m;
		D3DXMatrixIdentity( &m );
		D3DXMatrixTranslation( &m, position.x, position.y, position.z );
		
		mg->meteors[i]->update(position, m);
	}
}

void Simulator::simulateMeteorGroup(MeteorGroup* mg, double time) {
	for (int i = 0; i < mg->numMeteors; i++) {
		Meteor* m = mg->meteors[i];
		if (m->getAI()->getState() != AI::MOVING)
			continue;

		Vec3 currentPos = m->getPosition();
		Vec3 target = m->getTarget();
		float speed = 70;

		Vec3 direction = target - currentPos;
		float travelled = speed * time;
		float toTravel = D3DXVec3Length (&direction);
		if (travelled >= toTravel) {
			currentPos = target;
			m->informOfCollision();
			addCrater (m->getCraterToSpawn());
		}
		else {
			direction /= toTravel;
			currentPos += (direction * travelled);

			m->update (currentPos);
		}
	}
}

void Simulator::removeFromSimulation( Entity* entity )
{
	NxActor* pActor = entity->getPhysicsObj();
	m_Scene->releaseActor( *pActor );
	entity->setPhysicsObj( 0 );

	int size = m_Actors.size();
	for( int i=0; i < size; i++)
	{
		if( m_Actors[i] == pActor ) {
			m_Actors.erase( m_Actors.begin() + i );
			return;
		}
	}
}

NxActor* Simulator::createLittleBox( NxVec3 pos )
{
	// Create terrain and add to scene
	NxActorDesc actorDesc;

	//Create a small box 
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set( 0.1, 0.1, 0.1 );
	actorDesc.shapes.pushBack( &boxDesc );

	//Initialize the vehicle's parameters
	actorDesc.globalPose.t = pos;
	assert( actorDesc.isValid() );

	//Create the vehicle in the scene
	NxActor* pActor = m_Scene->createActor( actorDesc );
	assert( pActor );
	return pActor;
}

//--------------------------------------------------------------------------------------
// Function:  addTerrainFromX
//--------------------------------------------------------------------------------------
void Simulator::addTerrainFromX( Terrain* terrain )
{
	Mesh*  mesh = terrain->getRenderable()->m_pMesh;
	Vec3   p = terrain->getPosition();
	NxVec3 pos (p.x, p.y, p.z);

	NxTriangleMeshShapeDesc ShapeDesc = this->createTriMeshShape( mesh );

	// Create terrain and add to scene
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack( &ShapeDesc );
	actorDesc.globalPose.t = pos;

	NxActor* pActor = m_Scene->createActor( actorDesc );
	terrain->setPhysicsObj( pActor );
	m_Actors.push_back( pActor );
}

//--------------------------------------------------------------------------------------
// Function:  addCrater
// Adds a crater actor to the physics simulator.
//--------------------------------------------------------------------------------------
void Simulator::addCrater( Crater* crater )
{
	Mesh*  mesh = crater->getRenderable()->m_pMesh;
	Vec3   p = crater->getPosition();
	NxVec3 pos (p.x, p.y, p.z);

	NxTriangleMeshShapeDesc ShapeDesc = createTriMeshShape( mesh );

	// Create terrain and add to scene
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack( &ShapeDesc );
	actorDesc.globalPose.t = pos;

	NxActor* pActor = m_Scene->createActor( actorDesc );
	crater->setPhysicsObj( pActor );
	m_Actors.push_back( pActor );
}

//--------------------------------------------------------------------------------------
// Function:  createVehicle
// Creates a box character in the PhysX scene representing a vehicle.
//--------------------------------------------------------------------------------------
/*
void Simulator::createVehicle( Vec3 pos, BoundingBox b ) 
{
	//Add a single shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping	= 0.5f;
	bodyDesc.mass = m_rVehicleMass;

	//Create a box with the supplied bounding box dimensions
	NxBoxShapeDesc boxDesc;
	//boxDesc.mass = 10.0f;
	boxDesc.dimensions.set( b.m_fWidth, b.m_fHeight, b.m_fLength );
	actorDesc.shapes.pushBack( &boxDesc );

	//Initialize the vehicle's parameters
	actorDesc.body = &bodyDesc;
	//actorDesc.density = m_rVehicleMass / (b.m_fLength * b.m_fHeight * b.m_fWidth);
	actorDesc.globalPose.t = NxVec3( pos.x, pos.y, pos.z );
	assert( actorDesc.isValid() );

	//Create the vehicle in the scene
	NxActor* pActor = m_Scene->createActor( actorDesc );
	pActor->setMaxAngularVelocity(m_rMaxAngularVelocity);
	assert( pActor );

	//Add the vehicle to global list of all vehicles
	m_Vehicles.push_back( pActor );
}
*/

//--------------------------------------------------------------------------------------
// Function:  createTriMeshShape
//--------------------------------------------------------------------------------------
NxTriangleMeshShapeDesc Simulator::createTriMeshShape( Mesh* mesh )
{
	ID3DXMesh* Mesh = mesh->GetMesh();

	typedef struct {
		Vec3 VertexPos;
		Vec3 Normal;
		D3DXVECTOR2 TexCoord;
	} Mesh_FVF;

	//Used to copy indices
	typedef struct {
		short IBNumber[3];
	} IndexBufferStruct;

	int NumVerticies = Mesh->GetNumVertices();
	int NumTriangles = Mesh->GetNumFaces();
	DWORD FVFSize = D3DXGetFVFVertexSize(Mesh->GetFVF());

	//Create pointer for vertices
	NxVec3* verts = new NxVec3[NumVerticies];

	char *DXMeshPtr;
	Mesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&DXMeshPtr );
	for(int i = 0; i < NumVerticies; i++)
	{
		Mesh_FVF *DXMeshFVF = ( Mesh_FVF* )DXMeshPtr;
		verts[i] = NxVec3( DXMeshFVF->VertexPos.x, DXMeshFVF->VertexPos.y, DXMeshFVF->VertexPos.z );
		DXMeshPtr += FVFSize;
	}
	Mesh->UnlockVertexBuffer();

	//Create pointer for indices
	IndexBufferStruct *tris = new IndexBufferStruct[NumTriangles];

	IndexBufferStruct *DXMeshIBPtr;
	Mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&DXMeshIBPtr);
	for(int NumInd = 0; NumInd < NumTriangles; NumInd++)
	{
		tris[NumInd].IBNumber[0] = DXMeshIBPtr[NumInd].IBNumber[0];
		tris[NumInd].IBNumber[1] = DXMeshIBPtr[NumInd].IBNumber[1];
		tris[NumInd].IBNumber[2] = DXMeshIBPtr[NumInd].IBNumber[2];
	}
	Mesh->UnlockIndexBuffer();

	// Build physical model in PhysX
	NxTriangleMeshDesc TriMeshDesc;
	TriMeshDesc.numVertices = NumVerticies;
	TriMeshDesc.numTriangles = NumTriangles;
	TriMeshDesc.pointStrideBytes = sizeof(NxVec3);
	TriMeshDesc.triangleStrideBytes = 3*sizeof(NxU16);
	TriMeshDesc.points = verts;
	TriMeshDesc.triangles = tris;
	TriMeshDesc.flags = NX_MF_16_BIT_INDICES;

	NxInitCooking();
	NxTriangleMeshShapeDesc ShapeDesc;

	// Cooking from memory
	MemoryWriteBuffer buf;
	bool status = NxCookTriangleMesh( TriMeshDesc, buf );
	ShapeDesc.meshData = m_PhysicsSDK->createTriangleMesh( MemoryReadBuffer(buf.data) );

	delete[] verts;
	delete[] tris;

	return ShapeDesc;
}

//--------------------------------------------------------------------------------------
// Function:  Destructor
// Clears up all the memory used by PhysX by releasing all actors, the scene and the
// Physics SDK.
//--------------------------------------------------------------------------------------
Simulator::~Simulator() 
{
	for( int i=0; i < m_Actors.size(); i++ )
	{
		m_Scene->releaseActor( *m_Actors[i] );
		m_Actors[i] = NULL;
	}

	//now release the scene and physics SDK
	if( m_PhysicsSDK != NULL )
	{
		if( m_Scene != NULL)
		{
			m_PhysicsSDK->releaseScene(*m_Scene);
			m_Scene = NULL;
		}
		NxReleasePhysicsSDK( m_PhysicsSDK );
		m_PhysicsSDK = NULL;
	}
}


void Simulator::setForceVec(Vec3 vec)
{
	m_vForceVec = NxVec3(vec.x, vec.y, vec.z);
}


void Simulator::setForceStr(double str)
{
	m_rForceStrength = NxReal(str);
}

void Simulator::setForceMode(bool mode)
{
	m_bForceMode = mode;
}

void Simulator::setGravity(Vec3 gravity)
{
	NxScene* scene;

	m_vDefaultGravity = NxVec3(gravity.x, gravity.y, gravity.z);
	if( m_PhysicsSDK )
	{
		scene = m_PhysicsSDK->getScene( 0 );
		scene->setGravity( m_vDefaultGravity );
	}

	m_rSpringK	= ( -m_vDefaultGravity.y * m_rVehicleMass ) / ( m_rWheelRestLength * 4 );
	m_rSpringC	= 2 * sqrt( m_rSpringK * m_rVehicleMass );
}

void Simulator::setRestitution(double res)
{
	m_rRestitution = NxReal(res);
}

void Simulator::setSFriction(double friction)
{
	m_rStaticFriction = NxReal(friction);
}

void Simulator::setDFriction(double friction)
{
	m_rDynamicFriction = NxReal(friction);
}

void Simulator::setMaxAngularVelocity(double maxAngle)
{
	m_rMaxAngularVelocity = NxReal(maxAngle);
}

void Simulator::setMaxWheelAngle(double maxAngle)
{
	m_rMaxWheelAngle = maxAngle;
}

void Simulator::setSpringScale( double springScale )
{
	m_rSpringScale = springScale;
}

void Simulator::setDamperScale( double damperScale )
{
	m_rDamperScale = damperScale;
}

void Simulator::setSteeringPower( double steeringPower )
{
	m_rSteeringPower = steeringPower;
}

void Simulator::setBrakingFriction( double friction )
{
	m_rBrakingFriction = friction;
}

void Simulator::setVehicleMass( double mass )
{
	m_rVehicleMass = mass;

	m_rSpringK	= ( -m_vDefaultGravity.y * m_rVehicleMass ) / ( m_rWheelRestLength * 4 );
	m_rSpringC	= 2 * sqrt( m_rSpringK * m_rVehicleMass );

	for( int i=0; i<m_Actors.size(); i++ )
		m_Actors[i]->setMass( mass );
}


void Simulator::printVariables()
{
	m_Debugger.writeToFile("variables");
	m_Debugger.writeToFile(Vec3(m_vForceVec.x,m_vForceVec.y,m_vForceVec.z));
	m_Debugger.writeToFile((double)m_rForceStrength);
	if (m_bForceMode)
		m_Debugger.writeToFile("true");
	else
		m_Debugger.writeToFile("false");
	m_Debugger.writeToFile(Vec3(m_vDefaultGravity.x, m_vDefaultGravity.y, m_vDefaultGravity.z));
	m_Debugger.writeToFile((double)m_rRestitution);
	m_Debugger.writeToFile((double)m_rStaticFriction);
	m_Debugger.writeToFile((double)m_rDynamicFriction);
	m_Debugger.writeToFile((double)m_rMaxAngularVelocity);
	m_Debugger.writeToFile(m_rMaxWheelAngle);
}