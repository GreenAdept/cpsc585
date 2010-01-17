#pragma once
#ifndef PLAYERVEHICLE_H
#define PLAYERVEHICLE_H

#include "vehicle.h"

class PlayerVehicle: public Vehicle {
	public:
		PlayerVehicle(void);

		void update(void);
};
#endif