#include "AIPath.h"
#include <cmath>


void AIPath::addWaypoint (Vec3 p) {
	wps.push_back (Waypoint (p));
}
void AIPath::addWaypoint (float x, float y, float z) {
	wps.push_back (Waypoint (x, y, z));
}
void AIPath::addPositionToLastWP (Vec3 p) {
	wps[wps.size()-1].addPosition (p);
}
void AIPath::addPositionToLastWP (float x, float y, float z) {
	wps[wps.size()-1].addPosition (Vec3 (x, y, z));
}


Vec3 Waypoint::getClosestPosition (Vec3 p) {
	float minDist = distSquared (positions[0], p);
	int index = 0;

	for (int i=1; i<positions.size(); i++) {
		float d = distSquared (positions[i], p);
		if (d < minDist) {
			minDist = d;
			index = i;
		}
	}

	return positions[index];
}

Vec3 Waypoint::getDirectionToWP (Vec3 p) {
	Vec3 temp = getClosestPosition(p) - p;
	temp.y = 0.0f;
	if (temp.x==0.0f && temp.z==0.0f)
		return temp;

	D3DXVec3Normalize (&temp, &temp);
	return temp;
}

float Waypoint::getDistanceSquaredToWP (Vec3 p) {
	float minDist = distSquared (positions[0], p);

	for (int i=1; i<positions.size(); i++) {
		float d = distSquared (positions[i], p);
		if (d < minDist)
			minDist = d;
	}

	return minDist;
}
float Waypoint::getDistanceToWP (Vec3 p) {
	return sqrt (getDistanceSquaredToWP (p));
}




Vec3 AIPath::getPositionOfWP (Vec3 p, int waypntIndex) {
	return wps[waypntIndex % wps.size()].getClosestPosition (p);
}

Vec3 AIPath::getDirectionToWP (Vec3 p, int waypntIndex) {
	return wps[waypntIndex % wps.size()].getDirectionToWP (p);
}

float AIPath::getDistanceSquaredToWP (Vec3 p, int waypntIndex) {
	return wps[waypntIndex % wps.size()].getDistanceSquaredToWP (p);
}

bool AIPath::reachedWaypoint (Vec3 p, int waypntIndex, float waypntRadius) {
	float d = wps[waypntIndex % wps.size()].getDistanceSquaredToWP (p);
	return d <= waypntRadius * waypntRadius;
}

int AIPath::findCurrentLap (int passedWPs) {
	return (passedWPs / wps.size()) + 1;
}
