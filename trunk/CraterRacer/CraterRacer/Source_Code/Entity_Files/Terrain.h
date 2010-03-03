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
	Vec3 position;
	vector<Waypoint*> next;

public:
	Waypoint (Vec3 p) : position (p) {}
	Waypoint (float x, float y, float z) : position (x, y, z) {}
	~Waypoint ();

	Vec3&    getPosition () { return position; }
	Vec3     getDirectionToWP (Vec3 p);
	Waypoint *addNext (Vec3 p);
	Waypoint *addNext (float x, float y, float z);
	Waypoint *addNext (Waypoint* wp);
	Waypoint *getRandomNext ();
	Waypoint *getNext (int index) { return next[index]; }
};


//---------------------------------------------------------
// Class: Terrain
//---------------------------------------------------------

class Terrain: public Entity {
public:
	//Public Interface ------------------------------------
	Terrain () { trackStart = 0; laps = 1; }
	~Terrain () { delete trackStart; }

	int       getNumberOfLaps ()      { return laps; }
	void      setNumberOfLaps (int l) { if (l > 0) laps = l; }
	Waypoint* getTrackStart   ()      { return trackStart; }
	void      buildTrack      (Vec3* path, int size);		//builds a linear track
	//void      buildTrack (Vec3* mainPath, Vec3* sidePath, int size);	//builds a track with forks
	
private:
	//Data Members ----------------------------------------
	int       laps;
	Waypoint* trackStart;
};

#endif //TERRAIN_H