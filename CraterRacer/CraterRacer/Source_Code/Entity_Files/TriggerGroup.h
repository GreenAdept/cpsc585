#pragma once
#ifndef TRIGGERGROUP_H
#define TRIGGERGROUP_H

#include "constants.h"

class TriggerGroup {
	public:
		TriggerGroup(int id, int numMeteors);
		void setTriggerVolume(Vec3 triggerVolume, int radius);
		void addMeteor(int index, Vec3 position);

		int id;
		int numMeteors;
		Vec3 triggerVolume;
		int radius;
		Vec3 *meteorPositions;
};
#endif