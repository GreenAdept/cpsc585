#ifndef AI_PATH_H
#define AI_PATH_H

#include "AI.h"
#include "Constants.h"
#include <vector>


class Waypoint {
	std::vector<Vec3> positions;

public:
	Waypoint (Vec3 p) { positions.push_back (p); }
	Waypoint (float x, float y, float z) { positions.push_back (Vec3 (x, y, z)); }

	void      addPosition            (Vec3 p) { positions.push_back (p); }
	Vec3      getClosestPosition     (Vec3 p);
	float     getDistanceSquaredToWP (Vec3 p);
	float     getDistanceToWP        (Vec3 p);

	Vec3      getDirectionToWP_ClosestToGivenPos (Vec3 p);
	Vec3      getDirectionToWP_ClosestToGivenDir (Vec3 p, Vec3 d);
};


class AIPath {
	std::vector<Waypoint> wps;
	int laps;

public:
	AIPath () : laps (1) {}

	void  addWaypoint            (Vec3 p);
	void  addWaypoint            (float x, float y, float z);
	void  addPositionToLastWP    (Vec3 p);
	void  addPositionToLastWP    (float x, float y, float z);

	int   getNumberOfWPs         ()      { return wps.size(); }
	int   getNumberOfLaps        ()      { return laps; }
	void  setNumberOfLaps        (int l) { if (l > 0) laps = l; }

	Vec3  getPositionOfWP        (Vec3 p, int waypntIndex);
	Vec3  getDirectionToWP       (Vec3 p, int waypntIndex);
	float getDistanceSquaredToWP (Vec3 p, int waypntIndex);
	bool  reachedWaypoint        (Vec3 p, int waypntIndex, float waypntRadius);
	bool  goingWrongWay          (Vec3 p, Vec3 d, int waypntIndex);
	int   findCurrentLap         (int passedWPs);
};


#endif
