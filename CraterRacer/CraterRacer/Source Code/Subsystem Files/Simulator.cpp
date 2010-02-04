#include "Simulator.h"

// Physics SDK globals
NxPhysicsSDK*     gPhysicsSDK = NULL;
NxScene*          gScene = NULL;
NxVec3            gDefaultGravity(0,-9.8,0);

//Force globals
NxVec3 gForceVec(0, 0, 0);
NxReal gForceStrength = 1000000;
bool bForceMode = true;

// Keyboard globals
#define MAX_KEYS 256
bool gKeys[MAX_KEYS];

//Time global
double deltaTime;

//Actor globals
NxActor* groundPlane = NULL;
vector< NxActor* > gVehicles;
Vec3 boxPos;
double boxSize;

bool* input;
Vec3 p1_dir(0.0, 0.0, 0.0);

//Debugging
DebugWriter debug;


//--------------------------------------------------------------------------------------
// Function:  Constructor
// Initializes the simulator object
//--------------------------------------------------------------------------------------
Simulator::Simulator() 
{
	InitNx();
}


//--------------------------------------------------------------------------------------
// Function:  simulate
// Simulates the movement of entities based on the elapsed time
//--------------------------------------------------------------------------------------
void Simulator::simulate( vector<Vehicle*> vehicles, double elapsedTime ) 
{
	NxF32 mat[4][4];
	deltaTime = elapsedTime;
	startPhysics();
	getPhysicsResults();

	//Update all the entity positions based on PhysX simulated actor positions
	for( int i=0; i < gVehicles.size(); i++ )
	{
		//Get the inputs associated with vehicle
		//keyboard controls
		input = vehicles[i]->getInput();

		// XBox controls
		p1_dir = vehicles[i]->getDir();

		//Add forces to the vehicle based on input
		processForceKeys();

		//Get the new position of the vehicle in vector and matrix formats
		NxVec3 vec	 = gVehicles[i]->getGlobalPosition();
		float height = vehicles[i]->getBoundingBox().m_fHeight;

		gVehicles[i]->getGlobalPose().getColumnMajor44( mat );
		Matrix m = Matrix( mat[0] );
		D3DXMatrixTranslation( &m, vec.x, 0.0, vec.z );
		
		NxVec3 vlc = gVehicles[i]->getLinearVelocity();

		//Update the vehicle position in the game
		vehicles[i]->update( Vec3(vec.x, 0, vec.z), Vec3(vlc.x, 0, vlc.z), m );

		/*
		debug.writeToFile( "Position: " );
		debug.writeToFile(vec);
		*/
		/*
		debug.writeToFile("Velocity: ");
		debug.writeToFile(velocity);
		*/
	}
}


//--------------------------------------------------------------------------------------
// Function:  InitNx
// Initializes the PhysX engine as well as some other fundamental elements
//--------------------------------------------------------------------------------------
void Simulator::InitNx( void ) 
{
	 input = new bool[4];

	//Create the Phyics SDK
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
	if (!gPhysicsSDK) return;

	//Create the scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = gDefaultGravity;
	sceneDesc.simType = NX_SIMULATION_SW;
	gScene = gPhysicsSDK->createScene(sceneDesc);

	//Set the physics parameters
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01);

	//Set the debug visualization parameters
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

	//Create the default material
	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.0);
	defaultMaterial->setDynamicFriction(0.0);

	//Create the ground
	groundPlane = createGroundPlane( );

	input[0] = false;
	input[1] = false;
	input[2] = false;
	input[3] = false;

	p1_dir = Vec3(0.0, 0.0, 0.0);
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
	return gScene->createActor(actorDesc);
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

	//Initialize the vehicle
	actorDesc.body = &bodyDesc;
	actorDesc.density = 10.0f;
	actorDesc.globalPose.t = NxVec3( pos.x, pos.y + b.m_fHeight, pos.z );
	assert( actorDesc.isValid() );

	//Create the vehicle in the scene
	NxActor* pActor = gScene->createActor(actorDesc);
	assert( pActor );

	//Add the vehicle to global list of all vehicles
	gVehicles.push_back( pActor );
}

//--------------------------------------------------------------------------------------
// Function:  startPhysics
// Called the first time PhysX simulates
//--------------------------------------------------------------------------------------
void Simulator::startPhysics() 
{
	gScene->simulate(deltaTime);
	gScene->flushStream();
}

//--------------------------------------------------------------------------------------
// Function:  getPhysicsResults
// Processes inputs, and updates objects
//--------------------------------------------------------------------------------------
void Simulator::getPhysicsResults() 
{
	while (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, true));
	processInput();
}

//--------------------------------------------------------------------------------------
// Function:  processInput
// Processes all the inputs
//--------------------------------------------------------------------------------------
void Simulator::processInput()
{
	processForceKeys();
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
		for( int i = 0; i < 4; i++ )
		{
			if(!input[i]) { continue; } 

			switch (i)
			{
				case LEFT: 
				{ 
					gForceVec = applyForceToActor( gVehicles[0], NxVec3(-1,0,0), gForceStrength );
					debug.writeToFile("left");
					break; 
				}
				case RIGHT: 
				{ 
					gForceVec = applyForceToActor( gVehicles[0], NxVec3(0,0,1), gForceStrength ); 
					break; 
				}
				case FORWARD: 
				{ 
					gForceVec = applyForceToActor( gVehicles[0], NxVec3(1,0,0), gForceStrength );
					break; 
				}
				case BACKWARD: 
				{ 
					gForceVec = applyForceToActor( gVehicles[0], NxVec3(0,0,-1), gForceStrength ); 
					break; 
				}
			}
		}
	//}

	//xbox controllers
	gForceVec = applyForceToActor(gVehicles[0], NxVec3(p1_dir.x, 0, p1_dir.z), gForceStrength);

}

