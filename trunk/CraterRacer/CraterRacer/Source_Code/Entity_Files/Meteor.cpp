/*
* The Meteor entity.
*/

#include "Meteor.h"


//------------------------------------------------------
// Constructor: MeteorAI
// Initializes the meteor AI.
//------------------------------------------------------
MeteorAI::MeteorAI (float radius) {
	trigger = new TriggerCylinder (radius);
}

//------------------------------------------------------
// Destructor: MeteorAI
// Frees all the memory allocated to the meteor AI.
//------------------------------------------------------
MeteorAI::~MeteorAI () {
	delete trigger;
}

//------------------------------------------------------
// Function: think
// Implementation of the AI's think method. The meteor
// AI detects if the player has enter its trigger
// volume and sets its state to MOVING.
//------------------------------------------------------
void MeteorAI::think (EntityManager *em, int myList, int myIndex) {
	if (state==AI::STOPPED || state==AI::MOVING)
		return;

	trigger->think (em, myList, myIndex);
	if (trigger->getState() == AI::TRIGGERED)
		state = AI::MOVING;
}

//------------------------------------------------------
// Function: informOfCollision
// When the meteor AI is informed of a collision, it
// sets its state to STOPPED and frees the memory used
// by its trigger volume.
//------------------------------------------------------
void MeteorAI::informOfCollision () {
	state = AI::STOPPED;
	delete trigger;
	trigger = 0;
}




//------------------------------------------------------
// Constructor: Meteor
// Initializes the meteor entity.
//------------------------------------------------------
Meteor::Meteor() {
	mind = new MeteorAI (400.0f);
}

//------------------------------------------------------
// Destructor: Meteor
// Frees all the memory allocated to the meteor entity.
//------------------------------------------------------
Meteor::~Meteor () {
	delete mind;
}

//------------------------------------------------------
// Function: getAI
// Returns a pointer to this meteor's AI object.
//------------------------------------------------------
AI* Meteor::getAI () {
	return mind;
}

//------------------------------------------------------
// Function: informOfCollision
// Informs the meteor entity that it has collided with
// the ground.
//------------------------------------------------------
void Meteor::informOfCollision () {
	mind->informOfCollision();
}
