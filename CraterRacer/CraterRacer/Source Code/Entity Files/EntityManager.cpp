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




EntityManager::~EntityManager () {
	for (int i=0; i<NUM_LISTS; i++) {
		int size = entities[i].size();
		for (int j=0; j<size; j++)
			delete entities[i][j];
	}
}


vector<Vehicle*> EntityManager::getVehicles () {
	int playerSize = entities[PLAYERS].size();
	int compSize = entities[COMPUTERS].size();

	vector<Vehicle*> result;
	for (int i=0; i<playerSize; i++)
		result.push_back ((Vehicle*) entities[PLAYERS][i]);
	for (int i=0; i<compSize; i++)
		result.push_back ((Vehicle*) entities[COMPUTERS][i]);
	return result;
}

vector<Renderable*> EntityManager::getRenderables () {
	vector<Renderable*> result;

	for (int i=0; i<NUM_LISTS; i++) {
		int size = entities[i].size();
		for (int j=0; j<size; j++)
			result.push_back (entities[i][j]->getRenderable());
	}

	return result;
}

vector<AI*> EntityManager::getAIs () {
	vector<AI*> result;

	for (int i=0; i<NUM_LISTS; i++) {
		int size = entities[i].size();
		for (int j=0; j<size; j++)
			result.push_back (entities[i][j]->getAI());
	}

	return result;
}

vector<BoundingBox> EntityManager::getBoundingBoxes () {
	vector<BoundingBox> result;

	for (int i=0; i<NUM_LISTS; i++) {
		int size = entities[i].size();
		for (int j=0; j<size; j++)
			result.push_back (entities[i][j]->getBoundingBox());
	}

	return result;
}

Vec3 EntityManager::getPosition (int list, int index) {
	return entities[list][index]->getPosition();
}