//--------------------------------------------------------------------------------------
// Function:  applyForceToActor
// Applies the forces from input and PhysX to the entities and other objects.
//--------------------------------------------------------------------------------------
NxVec3 Simulator::applyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength) {
	NxVec3 forceVec = forceStrength*forceDir*deltaTime;
	actor->addForce(forceVec);
	return forceVec;
}


//--------------------------------------------------------------------------------------
// Function:  Destructor
// Clears up all the memory used by PhysX by releasing all actors, the scene and the
// Physics SDK.
//--------------------------------------------------------------------------------------
Simulator::~Simulator() 
{
	//release all actors
	for( int i=0; i < gVehicles.size(); i++ )
	{
		gScene->releaseActor( *gVehicles[i] );
		gVehicles[i] = NULL;
	}
	
	//now release the scene and physics SDK
	if(gPhysicsSDK != NULL)
	{
		if(gScene != NULL) gPhysicsSDK->releaseScene(*gScene);
		gScene = NULL;
		NxReleasePhysicsSDK(gPhysicsSDK);
		gPhysicsSDK = NULL;
	}
	
//	if (input)
	delete [] input;
}


//--------------------------------------------------------------------------------------
// Function:  addActor
// Creates an actor under PhysX simulation based on specified mesh and position.  The
// actor is added to the the list of all scene objects.
//--------------------------------------------------------------------------------------
void Simulator::addActor( Mesh* mesh, Vec3& pos )
{
	//gActors.push_back( createMeshActor( mesh, pos ) );
}



//--------------------------------------------------------------------------------------
// Function:  getVertsFromDXMesh
// Creates a list of NxVec3 vertices from a mesh object.
// This function is adapted from one off of gamedev.net at the following link.  This is just
// temporary to get it working for milestone 1.
// http://www.gamedev.net/community/forums/topic.asp?topic_id=534809&forum_id=10&gforum_id=0
//--------------------------------------------------------------------------------------
NxVec3* Simulator::getVertsFromDXMesh( Mesh* mesh )
{
	//initialize variables we need in this function
	DWORD numVerts		= mesh->m_dwNumVertices;
	DWORD numFaces		= mesh->m_dwNumFaces;
	NxVec3* verts		= new NxVec3[ numVerts ]; //the vertices we will be returning
	BYTE *ptr			= NULL;
	BYTE *lpIB			= NULL;
	LPD3DXMESH mesh_dx	= mesh->GetMesh(); 
	DWORD fvf;
	DWORD vertSize;

	//get the vertex format and its size
	fvf = mesh_dx->GetFVF();
	vertSize = D3DXGetFVFVertexSize(fvf);

	//transfer vertex values from DXMesh into NX verts list
	mesh_dx->LockVertexBuffer(0, (LPVOID*)&ptr);
	for( DWORD i=0; i < numVerts; i++ )
	{
		NxVec3 *vPtr =( NxVec3* )ptr;
		verts[i].x = vPtr->x;
		verts[i].y = vPtr->y;
		verts[i].z = vPtr->z;
		ptr += vertSize;		
	}
	mesh_dx->UnlockVertexBuffer();
	return verts;
}


//--------------------------------------------------------------------------------------
// Function:  createMeshActor
// Creates a PhysX actor given a mesh and position.  It returns the pointer to the new
// actor.  This function was adapted from a couple examples off of gamedev.net.
// http://www.gamedev.net/community/forums
//--------------------------------------------------------------------------------------
NxActor* Simulator::createMeshActor( Mesh* mesh, Vec3& pos )
{
	NxBodyDesc bodyDesc;
	NxConvexMeshDesc temp_convexDesc;

	//set up the description of a convex mesh using data from our known Mesh
	temp_convexDesc.numVertices		 = mesh->m_dwNumVertices;
	temp_convexDesc.pointStrideBytes = sizeof( NxVec3 );
	temp_convexDesc.points			 = getVertsFromDXMesh( mesh );
	temp_convexDesc.flags 			 = NX_CF_COMPUTE_CONVEX;
	bodyDesc.solverIterationCount	 = 3;

	//set up the description of the actor's shape we will create
	NxConvexShapeDesc convexShapeDesc;
	convexShapeDesc.localPose.t		= NxVec3(0,0,0);
	convexShapeDesc.mass			= 0;
	convexShapeDesc.density			= .001;

	//get PhysX to build it's own version of the mesh based on our temp description
	NxInitCooking();
	MemoryWriteBuffer buf;
	bool status = NxCookConvexMesh( temp_convexDesc, buf );

	//save this new mesh in our actor's shape description
	MemoryReadBuffer readBuffer( buf.data );
	convexShapeDesc.meshData = gPhysicsSDK->createConvexMesh( readBuffer );

	//initialize our actor and add the new PhysX mesh to it
	NxActorDesc actorDesc;
	bodyDesc.linearVelocity = NxVec3(0.0);
	actorDesc.shapes.pushBack( &convexShapeDesc );
	actorDesc.body = &bodyDesc;
	actorDesc.density = 10.0;

	actorDesc.globalPose.t = NxVec3(pos.x, pos.y, pos.z);
	assert(actorDesc.isValid());

	//create the actor and add it to the global scene
	NxActor* pActor = gScene->createActor(actorDesc);
	assert(pActor);
	actorDesc.contactReportFlags = NX_NOTIFY_ALL;

	//clear up the temporary description
	delete temp_convexDesc.points;

	return pActor;
}
