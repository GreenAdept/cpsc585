#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Entity.h"
#include <vector>


//---------------------------------------------------------
// Class: Waypoint
// Waypoint data structure to be used for AI pathing.
//---------------------------------------------------------

struct Waypoint {
	Vec3 position;
	Waypoint* next;

	Waypoint (Vec3 p) { position = p; }
	~Waypoint () { delete next; }
};


//---------------------------------------------------------
// Class: Terrain
//---------------------------------------------------------

class Terrain: public Entity {
public:
	//Public Interface ------------------------------------
	Terrain () {}
	~Terrain () { delete trackStart; }

	Waypoint* getTrackStart () { return trackStart; }
	void      buildTrack (Vec3* waypoints, int size);
	void      buildTrack (std::vector<Vec3> waypoints);
	
private:
	//Data Members ----------------------------------------
	Waypoint* trackStart;
};

#endif //TERRAIN_H