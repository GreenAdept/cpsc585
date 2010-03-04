/*
* The Meteor entity.
*/

#include "Meteor.h"


//------------------------------------------------------
// Constructor: MeteorAI
// Initializes the meteor AI.
//------------------------------------------------------
MeteorAI::MeteorAI () {
	state = AI::WAITING;
}

//------------------------------------------------------
// Destructor: MeteorAI
// Frees all the memory allocated to the meteor AI.
//------------------------------------------------------
MeteorAI::~MeteorAI () {}

//------------------------------------------------------
// Function: think
// Implementation of the AI's think method. The meteor
// AI detects if the player has enter its trigger
// volume and sets its state to MOVING.
//------------------------------------------------------
void MeteorAI::think (EntityManager *em, int myList, int myIndex) {}

//------------------------------------------------------
// Function: informOfCollision
// When the meteor AI is informed of a collision, it
// sets its state to STOPPED and frees the memory used
// by its trigger volume.
//------------------------------------------------------
void MeteorAI::informOfCollision () {
	state = AI::TRIGGERED;
}

//------------------------------------------------------
// Function: informOfTrigger
// Informs the meteor entity that it has been triggered.
//------------------------------------------------------
void MeteorAI::informOfTrigger () {
	state = AI::MOVING;
}




//------------------------------------------------------
// Constructor: Meteor
// Initializes the meteor entity.
//------------------------------------------------------
Meteor::Meteor(Vec3 target) {
	mind = new MeteorAI ();
	this->target = target;
}

//------------------------------------------------------
// Constructor: Meteor
// Initializes the meteor entity.
//------------------------------------------------------
Meteor::Meteor() {
	mind = new MeteorAI ();
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

//------------------------------------------------------
// Function: informOfTrigger
// Informs the meteor entity that it has been triggered.
//------------------------------------------------------
void Meteor::informOfTrigger () {
	mind->informOfTrigger();
}