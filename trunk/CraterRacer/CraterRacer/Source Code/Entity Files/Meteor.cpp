/*
* The Meteor entity.
*/

#include "Meteor.h"

Meteor::Meteor() {
	mind = new TriggerCylinder (10.0f);
}

Meteor::~Meteor () {
	delete mind;
}


void Meteor::setInput (Input::Arrow dir, bool isKeyDown)
{

}

bool* Meteor::getInput ()
{
	bool input[] = { false, false, false, false };
	return input;
}

AI* Meteor::getAI () {
	if (mind->getState() == AI::TRIGGERED)
		mind->setState (AI::STOPPED);
	return mind;
}
