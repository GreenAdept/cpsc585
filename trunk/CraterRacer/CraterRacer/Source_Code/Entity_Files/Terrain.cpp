#include "Terrain.h"


Waypoint::~Waypoint () {
	for (int i=0; i<next.size(); i++)
		delete next[i];
	next.clear();
}


Vec3 Waypoint::getDirectionToWP (Vec3 p) {
	Vec3 temp = position - p;
	temp.y = 0.0f;
	if (temp.x==0.0f && temp.z==0.0f)
		return temp;

	D3DXVec3Normalize (&temp, &temp);
	return temp;
}


Waypoint* Waypoint::addNext (Vec3 p) {
	Waypoint* temp = new Waypoint (p);
	next.push_back (temp);
	return temp;
}
Waypoint* Waypoint::addNext (float x, float y, float z) {
	Waypoint* temp = new Waypoint (x, y, z);
	next.push_back (temp);
	return temp;
}
Waypoint* Waypoint::addNext (Waypoint* wp) {
	next.push_back (wp);
	return wp;
}

Waypoint* Waypoint::getRandomNext () {
	if (next.size() == 1)
		return next[0];
	if (next.size() == 0)
		return 0;

	//Should return random member of next - to be implemented
	return next[0];
}



void Terrain::buildTrack (Vec3* path, int size) 
{
	if( trackStart )
		delete trackStart;

	if (size == 0) return;

	trackStart = new Waypoint (path[0]);
	Waypoint* temp = trackStart;

	for (int i=1; i<size; i++)
		temp = temp->addNext (path[i]);
}
/*
void Terrain::buildTrack (Vec3* mainPath, Vec3* sidePath, int size) {
	delete trackStart;
	if (size == 0) return;

	trackStart = new Waypoint (mainPath[0]);
	Waypoint *main = trackStart, *side = 0;

	for (int i=1; i<size; i++) {
		if (sidePath[i] != NULL) {
			Waypoint* temp = main->addNext (mainPath[i]);
			if (side == 0)
				side = main->addNext (sidePath[i]);
			else
				side = side->addNext (sidePath[i]);
			main = temp;
		}
		else {
			Waypoint* temp = main->addNext (mainPath[i]);
			if (side != 0) {
				side->addNext (temp);
				side = 0;
			}
			main = temp;
		}
	}
}
*/