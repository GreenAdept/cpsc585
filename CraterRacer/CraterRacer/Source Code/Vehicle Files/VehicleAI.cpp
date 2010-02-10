/*
 The Vehicle's AI class.
*/
#include "VehicleAI.h"
#include "EntityManager.h"


//--------------------------------------------------------------------------------------
// Function: think
// A stub for the thinking section of the AI.
//--------------------------------------------------------------------------------------
void VehicleAI::think (EntityManager *em, int myList, int myIndex) {
	Input* input = em->getComputerInputObj (myIndex);
	input->setInput (Input::UP, true);
}
