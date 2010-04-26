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

SceneObjects::SceneObjects( )
{
	entityManager = 0;
	simulator = 0;
	renderer = 0;
	debugger = 0;
	varLoader = 0;
}


//--------------------------------------------------------------------------------------
// Function:  Desctructor
//--------------------------------------------------------------------------------------
SceneLoader::~SceneLoader( )
{
	if( m_BackSurface )
		delete m_BackSurface;

	for( int i=0; i < m_Ramps.size(); i++ )
		delete m_Ramps[i];

	/*if( m_InnerTerrain )
		delete m_InnerTerrain;*/
}


//--------------------------------------------------------------------------------------
// Function:  initVars
//--------------------------------------------------------------------------------------
void SceneLoader::initVars( Device* device, const D3DSURFACE_DESC* backSurface, string filename )
{
	m_Device = device;
	m_InitFilename = filename;
	if( m_BackSurface )
		delete m_BackSurface;
	m_BackSurface = new D3DSURFACE_DESC( *backSurface );
}


//--------------------------------------------------------------------------------------
// Function:  initScene
// This function is used to load and initialize components that are common to all game 
// modes.  This function must be called before startGame().
//--------------------------------------------------------------------------------------
void SceneLoader::initScene( GameObj** obj )
{
	string str;

	if( m_Game )
		delete m_Game;

	m_Game = new GameObj();
	*obj = m_Game;
	m_Objs = m_Game->getSceneObjects( );

	// Clear debug.txt
	m_Objs.debugger->clearFile();

	ifstream file;
	file.open( m_InitFilename.c_str() );
	if (!file.is_open()) return;

	//load and initialize components that are common to all game modes here
	file >> str;
	processTerrainInfo( file );
	initializeSimulator( );
	file >> str;
	processPathInfo( file );
	file >> str;
	processCraterInfo( file );
	file >> str;
	processMeteorInfo( file );
	file >> str;
	processPropInfo( file );
	file >> str;

	file.close();
}


//--------------------------------------------------------------------------------------
// Function:  startGame
// This function is to be called after the initScene function call.  It loads all
// game-type specific components ( eg. vehicles ). 
//--------------------------------------------------------------------------------------
void SceneLoader::startGame( string filename )
{
	string str;
	if( !m_Game ) return;

	if (filename == "Source_Code\\Scene_Files\\TimeTrial.SCENE"){
		ifstream fin("times.txt");
		string s;
		if (fin.is_open()) {
			fin >> s; //read the best time from the file
			fin.close();

			if (s == "")
				s = "05:00:00";
		}
		else
			s = "05:00:00";

		m_Game->setTimeLimit(s);
	}

	ifstream file;
	file.open( filename.c_str() );
	if (!file.is_open()) return;

	//do all scene specific loading & initialization here
	file >> str;
	processVehicleInfo( file );

	file.close();
	m_Game->setSceneObjects( m_Objs );
	return;
}


//--------------------------------------------------------------------------------------
// Function:  processTerrainInfo
//--------------------------------------------------------------------------------------
void SceneLoader::processTerrainInfo( ifstream& file )
{
	string	terrainFile1, terrainFile2, 
			terrainEffectFile, rampFile,
			flush;
	int		laps, numRamps;

	file >> flush >> terrainFile1;
	if( terrainFile1 != "MESH_FILENAME1" ) return;
	file >> terrainFile1;
	
	file >> terrainFile2;
	if( terrainFile2 != "MESH_FILENAME2" ) return;
	file >> terrainFile2;

	file >> terrainEffectFile;
	if( terrainEffectFile != "EFFECT_FILENAME" ) return;
	file >> terrainEffectFile;

	// Create the terrain in the Entity Manager
	//m_Objs.entityManager->makeTerrain( m_Device, Vec3(0, 0, 0), toLPCWSTR(terrainFile1).c_str(), toLPCWSTR(terrainFile2).c_str(), toLPCWSTR(terrainEffectFile).c_str() );
	m_Objs.entityManager->makeTerrain( m_Device, Vec3(0, 0, 0), toLPCWSTR(terrainFile1).c_str(), toLPCWSTR(terrainEffectFile).c_str() );
	m_Objs.entityManager->makeTerrain( m_Device, Vec3(0, 0, 0), toLPCWSTR(terrainFile2).c_str(), toLPCWSTR(terrainEffectFile).c_str() );
	
	file >> terrainFile2;
	if( terrainFile2 != "TERRAIN_INNER2_FILENAME" ) return;
	file >> terrainFile2;

	this->m_InnerTerrain = new Terrain();
	m_InnerTerrain->initialize( m_Device, Vec3(0,0,0), toLPCWSTR(terrainFile2).c_str(), toLPCWSTR(terrainEffectFile).c_str() );
	
	file >> flush;
	file >> numRamps;

	Mesh* m;
	for( int i=0; i < numRamps; i++ )
	{
		file >> flush;
		file >> rampFile;
		m = new Mesh( toLPCWSTR(rampFile).c_str() );
		m->Create( m_Device, toLPCWSTR(rampFile).c_str() );
		this->m_Ramps.push_back( m );
	}


}


