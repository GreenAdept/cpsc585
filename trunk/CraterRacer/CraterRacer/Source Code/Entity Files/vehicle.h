#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include "entity.h"

using namespace std;

class Vehicle: public Entity {
	public:
		Vehicle(void);

		virtual void update(void);
};
#endif