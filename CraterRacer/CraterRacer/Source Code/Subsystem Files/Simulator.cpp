#include "Simulator.h"

// Physics SDK globals
NxPhysicsSDK*     gPhysicsSDK = NULL;
NxScene*          gScene = NULL;
NxVec3            gDefaultGravity(0,2,0);

//Force globals
NxVec3 gForceVec(0, 0, 0);
NxReal gForceStrength = 100;
bool bForceMode = true;

// Keyboard globals
#define MAX_KEYS 256
bool gKeys[MAX_KEYS];

//Time global
double deltaTime;

//Actor globals
NxActor* groundPlane = NULL;
NxActor* gSelectedActor = NULL;
vector< NxActor* > gActors;
Vec3 boxPos;
double boxSize;

bool* input = new bool[4];

//Debugging
DebugWriter debug;


Simulator::Simulator() 
{
	InitNx();
}


void Simulator::simulate( vector<Entity*> entities, double elapsedTime ) 
{
	NxF32* mat;
	deltaTime = elapsedTime;
	startPhysics();
	getPhysicsResults();

	//Update all the entity positions based on PhysX simulated actor positions
	for( int i=0; i < gActors.size(); i++ )
	{
		input = entities[0]->getInput();
		processForceKeys();

		entities[0]->resetInput();

		NxVec3 vec = gActors[i]->getGlobalPosition();
		gActors[i]->getGlobalPose().getRowMajor44( mat );

		entities[0]->update( Vec3(vec.x, vec.y, vec.z) );//, (Matrix*)&mat[0] );
		debug.writeToFile(vec);

	}
}


void Simulator::InitNx( void ) 
{
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
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);

	//Create the ground
	groundPlane = createGroundPlane( );

	input[0] = false;
	input[1] = false;
	input[2] = false;
	input[3] = false;
}


NxActor* Simulator::createGroundPlane() 
{
	NxPlaneShapeDesc planeDesc;
	NxActorDesc actorDesc;
	//planeDesc.normal = NxVec3(0, -1, 0);
	planeDesc.d = -8;
	actorDesc.shapes.pushBack(&planeDesc);
	return gScene->createActor(actorDesc);
}


NxActor* Simulator::createBox(Vec3 pos, double size) {
	//The height of the box
	NxReal boxStartHeight = 3.5;

	//Add a single shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	//The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	//boxDesc.dimensions.set(0.5, 0.5, 0.5);
	boxDesc.dimensions.set(size, size, size);
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density = 10.0f;
	//actorDesc.globalPose.t = NxVec3(0, boxStartHeight, 0);
	actorDesc.globalPose.t = NxVec3(pos.x, pos.y, pos.z);
	assert(actorDesc.isValid());
	NxActor* pActor = gScene->createActor(actorDesc);
	assert(pActor);

	gSelectedActor = pActor;

	return pActor;
}


void Simulator::startPhysics() 
{
	gScene->simulate(deltaTime);
	gScene->flushStream();
}


void Simulator::getPhysicsResults() 
{
	while (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, true));
	processInput();
}


void Simulator::processInput()
{
	processForceKeys();
}


