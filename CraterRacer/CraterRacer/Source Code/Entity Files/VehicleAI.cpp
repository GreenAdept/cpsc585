#include "VehicleAI.h"

VehicleAI::VehicleAI()
{
}

void VehicleAI::setInput(Input::Arrow dir, bool isKeyDown)
{
	input.setInput(dir, isKeyDown);
}

bool* VehicleAI::getInput()
{
	return input.getInput();
}

void VehicleAI::resetInput()
{
	input.reset();
}

