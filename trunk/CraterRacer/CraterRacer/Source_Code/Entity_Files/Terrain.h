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

class Waypoint {
	std::vector<Vec3> position;
	Waypoint* next;

public:
	Waypoint (Vec3 p) : next (0) { position.push_back (p); }
	Waypoint (float x, float y, float z) : next (0) { position.push_back (Vec3 (x, y, z)); }
	~Waypoint ();

	void      addPosition (Vec3 p) { position.push_back (p); }
	Vec3      getClosestPosition (Vec3 p);
	Vec3      getDirectionToWP (Vec3 p);
	float     getDistanceSquaredToWP (Vec3 p);
	Waypoint* setNext (Vec3 p);
	Waypoint* setNext (Waypoint* wp);
	Waypoint* getNext () { return next; }
};


//---------------------------------------------------------
// Class: Terrain
//---------------------------------------------------------

class Terrain: public Entity {
public:
	//Public Interface ------------------------------------
	Terrain () { trackStart = 0; laps = 1; }
	~Terrain () { delete trackStart; }

	int       getNumberOfLaps ()             { return laps; }
	void      setNumberOfLaps (int l)        { if (l > 0) laps = l; }
	Waypoint* getTrackStart   ()             { return trackStart; }
	void      setTrackStart   (Waypoint* ts) { trackStart = ts; }
	
private:
	//Data Members ----------------------------------------
	int       laps;
	Waypoint* trackStart;
};

#endif //TERRAIN_H