void Simulator::processForceKeys() {
	// Process force keys
	/*
	for (int i = 0; i < MAX_KEYS; i++)
	{	
		if (!gKeys[i])  { continue; }

		switch (i)
		{
			// Force controls
			case 'a': { gForceVec = applyForceToActor(gSelectedActor,NxVec3(0,0,1),gForceStrength);		break; }
			case 'b': { gForceVec = applyForceToActor(gSelectedActor,NxVec3(0,0,-1),gForceStrength);	break; }
			case 'c': { gForceVec = applyForceToActor(gSelectedActor,NxVec3(1,0,0),gForceStrength);		break; }
			case 'd': { gForceVec = applyForceToActor(gSelectedActor,NxVec3(-1,0,0),gForceStrength);	break; }
			case 'u': { gForceVec = applyForceToActor(gSelectedActor,NxVec3(0,1,0),gForceStrength);		break; }
			case 'm': { gForceVec = applyForceToActor(gSelectedActor,NxVec3(0,-1,0),gForceStrength);	break; }

		    // Return box to (0,3.5,0)
			case 't': 
			{ 
				if (gSelectedActor) 
				{
					gSelectedActor->setGlobalPosition(NxVec3(0,3.5,0)); 
					gScene->flushCaches();
				}
				break; 
			}
		}
	}
	*/
	
	for (int i = 0; i < 4; i++)
	{
		if (!input[i]) { continue; } 
		switch (i)
		{
			case 0: { gForceVec = applyForceToActor(gSelectedActor,NxVec3(0.1,0,0),gForceStrength);
				debug.writeToFile("left");
				break; }
			case 1: { gForceVec = applyForceToActor(gSelectedActor,NxVec3(0,0.1,0),gForceStrength); break; }
			case 2: { gForceVec = applyForceToActor(gSelectedActor,NxVec3(-0.1,0,0),gForceStrength); break; }
			case 3: { gForceVec = applyForceToActor(gSelectedActor,NxVec3(0,-0.1,0),gForceStrength); break; }
		}
	}
	
}

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
	for( int i=0; i < gActors.size(); i++ )
	{
		gScene->releaseActor( *gActors[i] );
		gActors[i] = NULL;
	}
	
	//now release the scene and physics SDK
	if(gPhysicsSDK != NULL)
	{
		if(gScene != NULL) gPhysicsSDK->releaseScene(*gScene);
		gScene = NULL;
		NxReleasePhysicsSDK(gPhysicsSDK);
		gPhysicsSDK = NULL;
	}
}


//--------------------------------------------------------------------------------------
// Function:  addActor
// Creates an actor under PhysX simulation based on specified mesh and position.  The
// actor is added to the the list of all scene objects.
//--------------------------------------------------------------------------------------
void Simulator::addActor( Mesh* mesh, Vec3& pos )
{
	gActors.push_back( createMeshActor( mesh, pos ) );
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
	gSelectedActor = pActor;
	return pActor;
}


// User report globals
//DebugRenderer     gDebugRenderer;

// HUD globals
//HUD hud;
 
// Display globals
/*int gMainHandle;
int mx = 0;
int my = 0;*/

/*// Camera globals
float	gCameraAspectRatio = 1.0f;
NxVec3	gCameraPos(0,5,-15);
NxVec3	gCameraForward(0,0,1);
NxVec3	gCameraRight(-1,0,0);
const NxReal gCameraSpeed = 10;*/

// Force globals
/*NxVec3	gForceVec(0,0,0);
NxReal	gForceStrength	= 20000;
bool	bForceMode		= true;*/

// Keyboard globals
/*#define MAX_KEYS 256
bool gKeys[MAX_KEYS];

// Simulation globals
NxReal	gDeltaTime			= 1.0/60.0;
bool	bHardwareScene		= false;
bool	bPause				= false;
bool	bShadows			= true;
bool	bDebugWireframeMode = false;*/

// Actor globals
/*NxActor* groundPlane = NULL;

// Focus actor
NxActor* gSelectedActor = NULL;*/

