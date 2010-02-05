#include "Simulator.h"


//--------------------------------------------------------------------------------------
// Function:  Constructor
// Initializes the simulator object
//--------------------------------------------------------------------------------------
Simulator::Simulator() 
{
	//initialize all simulation data members
	m_vForceVec			= NxVec3(0, 0, 0);
	m_PhysicsSDK			= NULL;
	m_Scene				= NULL;
	m_rForceStrength	= 1000000;
	m_bForceMode		= true;
	m_vDefaultGravity	= NxVec3(0,-9.8,0);
	m_GroundPlane		= NULL;
	m_vP1Dir			= Vec3(0, 0, 0);

	InitNx();
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
		m_bInputs = vehicles[i]->getInput();

		// XBox controls
		m_vP1Dir = vehicles[i]->getDir();

		//Add forces to the vehicle based on input
		processForceKeys();

		//Get the new position of the vehicle in vector and matrix formats
		NxVec3 vec	 = m_Vehicles[i]->getGlobalPosition();
		float height = vehicles[i]->getBoundingBox().m_fHeight;

		m_Vehicles[i]->getGlobalPose().getColumnMajor44( mat );
		Matrix m = Matrix( mat[0] );
		D3DXMatrixTranslation( &m, vec.x, vec.y-height, vec.z );
		
		NxVec3 vlc = m_Vehicles[i]->getLinearVelocity();

		//Update the vehicle position in the game
		vehicles[i]->update( Vec3(vec.x, vec.y-height, vec.z), Vec3(vlc.x, 0, vlc.z), m );

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
	//Create the Phyics SDK
	m_PhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION );
	if( !m_PhysicsSDK ) return;

	//Create the scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = m_vDefaultGravity;
	sceneDesc.simType = NX_SIMULATION_SW;
	m_Scene = m_PhysicsSDK->createScene( sceneDesc );

	//Set the physics parameters
	m_PhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01);

	//Set the debug visualization parameters
	m_PhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

	//Create the default material
	NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.0);
	defaultMaterial->setDynamicFriction(0.0);

	//Create the ground
	m_GroundPlane = createGroundPlane( );
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
	actorDesc.density = 10.0f;
	actorDesc.globalPose.t = NxVec3( pos.x, pos.y + b.m_fHeight, pos.z );
	assert( actorDesc.isValid() );

	//Create the vehicle in the scene
	NxActor* pActor = m_Scene->createActor( actorDesc );
	assert( pActor );

	//Add the vehicle to global list of all vehicles
	m_Vehicles.push_back( pActor );
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
		for( int i = 0; i < 4; i++ )
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
		}
	//}

	//xbox controllers
	m_vForceVec = applyForceToActor( m_Vehicles[0], NxVec3( m_vP1Dir.x, 0, m_vP1Dir.z), m_rForceStrength);

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
	convexShapeDesc.meshData = m_PhysicsSDK->createConvexMesh( readBuffer );

	//initialize our actor and add the new PhysX mesh to it
	NxActorDesc actorDesc;
	bodyDesc.linearVelocity = NxVec3(0.0);
	actorDesc.shapes.pushBack( &convexShapeDesc );
	actorDesc.body = &bodyDesc;
	actorDesc.density = 10.0;

	actorDesc.globalPose.t = NxVec3(pos.x, pos.y, pos.z);
	assert(actorDesc.isValid());

	//create the actor and add it to the global scene
	NxActor* pActor = m_Scene->createActor(actorDesc);
	assert(pActor);
	actorDesc.contactReportFlags = NX_NOTIFY_ALL;

	//clear up the temporary description
	delete temp_convexDesc.points;

	return pActor;
}