//--------------------------------------------------------------------------------------
// Function:  processPathInfo
//--------------------------------------------------------------------------------------
void SceneLoader::processPathInfo( ifstream& file )
{
	string	str,
			flush;
	int		num_waypoints;
	float	x, y, z;

	file >> flush >> str;
	if( str != "NUM_WAYPOINTS" ) return;
	file >> num_waypoints;

	AIPath* path = m_Objs.entityManager->getTerrain(0)->getTrack();
	for (int i=0; i<num_waypoints; i++) {
		file >> x >> y >> z;
		path->addWaypoint (x, y, z);

		file >> str;
		while (str != "DONE") {
			file >> x >> y >> z;
			path->addPositionToLastWP (x, y, z);
			file >> str;
		}
	}
}

//--------------------------------------------------------------------------------------
// Function:  processMeteorInfo
//--------------------------------------------------------------------------------------
void SceneLoader::processMeteorInfo( ifstream& file )
{
	string	str, flush;
	string	mesh, effect;
	int		numMeteorGroups, meteorGroupID, numMeteors, craterIndex, spawnOnLap, spawnProb;
	float	x, y, z, radius;

	file >> flush >> str;
	if( str != "MESH_FILENAME" ) return;
	file >> mesh;

	file >> str;
	if( str != "EFFECT_FILENAME" ) return;
	file >> effect;

	file >> str;
	if( str != "PROB_TO_SPAWN" ) return;
	file >> spawnProb;

	file >> str;
	if( str != "NUM_METEOR_GROUPS" ) return;
	file >> numMeteorGroups;

	for (int i = 0; i < numMeteorGroups; i++) {
		file >> str;
		if( str != "METEOR_GROUP" ) return;
		file >> meteorGroupID;

		file >> str;
		if( str != "POSITION" ) return;
		file >> x >> y >> z;

		file >> str;
		if( str != "RADIUS" ) return;
		file >> radius;

		file >> str;
		if( str != "SPAWN_ON_LAP" ) return;
		file >> spawnOnLap;

		file >> str;
		if( str != "NUM_METEORS" ) return;
		file >> numMeteors;

		MeteorGroup* meteorGroup = m_Objs.entityManager->makeMeteorGroup (Vec3(x,y,z), meteorGroupID, numMeteors, radius, spawnOnLap);
		meteorGroup->setMeteorSpawnProbability (spawnProb);

		for (int j = 0; j < numMeteors; j++) {
			file >> x >> y >> z;
			Meteor* m = m_Objs.entityManager->makeMeteor (m_Device, Vec3(x,y,z), toLPCWSTR(mesh).c_str(), toLPCWSTR(effect).c_str());
			file >> x >> y >> z;
			m->setTarget (Vec3 (x, y, z));

			file >> craterIndex;
			m->setCraterToSpawn (m_Objs.entityManager->getCrater(craterIndex));

			meteorGroup->addMeteor (j, m);
		}
	}
}

void SceneLoader::processCraterInfo( ifstream& file ) {
	string	str,
			flush,
			mesh,
			effect;
	int		num_craters;
	Vec3    pos;

	file >> flush >> str;
	if( str != "NUM_CRATERS" ) return;
	file >> num_craters;

	for (int i=0; i<num_craters; i++) {
		file >> str;
		if (str != "MESH_FILENAME") return;
		file >> mesh;

		file >> str;
		if (str != "EFFECT_FILENAME") return;
		file >> effect;

		file >> str;
		if (str != "POSITION") return;
		file >> pos.x >> pos.y >> pos.z;

		Crater* c = m_Objs.entityManager->makeCrater( m_Device, pos, toLPCWSTR(mesh).c_str(), toLPCWSTR(effect).c_str() );
		m_Objs.simulator->createCrater( c );
	}
}
void SceneLoader::processPropInfo( ifstream& file ) {
	string	str,
			flush,
			mesh,
			effect;
	int		num_props;
	Vec3    pos;

	file >> flush >> str;
	if( str != "NUM_PROPS" ) return;
	file >> num_props;

	for (int i=0; i<num_props; i++) {
		file >> str;
		if (str != "MESH_FILENAME") return;
		file >> mesh;

		file >> str;
		if (str != "EFFECT_FILENAME") return;
		file >> effect;

		file >> str;
		if (str != "POSITION") return;
		file >> pos.x >> pos.y >> pos.z;

		m_Objs.entityManager->makeProp( m_Device, pos, toLPCWSTR(mesh).c_str(), toLPCWSTR(effect).c_str() ); 
	}
}

