#pragma once
#ifndef AIVEHICLE_H
#define AIVEHICLE_H

#include "Vehicle.h"

class AIVehicle: public Vehicle {
	public:
		AIVehicle(void);

		void update(void);
};
#endif