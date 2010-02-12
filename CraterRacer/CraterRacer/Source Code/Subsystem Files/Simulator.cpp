#include "Simulator.h"


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
	m_rDynamicFriction		= NxReal(0.4);
	m_rMaxAngularVelocity	= NxReal(2);
	m_rVehicleMass			= 10.0;
	m_rWheelRestLength		= 0.5;
	m_rMinWheelDisplacement = 1.25;
	m_rSpringK				= ( 10 * m_rVehicleMass ) / ( m_rWheelRestLength * 4 );
	m_rSpringC				= 2 * sqrt( m_rSpringK * m_rVehicleMass );
	m_rMaxWheelDisplacement = 1.0;
	m_rMaxWheelAngle		= 35.0;
	m_bPaused				= false;
	m_rSpringScale			= 1.0;
	m_rDamperScale			= 1.0;

	forward = false;
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
	addTerrainFromX( terrainMesh, NxVec3(0, 0, 0) );
	
	m_PhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
}


//--------------------------------------------------------------------------------------
// Function:  pause
// pause=true simulation is paused
//--------------------------------------------------------------------------------------
bool Simulator::pause( bool pause )
{
	if( pause == m_bPaused )
		return false;
	m_bPaused = pause;
	return true;
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
// Function:  simulate
// Simulates the movement of entities based on the elapsed time
//--------------------------------------------------------------------------------------
void Simulator::simulate( vector<Vehicle*> vehicles, double elapsedTime ) 
{
	if( m_bPaused )
		return;

	m_dDeltaTime = elapsedTime;

	startPhysics();
	getPhysicsResults();

	//Update all the entity positions based on PhysX simulated actor positions
	for( int i=0; i < m_Vehicles.size(); i++ )
	{
		processForceKeys(m_Vehicles[i], vehicles[i]);

		//Get the new position of the vehicle in vector and matrix format
		Matrix m;
		NxVec3 vec = m_Vehicles[i]->getGlobalPosition();
		m_Vehicles[i]->getGlobalPose().getColumnMajor44( m );
		
		NxVec3 vlc = m_Vehicles[i]->getLinearVelocity();

		//Update the vehicle position in the game
		vehicles[i]->update( Vec3(vec.x, vec.y, vec.z), Vec3(vlc.x, 0, vlc.z), m );
	}
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
void Simulator::processForceKeys(NxActor* actor, Vehicle* vehicle) 
{
	NxVec3 localWheelForce[4];
	NxVec3 globalWheelForce[4];

	for (int i = 0; i < 4; i++) 
		localWheelForce[i] = globalWheelForce[i] = NxVec3(0, 0, 0);
	
	bool* buttons = vehicle->getInputObj()->getButtons();

	//INPUT
	for( int i = 0; i < 6; i++ )
	{
		if( !buttons[i] ) { continue; } 

		switch( i )
		{
			case 0: //A_BUTTON - accelerate
			{
				//TODO: change tire orientation
				float x_dir = vehicle->getInputObj()->getThumbstick();
				//m_vForceVec = applyForceToActor( actor, NxVec3(x_dir,0,0), m_rForceStrength ); //temporarily

				localWheelForce[2] += NxVec3(0, 0, m_rVehicleMass * m_rForceStrength );
				localWheelForce[3] += NxVec3(0, 0, m_rVehicleMass * m_rForceStrength );
				break; 
			}
			case 1: //B_BUTTON - brake / reverse
			{
				NxVec3 velocity = actor->getLinearVelocity();

				//brake
				if (velocity.magnitude() > 1) 
				{
					velocity = normalize(velocity);
				
					globalWheelForce[0] += (velocity * ( -m_rVehicleMass * m_rForceStrength ));
					globalWheelForce[1] += (velocity * ( -m_rVehicleMass * m_rForceStrength ));
					globalWheelForce[2] += (velocity * ( -m_rVehicleMass * m_rForceStrength ));
					globalWheelForce[3] += (velocity * ( -m_rVehicleMass * m_rForceStrength ));
				}
				break; 
			}
			case 2: //X_BUTTON - reverse
			{
				localWheelForce[2] += NxVec3(0, 0, -m_rVehicleMass * m_rForceStrength );
				localWheelForce[3] += NxVec3(0, 0, -m_rVehicleMass * m_rForceStrength );
				break;
			}
			case 3: //Y_BUTTON - emergency brake
			{
				
			}
			case 4: //LT_BUTTON - emergency brake
			{
				actor->setLinearVelocity(NxVec3(0, 0, 0));
				actor->setAngularVelocity(NxVec3(0, 0, 0));
				break;
			}
			case 5: //RT_BUTTON - emergency brake
			{
				NxMat33 reset(NxVec3(1, 0, 0), NxVec3(0, 1, 0), NxVec3(0, 0, 1));
				actor->setLinearVelocity(NxVec3(0, 0, 0));
				actor->setAngularVelocity(NxVec3(0, 0, 0));
				actor->setGlobalOrientation(reset);
				actor->setGlobalPosition(NxVec3(0, 30, 120));
				break;
			}
		}
	}

	//STEERING
	float angle = vehicle->getInputObj()->getThumbstick() * m_rMaxWheelAngle; //maximum wheel angle

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

	//for (int i = WHEEL0; i < WHEEL2 /*front wheels*/; i++) 
	//{
	//	w = &vehicle->m_Wheels[ i ];
	//	
	//	wheelPos = mat * w->getChassisPt();
	//	upVec = normalize( orient * NxVec3( 0, 1, 0 ));
	//	
	//	susAxis = normalize( orient * NxVec3(0,-1,0) );
	//	w->setSuspensionAxis( susAxis );

	//	wheelForward = 
	//	
	//}
	/*NxVec3 force(-50000*sin(angle*PI/180), 0, -100000*sin(angle*PI/180));
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
	}*/

	//get the angle of the normal to the wheel direction
	if (angle > 0) {
		angle = angle + 90;
	}
	else {
		angle = angle - 90;
	}

	//find the x and z components of the tire lateral
	angle = 90 - angle;
	float x = cos(angle*PI/180);
	float z = sin(angle*PI/180);

	NxVec3 tireLateral(x, 0, 0);

	if (vehicle->getInputObj()->getThumbstick() != 0) 
	{
		localWheelForce[0] += (tireLateral * (m_rVehicleMass * m_rForceStrength) * sqrt(actor->getLinearVelocity().magnitude()));
		localWheelForce[1] += (tireLateral * (m_rVehicleMass * m_rForceStrength) * sqrt(actor->getLinearVelocity().magnitude()));

		//globalWheelForce[0] += (-actor->getLinearVelocity() * (m_rForceStrength/2500) * sqrt(actor->getLinearVelocity().magnitude()));
		//globalWheelForce[1] += (-actor->getLinearVelocity() * (m_rForceStrength/2500) * sqrt(actor->getLinearVelocity().magnitude()));
	}
	/*NxVec3 velocity = NxVec3(actor->getGlobalOrientation() * actor->getLinearVelocity());

	//project the tireLateral on the velocity of the car
	NxVec3 steering = (velocity.dot(tireLateral) / tireLateral.dot(tireLateral)) * tireLateral;
	actor->addLocalForceAtLocalPos(NxVec3(steering.x, 0, steering.z)*5000, wheel[0]);
	actor->addLocalForceAtLocalPos(NxVec3(steering.x, 0, steering.z)*5000, wheel[1]);*/

	vehicle->getInputObj()->reset();


	float damperForce,
			wheelRadius;


	//SUSPENSION
	for (int i = 0; i < 4 /*number of wheels*/; i++) 
	{
		w = &vehicle->m_Wheels[ i ];
		wheelRadius = w->getBoundingBox().m_fRadius;

		//Ray casting
		wheelPos = mat * (w->getChassisPt()+NxVec3(0,-BB.m_fHeight,0));
		upVec = normalize( orient * NxVec3( 0, 1, 0 ));

		susAxis = normalize( orient * NxVec3(0,-1,0) );
		w->setSuspensionAxis( susAxis );

		NxRay ray( wheelPos, susAxis );
		NxRaycastHit hit;

		m_Scene->raycastClosestShape( ray, NX_ALL_SHAPES, hit );

		//apply forces due to suspension
		if( hit.distance < ( m_rMaxWheelDisplacement + wheelRadius )) 
		{
			////the car is about to bottom out
			//if( hit.distance <= wheelRadius )
			//	w->setDisplacement( 0 );

			//else //the car is between the max length and its min length somewhere
			w->setDisplacement( hit.distance - wheelRadius );
				
			susForce =  -1*m_rSpringScale * m_rSpringK * ( hit.distance - wheelRadius - m_rWheelRestLength );

			NxVec3 ptVelocity = actor->getLocalPointVelocity( w->getChassisPt() );
			
			damperForce = m_rSpringC * m_rDamperScale * ptVelocity.dot( susAxis );
			
			localWheelForce[i] += NxVec3(0, susForce, 0) + NxVec3(0, damperForce, 0);

			actor->addLocalForceAtLocalPos(localWheelForce[i], w->getChassisPt() );
			actor->addForceAtLocalPos(globalWheelForce[i], w->getChassisPt() );
		}
		else
		{
			w->setDisplacement( m_rMaxWheelDisplacement );
		}
		/*actor->addLocalForceAtLocalPos(localWheelForce[i], w->getChassisPt() );
		actor->addForceAtLocalPos(globalWheelForce[i], w->getChassisPt() );*/
	}
	
}

NxVec3 Simulator::normalize(NxVec3 vec) {
	NxReal mag = vec.magnitude();
	return NxVec3(vec.x/mag, vec.y/mag, vec.z/mag);
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
	m_vDefaultGravity = NxVec3(gravity.x, gravity.y, gravity.z);
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