//--------------------------------------------------------------------------------------
// Function:  processVehicleInfo
// Processes both player and computer vehicles.
//--------------------------------------------------------------------------------------
void SceneLoader::processVehicleInfo( ifstream& file )
{
	string mesh;
	string flush;
	//string timeLimit;
	int numPlayers;
	int numComputers;
	int numLaps;
	
	file >> flush >> flush;
	
	/*if ( flush == "TIME_LIMIT" ){
		file >> timeLimit;
		file >> flush;
	}*/

	if( flush != "NUM_PLAYERS" ) return;
	file >> numPlayers;

	file >> flush;
	if( flush != "NUM_COMPUTERS" ) return;
	file >> numComputers;

	file >> flush;
	if( flush != "NUM_LAPS" ) return;
	file >> numLaps;

	//m_Game->
	processPlayerVehicles( file, numPlayers );
	processComputerVehicles( file, numComputers );
	m_Objs.entityManager->getTerrain(0)->setNumberOfLaps( numLaps );
}


//--------------------------------------------------------------------------------------
// Function:  processPlayerVehicles
// Creates and adds vehicles to game based on information loaded from the specified file.
//--------------------------------------------------------------------------------------
void SceneLoader::processPlayerVehicles( ifstream& file, int numPlayers )
{
	string flush, effect, mesh;
	Vec3 pos;

	file >> flush;
	if( flush != "**PLAYER_VEHICLES" ) return;
	file >> flush;

	Vehicle * pv;
	for( int i=0; i < numPlayers; i++ )
	{
		//load effect filename and position from file
		file >> flush >> effect >> flush;
		file >> mesh >> flush;
		file >> pos.x >> pos.y >> pos.z;

		//create vehicle in entity manager
		pv = m_Objs.entityManager->makePlayer( m_Device, pos, toLPCWSTR(mesh).c_str(), toLPCWSTR(effect).c_str() ); 
	
		pv->setPlayerNum( i );

		//create a camera to go along with this player
		m_Objs.gameCameras.push_back( new GameCamera() );
		m_Objs.gameCameras[i]->updateWindow( m_BackSurface );
		m_Objs.gameCameras[i]->setTarget( pv );

		//create a controller to go along with player
		m_Objs.controllers.push_back( new XBox360Controller(i) ); 

		//add to simulator
		m_Objs.simulator->createVehicle( pv );
	}
}


//--------------------------------------------------------------------------------------
// Function:  processComputerVehicles
// Creates computer vehicles from file information and adds them to game.
//--------------------------------------------------------------------------------------
void SceneLoader::processComputerVehicles( ifstream& file, int numComputers )
{
	string flush, effect, mesh;
	Vec3 pos;

	file >> flush;
	if( flush != "**COMPUTER_VEHICLES" ) return;
	file >> flush;

	Vehicle * av;
	for( int i=0; i < numComputers; i++ )
	{
		//load effect filename and position from file
		file >> flush >> effect >> flush;
		file >> mesh >> flush;
		file >> pos.x >> pos.y >> pos.z;

		//create in Entity Manager
		av = m_Objs.entityManager->makeComputer( m_Device, pos, toLPCWSTR(mesh).c_str(), toLPCWSTR(effect).c_str() ); 
		
		av->setPlayerNum( -1 - i );

		//add to simulator
		m_Objs.simulator->createVehicle( av );
	}
	m_Objs.simulator->setContacts();
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
	vector<Terrain*> terrains = m_Objs.entityManager->getTerrain();
	if( terrains.size() <= 0 ) return;
	terrains[0] = this->m_InnerTerrain;

	m_Objs.simulator->InitNx( terrains );
	m_Objs.simulator->addRamps( m_Ramps );

	if( m_InnerTerrain )
		delete m_InnerTerrain;
	m_InnerTerrain = NULL;
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

