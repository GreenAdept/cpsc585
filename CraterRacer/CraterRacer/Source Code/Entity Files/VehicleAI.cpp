/*
 The Vehicle's AI class.
*/
#include "VehicleAI.h"

//--------------------------------------------------------------------------------------
// Constructor:
// The constructor for VehicleAI.
//--------------------------------------------------------------------------------------
VehicleAI::VehicleAI() {

}

//--------------------------------------------------------------------------------------
// Function: setInput
// Delegates set input to input system.
//--------------------------------------------------------------------------------------
void VehicleAI::setInput(Input::Arrow dir, bool isKeyDown) {
	input.setInput(dir, isKeyDown);
}

void VehicleAI::setDir(Vec3 vec)
{
	input.setDir(vec);
}

//--------------------------------------------------------------------------------------
// Function: getInput
// Delegates get input to input system.
//--------------------------------------------------------------------------------------
bool* VehicleAI::getInput() {
	return input.getInput();
}

Vec3 VehicleAI::getDir()
{
	return input.getDir();
}

//--------------------------------------------------------------------------------------
// Function: resetInput
// Delegates reset input to input system.
//--------------------------------------------------------------------------------------
void VehicleAI::resetInput() {
	input.reset();
}

//--------------------------------------------------------------------------------------
// Function: setPosition
// Informs the AI of the vehicle's current position.
//--------------------------------------------------------------------------------------
void VehicleAI::setPosition (Vec3 pos) {
	position = pos;
}

//--------------------------------------------------------------------------------------
// Function: think
// A stub for the thinking section of the AI.
//--------------------------------------------------------------------------------------
void VehicleAI::think() {

}

