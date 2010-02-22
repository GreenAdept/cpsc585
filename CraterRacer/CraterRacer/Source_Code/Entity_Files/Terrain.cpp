#include "Terrain.h"


void Terrain::buildTrack (Vec3* waypoints, int size) {
	if (size == 0)
		return;

	delete trackStart;
	trackStart = new Waypoint (waypoints[0]);
	Waypoint* temp = trackStart;

	for (int i=1; i<size; i++) {
		temp->next = new Waypoint (waypoints[i]);
		temp = temp->next;
	}
}

void Terrain::buildTrack (std::vector<Vec3> waypoints) {
	if (waypoints.size() == 0)
		return;

	delete trackStart;
	trackStart = new Waypoint (waypoints[0]);
	Waypoint* temp = trackStart;

	for (int i=1; i<waypoints.size(); i++) {
		temp->next = new Waypoint (waypoints[i]);
		temp = temp->next;
	}
}