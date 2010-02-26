#include "EntityManager.h"
using namespace std;


//------------------------------------------------------
// Destructor: EntityManager
// Frees all the memory allocated to the entities being
// tracked by the manager.
//------------------------------------------------------

EntityManager::~EntityManager () {
	for (int i=0; i<NUM_LISTS; i++) {
		int size = entities[i].size();
		for (int j=0; j<size; j++)
			delete entities[i][j];
	}
}



//------------------------------------------------------
// Function: getEntities
// Returns an std::vector containing pointers to all
// the entities being tracked by the manager.
//------------------------------------------------------

vector<Entity*> EntityManager::getEntities () {
	int index = 0;
	vector<Entity*> result (getSize());

	for (int i=0; i<NUM_LISTS; i++) {
		int s = entities[i].size();
		for (int j=0; j<s; j++)
			result[index++] = entities[i][j];
	}

	return result;
}

//------------------------------------------------------
// Function: getVehicles
// Returns an std::vector containing pointers to all
// the vehicle entities (player-controlled and computer-
// controlled) being tracked by the manager.
//------------------------------------------------------

vector<Vehicle*> EntityManager::getVehicles () {
	int index = 0;
	int playerSize = entities[PLAYERS].size();
	int compSize = entities[COMPUTERS].size();

	vector<Vehicle*> result (playerSize + compSize);
	for (int i=0; i<playerSize; i++)
		result[index++] = (Vehicle*) entities[PLAYERS][i];
	for (int i=0; i<compSize; i++)
		result[index++] = (Vehicle*) entities[COMPUTERS][i];
	return result;
}

//------------------------------------------------------
// Function: getMeteors
// Returns an std::vector containing pointers to all
// the meteor entities being tracked by the manager.
//------------------------------------------------------

vector<Meteor*> EntityManager::getMeteors () {
	int size = entities[METEORS].size();
	vector<Meteor*> result (size);
	for (int i=0; i<size; i++)
		result[i] = (Meteor*) entities[METEORS][i];
	return result;
}

//------------------------------------------------------
// Function: getRenderables
// Returns an std::vector containing pointers to all
// the Renderable components of the entities being
// tracked by the manager.
//------------------------------------------------------

vector< Renderable* > EntityManager::getRenderables( ) {
	int index = 0;
	int numWheels = entities[PLAYERS].size() * 4 + entities[COMPUTERS].size() * 4;
	vector<Renderable*> result (getSize() + numWheels);

	//Get vehicle renderables first because they have wheel renderables inside them
	for (int i=PLAYERS; i <= COMPUTERS; i++) {
		for (int v=0; v < entities[i].size(); v++) {
			Vehicle* tempVehicle = (Vehicle*) entities[i][v];

			result[index++] = tempVehicle->getRenderable();
			
			//Add the wheel renderables
			for (int w = WHEEL0; w <= WHEEL3; w++)
				result[index++] = tempVehicle->m_Wheels[w].getRenderable();
		}
	}

	//Now add all the rest of the renderables
	for (int i = METEORS; i < NUM_LISTS; i++) {
		int s = entities[i].size();
		for( int j=0; j<s; j++ )
			result[index++] = entities[i][j]->getRenderable();
	}

	return result;
}

//------------------------------------------------------
// Function: getAIs
// Returns an std::vector containing pointers to all
// the AI components of the entities being tracked by
// the manager.
//------------------------------------------------------

vector<AI*> EntityManager::getAIs (int list) {
	int size = entities[list].size();
	vector<AI*> result (size);

	for (int i=0; i<size; i++)
		result[i] = entities[list][i]->getAI();

	return result;
}

//------------------------------------------------------
// Function: getTerrain
// Returns a pointer to the terrain entity.
//------------------------------------------------------

Terrain* EntityManager::getTerrain () {
	if (entities[TERRAIN].empty())
		return 0;
	else
		return (Terrain*) entities[TERRAIN][0];
}



//------------------------------------------------------
// Function: getPosition
// Returns the position of the entity in the specified
// list at the specified index.
//------------------------------------------------------

Vec3 EntityManager::getPosition (int list, int index) {
	return entities[list][index]->getPosition();
}

//------------------------------------------------------
// Function: getPlayerInputObj
// Returns a pointer to the Input object of the
// PlayerVehicle with the specified index.
//------------------------------------------------------

Input* EntityManager::getPlayerInputObj (int index) {
	Vehicle* v = (Vehicle*) entities[PLAYERS][index];
	return v->getInputObj ();
}

//------------------------------------------------------
// Function: getComputerInputObj
// Returns a pointer to the Input object of the
// AIVehicle with the specified index.
//------------------------------------------------------

Input* EntityManager::getComputerInputObj (int index) {
	Vehicle* v = (Vehicle*) entities[COMPUTERS][index];
	return v->getInputObj ();
}


//------------------------------------------------------
// Function: clear
// Frees all the memory allocated to the entities being
// tracked by the manager and clears the entity lists
// of pointers.
//------------------------------------------------------

void EntityManager::clear () {
	for (int i=0; i<NUM_LISTS; i++) {
		int size = entities[i].size();
		for (int j=0; j<size; j++)
			delete entities[i][j];
		entities[i].clear();
	}
}

//------------------------------------------------------
// Function: getSize
// Returns the number of entities being tracked by
// the manager.
//------------------------------------------------------

int EntityManager::getSize () {
	int result = 0;

	for (int i=0; i<NUM_LISTS; i++)
		result += entities[i].size();

	return result;
}



//------------------------------------------------------
// make Functions
// These functions create an entity of the desired type,
// add it to the entity manager, and return a pointer
// to it.
//------------------------------------------------------

PlayerVehicle* EntityManager::makePlayer (Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename) {
	PlayerVehicle* pv = new PlayerVehicle();
	pv->initialize (device, pos, filename, effectFilename);
	entities[PLAYERS].push_back (pv);
	return pv;
}
AIVehicle* EntityManager::makeComputer (Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename) {
	AIVehicle* av = new AIVehicle();
	av->initialize (device, pos, filename, effectFilename);
	entities[COMPUTERS].push_back (av);
	return av;
}
Meteor* EntityManager::makeMeteor (Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename) {
	Meteor* m = new Meteor();
	m->initialize (device, pos, filename, effectFilename);
	entities[METEORS].push_back (m);
	return m;
}
Crater* EntityManager::makeCrater (Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename) {
	Crater* c = new Crater();
	c->initialize (device, pos, filename, effectFilename);
	entities[CRATERS].push_back (c);
	return c;
}
Prop* EntityManager::makeProp (Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename) {
	Prop* p = new Prop();
	p->initialize (device, pos, filename, effectFilename);
	entities[PROPS].push_back (p);
	return p;
}
Terrain* EntityManager::makeTerrain (Device* device, Vec3 pos, LPCWSTR filename, LPCWSTR effectFilename ) {
	Terrain* t = new Terrain();
	t->initialize (device, pos, filename, effectFilename );
	entities[TERRAIN].push_back( t );
	return t;
}
