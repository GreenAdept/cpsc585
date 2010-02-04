#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H


#include <vector>
#include "PlayerVehicle.h"
#include "AIVehicle.h"
#include "Meteor.h"
#include "Crater.h"
#include "Prop.h"


enum lists {PLAYERS, COMPUTERS, METEORS, CRATERS, PROPS};
#define NUM_LISTS 5

class EntityManager {
	std::vector<Entity*> entities [NUM_LISTS];

public:
	EntityManager () {}
	~EntityManager ();

	std::vector<Vehicle*> getVehicles ();
	std::vector<Renderable*> getRenderables ();
	std::vector<AI*> getAIs ();
	std::vector<BoundingBox> getBoundingBoxes ();
	
	Vec3 getPosition (int list, int index);

	void addEntity (int list, Entity* e) { entities[list].push_back(e); }

	std::vector<Entity*> & operator[] (int i) { return entities[i]; }
};


#endif