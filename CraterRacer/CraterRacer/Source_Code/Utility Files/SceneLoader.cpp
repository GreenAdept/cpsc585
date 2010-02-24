#include "SceneLoader.h"

//--------------------------------------------------------------------------------------
// Function:  Constructor
//--------------------------------------------------------------------------------------
SceneLoader::SceneLoader( )
{
	m_Game = NULL;
	m_BackSurface = NULL;
	m_Device = NULL;
}


//--------------------------------------------------------------------------------------
// Function:  Desctructor
//--------------------------------------------------------------------------------------
SceneLoader::~SceneLoader( )
{
	if( m_BackSurface )
		delete m_BackSurface;
}


//--------------------------------------------------------------------------------------
// Function:  initScene
// This function is used to load and initialize components that are common to all game 
// modes.  This function must be called before startGame().
//--------------------------------------------------------------------------------------
void SceneLoader::initScene( Device* device, const D3DSURFACE_DESC* backSurface, string filename )
{
	string str;

	m_Game = new GameObj();
	m_Objs = m_Game->getSceneObjects( );

	m_Device = device;
	m_BackSurface = new D3DSURFACE_DESC( *backSurface );

	// Clear debug.txt
	m_Objs.debugger->clearFile();

	ifstream file;
	file.open( filename.c_str() );
	if (!file.is_open()) return;

	//load and initialize components that are common to all game modes here
	file >> str;
	processTerrainInfo( file );
	file >> str;
	processPathInfo( file );
	
	initializeSimulator( );

	file.close();
}


//--------------------------------------------------------------------------------------
// Function:  startGame
// This function is to be called after the initScene function call.  It loads all
// game-type specific components ( eg. vehicles ). 
//--------------------------------------------------------------------------------------
GameObj* SceneLoader::startGame( string filename )
{
	string str;
	if( !m_Game ) return NULL;

	ifstream file;
	file.open( filename.c_str() );
	if (!file.is_open()) return NULL;

	//do all scene specific loading & initialization here
	file >> str;
	processVehicleInfo( file );

	file.close();
	m_Game->setSceneObjects( m_Objs );
	return m_Game;
}


//--------------------------------------------------------------------------------------
// Function:  processTerrainInfo
//--------------------------------------------------------------------------------------
void SceneLoader::processTerrainInfo( ifstream& file )
{
	string	terrainFile, 
			terrainEffectFile,
			flush;

	file >> flush >> terrainFile;
	if( terrainFile != "MESH_FILENAME" ) return;
	file >> terrainFile;

	file >> terrainEffectFile;
	if( terrainEffectFile != "EFFECT_FILENAME" ) return;
	file >> terrainEffectFile;

	// Create the terrain in the Entity Manager
	m_Objs.entityManager->makeTerrain( m_Device, Vec3(0, 0, 0), toLPCWSTR(terrainFile).c_str(), toLPCWSTR(terrainEffectFile).c_str() );
}


//--------------------------------------------------------------------------------------
// Function:  processPathInfo
//--------------------------------------------------------------------------------------
void SceneLoader::processPathInfo( ifstream& file )
{
	string	str,
			flush;
	int		num_waypoints;
	int		x, y, z;

	file >> flush >> str;
	if( str != "NUM_WAYPOINTS" ) return;
	file >> num_waypoints;

	Vec3* waypoints = new Vec3 [num_waypoints];
	for (int i=0; i<num_waypoints; i++) {
		file >> x >> y >> z;
		waypoints[i] = Vec3 (x, y, z);
	}

	m_Objs.entityManager->getTerrain()->buildTrack (waypoints, num_waypoints);
	delete[] waypoints;
}


//--------------------------------------------------------------------------------------
// Function:  processVehicleInfo
// Processes both player and computer vehicles.
//--------------------------------------------------------------------------------------
void SceneLoader::processVehicleInfo( ifstream& file )
{
	string mesh;
	string flush;
	int numPlayers;
	int numComputers;
	
	file >> flush >> flush;
	if( flush != "MESH_FILENAME" ) return;
	file >> mesh >> flush;

	file >> numPlayers >> flush;
	file >> numComputers;

	processPlayerVehicles( file, toLPCWSTR(mesh).c_str(), numPlayers );
	processComputerVehicles( file, toLPCWSTR(mesh).c_str(), numComputers );
}


//--------------------------------------------------------------------------------------
// Function:  processPlayerVehicles
// Creates and adds vehicles to game based on information loaded from the specified file.
//--------------------------------------------------------------------------------------
void SceneLoader::processPlayerVehicles( ifstream& file, LPCWSTR meshL, int numPlayers )
{
	string flush, effect;
	Vec3 pos;

	file >> flush;
	if( flush != "**PLAYER_VEHICLES" ) return;
	file >> flush;

	Vehicle * pv;
	for( int i=0; i < numPlayers; i++ )
	{
		//load effect filename and position from file
		file >> flush >> effect >> flush;
		file >> pos.x >> pos.y >> pos.z;

		//create vehicle in entity manager
		pv = m_Objs.entityManager->makePlayer( m_Device, pos, meshL, toLPCWSTR(effect).c_str() ); 
	
		//create a camera to go along with this player
		m_Objs.gameCameras.push_back( new GameCamera() );
		m_Objs.gameCameras[i]->updateWindow( m_BackSurface );
		m_Objs.gameCameras[i]->setTarget( pv );

		//create a controller to go along with player
		m_Objs.controllers.push_back( new XBox360Controller(i) ); 

		//add to simulator
		m_Objs.simulator->createVehicle( pv->getRenderable()->m_pMesh, pv->getPosition(), pv->getBoundingBox()  );
	}
}


//--------------------------------------------------------------------------------------
// Function:  processComputerVehicles
// Creates computer vehicles from file information and adds them to game.
//--------------------------------------------------------------------------------------
void SceneLoader::processComputerVehicles( ifstream& file, LPCWSTR meshL, int numComputers )
{
	string flush, effect;
	Vec3 pos;

	file >> flush;
	if( flush != "**COMPUTER_VEHICLES" ) return;
	file >> flush;

	Vehicle * av;
	for( int i=0; i < numComputers; i++ )
	{
		//load effect filename and position from file
		file >> flush >> effect >> flush;
		file >> pos.x >> pos.y >> pos.z;

		//create in Entity Manager
		av = m_Objs.entityManager->makeComputer( m_Device, pos, meshL, toLPCWSTR(effect).c_str() ); 
		
		//add to simulator
		m_Objs.simulator->createVehicle( av->getRenderable()->m_pMesh, av->getPosition(), av->getBoundingBox() );
	}
}


//--------------------------------------------------------------------------------------
// Function:  initializeSimulator
// Initializes PhysX with variables loaded from the game VarLoader and with the
// terrain already created.
//--------------------------------------------------------------------------------------
void SceneLoader::initializeSimulator( )
{	
	//load the simulator values from file
	m_Objs.varLoader->loadVars( m_Objs.simulator );

	//initialize simulator with terrain
	Terrain* t = m_Objs.entityManager->getTerrain();
	if( !t ) return;

	m_Objs.simulator->InitNx( t->getRenderable()->m_pMesh );
}


//--------------------------------------------------------------------------------------
// Function:  toLPCWSTR
// Converts string to wide byte string
//--------------------------------------------------------------------------------------
std::wstring SceneLoader::toLPCWSTR( std::string& s )
{
	 int len;
	 int slength = (int)s.length() + 1;
	 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	 wchar_t* buf = new wchar_t[len];
	 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	 std::wstring r(buf);
	 delete[] buf;

	 return r; 
}

