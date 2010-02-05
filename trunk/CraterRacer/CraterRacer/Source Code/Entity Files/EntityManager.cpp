#include "EntityManager.h"
using namespace std;

/*
struct EntityNode {
	Entity* data;
	EntityNode* next;

	EntityNode ()          : data(0), next(0) {}
	EntityNode (Entity* e) : data(e), next(0) {}
	~EntityNode () {
		delete data;
		delete next;
	}

	void add (Entity* e) {
		EntityNode *node = this;
		while ((int)node->next != 0)
			node = node->next;
		node->next = new EntityNode (e);
	}

	Entity* get (int index) {
		EntityNode *node = this;
		while (index > 0)
			node = node->next;
		return node->data;
	}
};
*/




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
// Function: getRenderables
// Returns an std::vector containing pointers to all
// the Renderable components of the entities being
// tracked by the manager.
//------------------------------------------------------

vector<Renderable*> EntityManager::getRenderables () {
	int index = 0;
	vector<Renderable*> result (getSize());

	for (int i=0; i<NUM_LISTS; i++) {
		int s = entities[i].size();
		for (int j=0; j<s; j++)
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
// Function: getPosition
// Returns the position of the entity in the specified
// list at the specified index.
//------------------------------------------------------

Vec3 EntityManager::getPosition (int list, int index) {
	return entities[list][index]->getPosition();
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



PlayerVehicle* EntityManager::makePlayer (Device* device, Vec3 pos, LPCWSTR filename) {
	PlayerVehicle* pv = new PlayerVehicle();
	pv->initialize (device, pos, filename);
	entities[PLAYERS].push_back (pv);
	return pv;
}
/*
AIVehicle* EntityManager::makeComputer (Device* device, Vec3 pos, LPCWSTR filename) {
	AIVehicle* av = new AIVehicle();
	av->initialize (device, pos, filename);
	entities[COMPUTERS].push_back (av);
	return av;
}
*/
Meteor* EntityManager::makeMeteor (Device* device, Vec3 pos, LPCWSTR filename) {
	Meteor* m = new Meteor();
	m->initialize (device, pos, filename);
	entities[METEORS].push_back (m);
	return m;
}
Crater* EntityManager::makeCrater (Device* device, Vec3 pos, LPCWSTR filename) {
	Crater* c = new Crater();
	c->initialize (device, pos, filename);
	entities[CRATERS].push_back (c);
	return c;
}
Prop* EntityManager::makeProp (Device* device, Vec3 pos, LPCWSTR filename) {
	Prop* p = new Prop();
	p->initialize (device, pos, filename);
	entities[PROPS].push_back (p);
	return p;
}
