#include "Simulator.h"


//--------------------------------------------------------------------------------------
// Function:  Constructor
// Initializes the simulator object
//--------------------------------------------------------------------------------------
Simulator::Simulator() 
{
	//initialize all simulation data members
	m_vForceVec			= NxVec3(0, 0, 0);
	m_PhysicsSDK		= NULL;
	m_Scene				= NULL;
	m_rForceStrength	= 1000000;
	m_bForceMode		= true;
	m_vDefaultGravity	= NxVec3(0,-20,0);
	m_GroundPlane		= NULL;
	m_vP1Dir			= Vec3(0, 0, 0);
	m_rRestitution		= NxReal(0.0);
	m_rStaticFriction	= NxReal(0.1);
	m_rDynamicFriction	= NxReal(0.3);

	forward = false;

	//InitNx();
}

//--------------------------------------------------------------------------------------
// Function:  InitNx
// Initializes the PhysX engine as well as some other fundamental elements
//--------------------------------------------------------------------------------------
void Simulator::InitNx( Mesh* terrainMesh ) 
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
	m_PhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.1);

	//Set the debug visualization parameters
	m_PhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

	//Create the default material
	NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(m_rRestitution);
	defaultMaterial->setStaticFriction(m_rStaticFriction);
	defaultMaterial->setDynamicFriction(m_rDynamicFriction);

	//Create the ground
	//m_GroundPlane = createGroundPlane( );
	addTerrainFromX( terrainMesh, NxVec3(0, 0, 0) );
	
	m_PhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
}

//--------------------------------------------------------------------------------------
// Function:  simulate
// Simulates the movement of entities based on the elapsed time
//--------------------------------------------------------------------------------------
void Simulator::simulate( vector<Vehicle*> vehicles, double elapsedTime ) 
{
	NxF32 mat[4][4];
	m_dDeltaTime = elapsedTime;

	startPhysics();
	getPhysicsResults();

	//Update all the entity positions based on PhysX simulated actor positions
	for( int i=0; i < m_Vehicles.size(); i++ )
	{
		//Get the inputs associated with vehicle
		//keyboard controls
		//m_bInputs = vehicles[i]->getInput();
		//m_bButtons = vehicles[i]->getButtons();

		// XBox controls
		//m_vP1Dir = vehicles[i]->getDir();

		//using an XBox controller
		//if (vehicles[i]->getController()) {
			processForceKeys(m_Vehicles[i], vehicles[i]);
	//	}

		//Add forces to the vehicle based on input
		//processForceKeys();

		/*if (forward) {
			m_Debugger.writeToFile("here");
			//m_Vehicles[i]->addLocalForceAtLocalPos(NxVec3(0, 0, m_rForceStrength/10), NxVec3(width/2, 0, 0));
			//m_Vehicles[i]->addLocalForceAtLocalPos(NxVec3(0, 0, m_rForceStrength/10), NxVec3(-10, 0, 0));
			forward = false;
		}*/

		//Get the new position of the vehicle in vector and matrix formats
		NxVec3 vec	 = m_Vehicles[i]->getGlobalPosition();
		//float height = vehicles[i]->getBoundingBox().m_fHeight;

		Matrix m, newMat;
		NxMat34 mat;
		m_Vehicles[i]->getGlobalPose().getColumnMajor44( m );
		mat = m_Vehicles[i]->getGlobalPose();

		BoundingBox BB = vehicles[i]->getBoundingBox();
		NxVec3 leftTop( -BB.m_fWidth/2, -BB.m_fHeight, BB.m_fLength/2 );
		NxVec3 rightTop( BB.m_fWidth/2, -BB.m_fHeight, BB.m_fLength/2 );
		NxVec3 leftBottom( -BB.m_fWidth/2, -BB.m_fHeight, -BB.m_fLength/2 );
		NxVec3 rightBottom( BB.m_fWidth/2, -BB.m_fHeight, -BB.m_fLength/2 );

		leftTop =  mat * leftTop;
		rightTop =  mat * rightTop;
		leftBottom =  mat * leftBottom;
		rightBottom =  mat * rightBottom;

		//D3DXMatrixTranslation( &m, vec.x, vec.y-height, vec.z );
		
		NxVec3 vlc = m_Vehicles[i]->getLinearVelocity();

		//Update the vehicle position in the game
		vehicles[i]->update( Vec3(vec.x, vec.y, vec.z), Vec3(vlc.x, 0, vlc.z), m, Vec3( &leftTop.x ), Vec3( &rightTop.x ), Vec3( &leftBottom.x ), Vec3( &rightBottom.x ) );
	
		//
		//m_Debugger.writeToFile("global pos");
		//m_Debugger.writeToFile(m_Vehicles[i]->getGlobalPosition());
		//m_Debugger.writeToFile("orientation");
		//m_Debugger.writeToFile(m_Vehicles[i]->g());
		/*
		debug.writeToFile( "Position: " );
		debug.writeToFile(vec);
		*/
		/*
		debug.writeToFile("Velocity: ");
		debug.writeToFile(velocity);
		*/
		//m_Debugger.writeToFile(m_Vehicles[i]->getAngularVelocity());
	}
}

