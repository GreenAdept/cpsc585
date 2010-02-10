#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H


#include <vector>
#include "PlayerVehicle.h"
#include "AIVehicle.h"
#include "Meteor.h"
#include "Crater.h"
#include "Prop.h"
#include "Terrain.h"


//------------------------------------------------------
// The list enums provide indices into the EntityManager's
// array of entity lists
//------------------------------------------------------
enum lists {PLAYERS, COMPUTERS, METEORS, CRATERS, PROPS, TERRAIN};


//------------------------------------------------------
// NUM_LISTS defines the number of seperate entity lists to
// be maintained by the EntityManager
//------------------------------------------------------
#define NUM_LISTS 6


//------------------------------------------------------
// Class: EntityManager
// The EntityManager class keeps track of pointers to
// all the entities. It provides methods for creating
// new entities, getting pointers to those entities and
// their components, and can be used as a data broker.
//------------------------------------------------------

class EntityManager {
	std::vector<Entity*> entities [NUM_LISTS];

public:
	EntityManager () {}
	~EntityManager ();

	std::vector<Entity*>     getEntities ();
	std::vector<Vehicle*>    getVehicles ();
	std::vector<Renderable*> getRenderables ();
	std::vector<AI*>         getAIs (int list);
	Input*                   getPlayerInputObj (int index);
	Input*                   getComputerInputObj (int index);
	Terrain*                 getTerrain ();
	
	Vec3 getPosition (int list, int index);

	void addEntity (int list, Entity* e) { entities[list].push_back(e); }
	PlayerVehicle* makePlayer   (Device* device, Vec3 pos, LPCWSTR filename);
	AIVehicle*     makeComputer (Device* device, Vec3 pos, LPCWSTR filename);
	Meteor*        makeMeteor   (Device* device, Vec3 pos, LPCWSTR filename);
	Crater*        makeCrater   (Device* device, Vec3 pos, LPCWSTR filename);
	Prop*          makeProp     (Device* device, Vec3 pos, LPCWSTR filename);
	Terrain*       makeTerrain  (Device* device, Vec3 pos, LPCWSTR filename);

	std::vector<Entity*> & operator[] (int i) { return entities[i]; }
	void clear ();
	int getSize ();
};


#endif