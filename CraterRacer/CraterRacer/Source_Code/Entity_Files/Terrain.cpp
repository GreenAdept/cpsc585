#include "Terrain.h"


Waypoint::~Waypoint () {
	delete next;
}


Vec3 Waypoint::getClosestPosition (Vec3 p) {
	float minDist = distSquared (position[0], p);
	int index = 0;

	for (int i=1; i<position.size(); i++) {
		float d = distSquared (position[i], p);
		if (d < minDist) {
			minDist = d;
			index = i;
		}
	}

	return position[index];
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
	float minDist = distSquared (position[0], p);

	for (int i=1; i<position.size(); i++) {
		float d = distSquared (position[i], p);
		if (d < minDist)
			minDist = d;
	}

	return minDist;
}

Waypoint* Waypoint::setNext (Vec3 p) {
	next = new Waypoint (p);
	return next;
}
Waypoint* Waypoint::setNext (Waypoint* wp) {
	next = wp;
	return next;
}