/*void PrintControls()
{
	printf("\n Flight Controls:\n ----------------\n w = forward, s = back\n a = strafe left, d = strafe right\n q = up, z = down\n");
    printf("\n Force Controls:\n ---------------\n i = +z, k = -z\n j = +x, l = -x\n u = +y, m = -y\n");
	printf("\n Miscellaneous:\n --------------\n p   = Pause\n b   = Toggle Debug Wireframe Mode\n x   = Toggle Shadows\n r   = Select Actor\n F10 = Reset scene\n");
}

NxVec3 ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength)
{
	NxVec3 forceVec = forceStrength*forceDir*gDeltaTime;
	actor->addForce(forceVec);
	return forceVec;
}

void ProcessCameraKeys()
{
	NxReal deltaTime;

    if (bPause) 
	{
		deltaTime = 0.0005;
	}
	else 
	{
		deltaTime = gDeltaTime;
	}
		
	// Process camera keys
	for (int i = 0; i < MAX_KEYS; i++)
	{	
		if (!gKeys[i])  { continue; }

		switch (i)
		{
			// Camera controls
			case 'w':{ gCameraPos += gCameraForward*gCameraSpeed*deltaTime; break; }
			case 's':{ gCameraPos -= gCameraForward*gCameraSpeed*deltaTime; break; }
			case 'a':{ gCameraPos -= gCameraRight*gCameraSpeed*deltaTime;	break; }
			case 'd':{ gCameraPos += gCameraRight*gCameraSpeed*deltaTime;	break; }
			case 'z':{ gCameraPos -= NxVec3(0,1,0)*gCameraSpeed*deltaTime;	break; }
			case 'q':{ gCameraPos += NxVec3(0,1,0)*gCameraSpeed*deltaTime;	break; }
		}
	}
}

void SetupCamera()
{
	gCameraAspectRatio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
	
	// Setup camera
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, gCameraAspectRatio, 1.0f, 10000.0f);
	gluLookAt(gCameraPos.x,gCameraPos.y,gCameraPos.z,gCameraPos.x + gCameraForward.x, gCameraPos.y + gCameraForward.y, gCameraPos.z + gCameraForward.z, 0.0f, 1.0f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderActors(bool shadows)
{
    // Render all the actors in the scene
    NxU32 nbActors = gScene->getNbActors();
    NxActor** actors = gScene->getActors();
    while (nbActors--)
    {
        NxActor* actor = *actors++;
        DrawActor(actor, gSelectedActor, false);

        // Handle shadows
        if (shadows)
        {
			DrawActorShadow(actor, false);
        }
    }
}

void DrawForce(NxActor* actor, NxVec3& forceVec, const NxVec3& color)
{
	// Draw only if the force is large enough
	NxReal force = forceVec.magnitude();
	if (force < 0.1)  return;

	forceVec = 3*forceVec/force;

	NxVec3 pos = actor->getCMassGlobalPosition();
	DrawArrow(pos, pos + forceVec, color);
}

bool IsSelectable(NxActor* actor)
{
   NxShape*const* shapes = gSelectedActor->getShapes();
   NxU32 nShapes = gSelectedActor->getNbShapes();
   while (nShapes--)
   {
       if (shapes[nShapes]->getFlag(NX_TRIGGER_ENABLE)) 
       {           
           return false;
       }
   }

   if(!actor->isDynamic())
	   return false;

   if (actor == groundPlane)
       return false;

   return true;
}

void SelectNextActor()
{
   NxU32 nbActors = gScene->getNbActors();
   NxActor** actors = gScene->getActors();
   for(NxU32 i = 0; i < nbActors; i++)
   {
       if (actors[i] == gSelectedActor)
       {
           NxU32 j = 1;
           gSelectedActor = actors[(i+j)%nbActors];
           while (!IsSelectable(gSelectedActor))
           {
               j++;
               gSelectedActor = actors[(i+j)%nbActors];
           }
           break;
       }
   }
}

void ProcessForceKeys()
{
	// Process force keys
	for (int i = 0; i < MAX_KEYS; i++)
	{	
		if (!gKeys[i])  { continue; }

		switch (i)
		{
			// Force controls
			case 'i': { gForceVec = ApplyForceToActor(gSelectedActor,NxVec3(0,0,1),gForceStrength);		break; }
			case 'k': { gForceVec = ApplyForceToActor(gSelectedActor,NxVec3(0,0,-1),gForceStrength);	break; }
			case 'j': { gForceVec = ApplyForceToActor(gSelectedActor,NxVec3(1,0,0),gForceStrength);		break; }
			case 'l': { gForceVec = ApplyForceToActor(gSelectedActor,NxVec3(-1,0,0),gForceStrength);	break; }
			case 'u': { gForceVec = ApplyForceToActor(gSelectedActor,NxVec3(0,1,0),gForceStrength);		break; }
			case 'm': { gForceVec = ApplyForceToActor(gSelectedActor,NxVec3(0,-1,0),gForceStrength);	break; }

		    // Return box to (0,5,0)
			case 't': 
			{ 
				if (gSelectedActor) 
				{
					gSelectedActor->setGlobalPosition(NxVec3(0,5,0)); 
					gScene->flushCaches();
				}
				break; 
			}
		}
	}
}

void ProcessInputs()
{
    ProcessForceKeys();

    // Show debug wireframes
	if (bDebugWireframeMode)
	{
		if (gScene)  gDebugRenderer.renderData(*gScene->getDebugRenderable());
	}
}

void RenderCallback()
{
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ProcessCameraKeys();
	SetupCamera();

    if (gScene && !bPause)
	{
		GetPhysicsResults();
        ProcessInputs();
		StartPhysics();
	}

    // Display scene
 	RenderActors(bShadows);

	DrawForce(gSelectedActor, gForceVec, NxVec3(1,1,0));
	gForceVec = NxVec3(0,0,0);

	// Render the HUD
	hud.Render();

    glFlush();
    glutSwapBuffers();
}

void ReshapeCallback(int width, int height)
{
    glViewport(0, 0, width, height);
    gCameraAspectRatio = float(width)/float(height);
}

void IdleCallback()
{
    glutPostRedisplay();
}

void KeyboardCallback(unsigned char key, int x, int y)
{
	gKeys[key] = true;

	switch (key)
	{
		case 'r':	{ SelectNextActor(); break; }
		default:	{ break; }
	}
}

void KeyboardUpCallback(unsigned char key, int x, int y)
{
	gKeys[key] = false;

	switch (key)
	{
		case 'p': 
		{ 
			bPause = !bPause; 
			if (bPause)
				hud.SetDisplayString(0, "Paused - Hit \"p\" to Unpause", 0.3f, 0.55f);
			else
				hud.SetDisplayString(0, "", 0.0f, 0.0f);	
			UpdateTime(); 
			break; 
		}
		case 'x': { bShadows = !bShadows; break; }
		case 'b': { bDebugWireframeMode = !bDebugWireframeMode; break; }
		case 27 : { exit(0); break; }
		default : { break; }
	}
}

void SpecialCallback(int key, int x, int y)
{
	switch (key)
    {
		// Reset PhysX
		case GLUT_KEY_F10: ResetNx(); return; 
	}
}

void MouseCallback(int button, int state, int x, int y)
{
    mx = x;
    my = y;
}

void MotionCallback(int x, int y)
{
    int dx = mx - x;
    int dy = my - y;
    
    gCameraForward.normalize();
    gCameraRight.cross(gCameraForward,NxVec3(0,1,0));

    NxQuat qx(NxPiF32 * dx * 20 / 180.0f, NxVec3(0,1,0));
    qx.rotate(gCameraForward);
    NxQuat qy(NxPiF32 * dy * 20 / 180.0f, gCameraRight);
    qy.rotate(gCameraForward);

    mx = x;
    my = y;
}

void ExitCallback()
{
	ReleaseNx();
}

void InitGlut(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(512, 512);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    gMainHandle = glutCreateWindow("Lesson 101: Primary Shape");
    glutSetWindow(gMainHandle);
    glutDisplayFunc(RenderCallback);
    glutReshapeFunc(ReshapeCallback);
    glutIdleFunc(IdleCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutKeyboardUpFunc(KeyboardUpCallback);
	glutSpecialFunc(SpecialCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback);
	MotionCallback(0,0);
	atexit(ExitCallback);

    // Setup default render states
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);

    // Setup lighting
    glEnable(GL_LIGHTING);
    float AmbientColor[]    = { 0.0f, 0.1f, 0.2f, 0.0f };         glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientColor);
    float DiffuseColor[]    = { 0.2f, 0.2f, 0.2f, 0.0f };         glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseColor);
    float SpecularColor[]   = { 0.5f, 0.5f, 0.5f, 0.0f };         glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularColor);
    float Position[]        = { 100.0f, 100.0f, -400.0f, 1.0f };  glLightfv(GL_LIGHT0, GL_POSITION, Position);
    glEnable(GL_LIGHT0);
}

NxActor* CreateGroundPlane()
{
    // Create a plane with default descriptor
    NxPlaneShapeDesc planeDesc;
    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&planeDesc);
    return gScene->createActor(actorDesc);
}

NxActor* CreateBox()
{
	// Set the box starting height to 3.5m so box starts off falling onto the ground
	NxReal boxStartHeight = 3.5; 

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(0.5,0.5,0.5);
	boxDesc.localPose.t = NxVec3(0, 0, 0);
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t	= NxVec3(0,boxStartHeight,0);	
	assert(actorDesc.isValid());
	NxActor *pActor = gScene->createActor(actorDesc);	
	assert(pActor);

	// //create actor with no shapes
	//NxShape* const *shape = pActor->getShapes();
	//NxBoxShape *boxShape = shape[0]->isBox();
	//assert(boxShape);
	//pActor->releaseShape(*boxShape);

	return pActor;
}

NxActor* CreateSphere()
{
	// Set the sphere starting height to 3.5m so box starts off falling onto the ground
	NxReal sphereStartHeight = 3.5; 

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a sphere, 1m on radius
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius		= 0.65f;
	sphereDesc.localPose.t	= NxVec3(0, 0, 0);

	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t	= NxVec3(3.0f,sphereStartHeight,0);	
	return gScene->createActor(actorDesc);
}


NxActor* CreateCapsule()
{
	// Set the capsule starting height to 3.5m so box starts off falling onto the ground
	NxReal capsuleStartHeight = 3.5; 

	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a sphere, 1m on radius
	NxCapsuleShapeDesc capsuleDesc;
	capsuleDesc.radius		= 0.55f;
	capsuleDesc.height		= 0.75f;
	capsuleDesc.localPose.t = NxVec3(0, 0, 0);
	
	//Rotate capsule shape
	NxQuat quat(45, NxVec3(0, 0, 1));
	NxMat33 m33(quat);
	capsuleDesc.localPose.M = m33;

	actorDesc.shapes.pushBack(&capsuleDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t	= NxVec3(6.0f,capsuleStartHeight,0);

	////Rotate actor
	//NxQuat quat1(45, NxVec3(1, 0, 0));
	//NxMat33 m331(quat1);
	//actorDesc.globalPose.M = m331;

	return gScene->createActor(actorDesc);
}

void InitializeHUD()
{
	bHardwareScene = (gScene->getSimType() == NX_SIMULATION_HW);

	hud.Clear();

	//// Add hardware/software to HUD
	//if (bHardwareScene)
	//    hud.AddDisplayString("Hardware Scene", 0.74f, 0.92f);
	//else
	//	hud.AddDisplayString("Software Scene", 0.74f, 0.92f);

	// Add pause to HUD
	if (bPause)  
		hud.AddDisplayString("Paused - Hit \"p\" to Unpause", 0.3f, 0.55f);
	else
		hud.AddDisplayString("", 0.0f, 0.0f);
}

void InitNx()
{
	// Initialize camera parameters
	gCameraAspectRatio	= 1.0f;
	gCameraPos			= NxVec3(0,5,-15);
	gCameraForward		= NxVec3(0,0,1);
	gCameraRight		= NxVec3(-1,0,0);
	
	// Create the physics SDK
    gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
    if (!gPhysicsSDK)  return;

	// Set the physics parameters
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01);

	// Set the debug visualization parameters
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

    // Create the scene
    NxSceneDesc sceneDesc;
 	sceneDesc.simType				= NX_SIMULATION_SW;
    sceneDesc.gravity               = gDefaultGravity;
    gScene = gPhysicsSDK->createScene(sceneDesc);	
	if(!gScene)
	{ 
		sceneDesc.simType			= NX_SIMULATION_SW; 
		gScene = gPhysicsSDK->createScene(sceneDesc);  
		if(!gScene) return;
	}


	// Create the default material
	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);

	// Create the objects in the scene
	groundPlane		= CreateGroundPlane();
	gSelectedActor	= CreateBox();
	CreateSphere();
	CreateCapsule();

	// Initialize HUD
	InitializeHUD();

	// Get the current time
	UpdateTime();

	// Start the first frame of the simulation
	if (gScene)  StartPhysics();
}

void ReleaseNx()
{
    if (gScene)
	{
		GetPhysicsResults();  // Make sure to fetchResults() before shutting down
		gPhysicsSDK->releaseScene(*gScene);
	}
	if (gPhysicsSDK)  gPhysicsSDK->release();
}

void ResetNx()
{
	ReleaseNx();
	InitNx();
}

void StartPhysics()
{
	// Update the time step
	gDeltaTime = UpdateTime();

	// Start collision and dynamics for delta time since the last frame
    gScene->simulate(gDeltaTime);
	gScene->flushStream();
}

void GetPhysicsResults()
{
	// Get results from gScene->simulate(gDeltaTime)
	while (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
}

int main(int argc, char** argv)
{
	PrintControls();
    InitGlut(argc, argv);
    InitNx();
    glutMainLoop();
	ReleaseNx();
	return 0;
}*/