//--------------------------------------------------------------------------------------
// Function:  createGroundPlane
// Creates a ground plane at (0, 0, 0) and normal (0, 1, 0)
//--------------------------------------------------------------------------------------
NxActor* Simulator::createGroundPlane() 
{
	NxPlaneShapeDesc planeDesc;
	NxActorDesc actorDesc;
	planeDesc.normal = NxVec3(0, 1, 0);
	planeDesc.d = 0;
	actorDesc.shapes.pushBack(&planeDesc);

	return m_Scene->createActor( actorDesc );
}

//--------------------------------------------------------------------------------------
// Function:  createVehicle
// Creates a box character in the PhysX scene representing a vehicle.
//--------------------------------------------------------------------------------------
void Simulator::createVehicle( Vec3 pos, BoundingBox b ) 
{
	//Add a single shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	//Create a box with the supplied bounding box dimensions
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set( b.m_fWidth, b.m_fHeight, b.m_fLength );
	actorDesc.shapes.pushBack( &boxDesc );

	//Initialize the vehicle's parameters
	actorDesc.body = &bodyDesc;
	actorDesc.density = 15.0f;
	actorDesc.globalPose.t = NxVec3( pos.x, pos.y, pos.z );
	assert( actorDesc.isValid() );

	//Initialize the wheels for the vehicle
	m_Debugger.writeToFile(b.m_fHeight);

	//Create the vehicle in the scene
	NxActor* pActor = m_Scene->createActor( actorDesc );
	assert( pActor );

	//Add the vehicle to global list of all vehicles
	m_Vehicles.push_back( pActor );

	//Debugging
	m_Debugger.writeToFile("width");
	m_Debugger.writeToFile(b.m_fWidth);
	m_Debugger.writeToFile("height");
	m_Debugger.writeToFile(b.m_fHeight);
	m_Debugger.writeToFile("length");
	m_Debugger.writeToFile(b.m_fLength);
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
// Function:  processForceKeys
// Processes all of the force inputs
//--------------------------------------------------------------------------------------
void Simulator::processForceKeys() {
	// Process force keys

	// iterate through all the vehicles
	//for( int v = 0; v < gVehicles.size(); v++ )
	//{
		/*for( int i = 0; i < 4; i++ )
		{
			if( !m_bInputs[i] ) { continue; } 

			switch( i )
			{
				case LEFT: 
				{ 
					m_vForceVec = applyForceToActor( m_Vehicles[0], NxVec3(-1,0,0), m_rForceStrength );
					break; 
				}
				case RIGHT: 
				{ 
					m_vForceVec = applyForceToActor( m_Vehicles[0], NxVec3(0,0,1), m_rForceStrength ); 
					forward = true;
					break; 
				}
				case FORWARD: 
				{ 
					m_vForceVec = applyForceToActor( m_Vehicles[0], NxVec3(1,0,0), m_rForceStrength );
					break; 
				}
				case BACKWARD: 
				{ 
					m_vForceVec = applyForceToActor( m_Vehicles[0], NxVec3(0,0,-1), m_rForceStrength ); 
					break; 
				}
			}
		}*/
	//}

	//xbox controllers
	//m_vForceVec = applyForceToActor( m_Vehicles[0], NxVec3( m_vP1Dir.x, 0, m_vP1Dir.z), m_rForceStrength);
}

//--------------------------------------------------------------------------------------
// Function:  processForceKeys
// Processes all of the force inputs
//--------------------------------------------------------------------------------------
void Simulator::processForceKeys(NxActor* actor, Vehicle* vehicle) {
	// Process force keys
	float length = vehicle->getBoundingBox().m_fLength;
	float width = vehicle->getBoundingBox().m_fWidth;
	float height = vehicle->getBoundingBox().m_fHeight;
	float wheelDiameter = vehicle->m_Wheels[0].getDiameter();
	float wheelRadius = wheelDiameter / 2.0;
	bool freeze = false;

	NxVec3 wheel[4];
	wheel[0] = NxVec3(width/2, 0, -length/2);
	wheel[1] = NxVec3(-width/2, 0, -length/2);
	wheel[2] = NxVec3(width/2, 0, length/2);
	wheel[3] = NxVec3(-width/2, 0, length/2);

	NxVec3 localWheelForce[4];
	NxVec3 globalWheelForce[4];
	for (int i = 0; i < 4; i++) {
		localWheelForce[i] = globalWheelForce[i] = NxVec3(0, 0, 0);
	}

	bool* buttons = vehicle->getButtons();

	//NxVec3 test = actor->getGlobalOrientation() * actor->getLinearVelocity();
	//m_Debugger.writeToFile(Vec3(test.x, test.y, test.z));

	//INPUT
	for( int i = 0; i < 4; i++ )
	{
		if( !buttons[i] ) { continue; } 

		switch( i )
		{
			case 0: //A_BUTTON - accelerate
			{
				//TODO: change tire orientation
				float x_dir = vehicle->getThumbstick();
				//m_vForceVec = applyForceToActor( actor, NxVec3(x_dir,0,0), m_rForceStrength ); //temporarily
				
				//actor->addLocalForceAtLocalPos(NxVec3(0, 0, m_rForceStrength/10), wheel[2]);
				//actor->addLocalForceAtLocalPos(NxVec3(0, 0, m_rForceStrength/10), wheel[3]);

				localWheelForce[2] += NxVec3(0, 0, m_rForceStrength/10);
				localWheelForce[3] += NxVec3(0, 0, m_rForceStrength/10);
				break; 
			}
			case 1: //B_BUTTON - brake / reverse
			{
				NxVec3 velocity = actor->getLinearVelocity();

				//brake
				if (velocity.magnitude() > 1) {
					velocity = normalize(velocity);

					globalWheelForce[0] += (velocity * (-m_rForceStrength/10));
					globalWheelForce[1] += (velocity * (-m_rForceStrength/10));
					globalWheelForce[2] += (velocity * (-m_rForceStrength/10));
					globalWheelForce[3] += (velocity * (-m_rForceStrength/10));
				}
				//reverse
				/*else {
					localWheelForce[2] += NxVec3(0, 0, -m_rForceStrength/10);
					localWheelForce[3] += NxVec3(0, 0, -m_rForceStrength/10);
				}*/
				break; 
			}
			case 2: //X_BUTTON - reverse
			{
				localWheelForce[2] += NxVec3(0, 0, -m_rForceStrength/10);
				localWheelForce[3] += NxVec3(0, 0, -m_rForceStrength/10);
				break;
			}
			case 3: //Y_BUTTON - emergency brake
			{
				
			}
		}
	}

	//STEERING
	float angle = vehicle->getThumbstick()* 35; //35 is maximum wheel angle

	NxVec3 force(-50000*sin(angle*PI/180), 0, -100000*sin(angle*PI/180));
	localWheelForce[0] += force;
	localWheelForce[1] += force;
	
	if (angle > 0) {
		//actor->addLocalForceAtLocalPos(NxVec3(-10000, 0, 0), wheel[0]);
		//actor->addLocalForceAtLocalPos(NxVec3(-10000, 0, 0), wheel[1]);

		NxVec3 force(-100000*sin(angle*PI/180), 0, 0);//-100000*sin(angle*PI/180));
		localWheelForce[0] += force;
		localWheelForce[1] += force;

		//localWheelForce[0] += NxVec3(-10000*, 0, 0);
		//localWheelForce[1] += NxVec3(-10000, 0, 0);
	}

	else if (angle < 0) {
		//actor->addLocalForceAtLocalPos(NxVec3(10000, 0, 0), wheel[0]);
		//actor->addLocalForceAtLocalPos(NxVec3(10000, 0, 0), wheel[1]);

		//localWheelForce[0] += NxVec3(10000, 0, 0);
		//localWheelForce[1] += NxVec3(10000, 0, 0);

		NxVec3 force(-100000*sin(angle*PI/180), 0, 0);//100000*sin(angle*PI/180));
		localWheelForce[0] += force;
		localWheelForce[1] += force;
	}

	//get the angle of the normal to the wheel direction
	/*if (angle > 0) {
		angle = angle + 90;
	}
	else {
		angle = angle - 90;
	}

	//find the x and z components of the tire lateral
	angle = 90 - angle;
	float x = cos(angle*PI/180);
	float z = sin(angle*PI/180);

	NxVec3 tireLateral(x, 0, z);
	NxVec3 velocity = NxVec3(actor->getGlobalOrientation() * actor->getLinearVelocity());

	//project the tireLateral on the velocity of the car
	NxVec3 steering = (velocity.dot(tireLateral) / tireLateral.dot(tireLateral)) * tireLateral;
	actor->addLocalForceAtLocalPos(NxVec3(steering.x, 0, steering.z)*1000, wheel[0]);
	actor->addLocalForceAtLocalPos(steering*1000, wheel[1]);*/

	//SUSPENSION
	for (int i = 0; i < 4 /*number of wheels*/; i++) {
		//Ray casting
		NxVec3 origin = actor->getGlobalPosition() + wheel[i];
		NxVec3 direction(0, -1, 0);

		NxRay ray(origin, direction);
		NxRaycastHit hit;

		m_Scene->raycastClosestShape(ray, NX_ALL_SHAPES, hit);
		//m_Debugger.writeToFile(hit.distance);

		//apply forces due to suspension
		if (hit.distance < (wheelDiameter + height)) {
			//actor->addLocalForceAtLocalPos(NxVec3(0, 0.1, 0), wheel[i]); //(0.5) * (m_rForceStrength/10) * (wheelDiameter - hit.distance) * (wheelDiameter - hit.distance)
			
			localWheelForce[i] += NxVec3(0, (m_rForceStrength/5000000) * (wheelDiameter - hit.distance) * (wheelDiameter - hit.distance), 0);

			actor->addLocalForceAtLocalPos(localWheelForce[i], wheel[i]);
			actor->addForceAtLocalPos(globalWheelForce[i], wheel[i]);
			
			//m_Debugger.writeToFile("here!!!!");
		}
		/*m_Debugger.writeToFile("hit distance");
			m_Debugger.writeToFile(hit.distance);
			m_Debugger.writeToFile("wheel diameter");
			m_Debugger.writeToFile(wheelDiameter);
			Vec3 test(localWheelForce[i].x, localWheelForce[i].y, localWheelForce[i].z);
			m_Debugger.writeToFile(test);
		m_Debugger.writeToFile("");*/
	}

	if (freeze) {
		//actor->setAngularVelocity(0);
		//actor->setLinearVelocity(0);
		freeze = false;
	}

	//m_Debugger.writeToFile(steering*10000);
	//m_Debugger.writeToFile("");
	
	//xbox controllers
	//m_vForceVec = applyForceToActor( m_Vehicles[0], NxVec3( m_vP1Dir.x, 0, m_vP1Dir.z), m_rForceStrength);
}

NxVec3 Simulator::normalize(NxVec3 vec) {
	NxReal mag = vec.magnitude();
	return NxVec3(vec.x/mag, vec.y/mag, vec.z/mag);
}

//--------------------------------------------------------------------------------------
// Function:  applyForceToActor
// Applies the forces from input and PhysX to the entities and other objects.
//--------------------------------------------------------------------------------------
NxVec3 Simulator::applyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength) 
{
	NxVec3 forceVec = forceStrength * forceDir * m_dDeltaTime;
	actor->addForce( forceVec );
	return forceVec;
}



//--------------------------------------------------------------------------------------
// Function:  addTerrainFromX
//--------------------------------------------------------------------------------------
void Simulator::addTerrainFromX( Mesh* mesh, NxVec3 pos )
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

	// Create terrain and add to scene
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack( &ShapeDesc );
	actorDesc.globalPose.t = pos;

	m_Terrain = m_Scene->createActor( actorDesc );

	delete[] verts;
	delete[] tris;
}


//--------------------------------------------------------------------------------------
// Function:  Destructor
// Clears up all the memory used by PhysX by releasing all actors, the scene and the
// Physics SDK.
//--------------------------------------------------------------------------------------
Simulator::~Simulator() 
{
	//release all actors
	for( int i=0; i < m_Vehicles.size(); i++ )
	{
		m_Scene->releaseActor( *m_Vehicles[i] );
		m_Vehicles[i] = NULL;
	}
	
	m_Scene->releaseActor( *m_Terrain );

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