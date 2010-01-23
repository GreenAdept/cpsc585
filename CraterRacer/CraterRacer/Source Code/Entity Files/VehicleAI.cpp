#include "VehicleAI.h"

VehicleAI::VehicleAI()
{
}

void VehicleAI::setInput(Input::Arrow dir, bool isKeyDown)
{
	input.setInput(dir, isKeyDown);
}


