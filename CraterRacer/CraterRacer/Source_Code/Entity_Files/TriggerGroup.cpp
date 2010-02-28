#include "TriggerGroup.h"

TriggerGroup::TriggerGroup(int id, int numMeteors) {
	this->id = id;
	meteorPositions = new Vec3[numMeteors];
}

void TriggerGroup::setTriggerVolume(Vec3 triggerVolume, int radius) {
	this->triggerVolume = triggerVolume;
	this->radius = radius;
}

void TriggerGroup::addMeteor(int index, Vec3 position) {
	meteorPositions[index] = position;
}