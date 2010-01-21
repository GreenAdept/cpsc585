#pragma once
#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H

#include "Vehicle.h"

class PlayerVehicle: public Vehicle {
	public:
		PlayerVehicle(void);

		void update(void);
};
#endif