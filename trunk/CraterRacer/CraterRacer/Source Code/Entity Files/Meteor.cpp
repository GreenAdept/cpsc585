/*
* The Meteor entity.
*/

#include "Meteor.h"

Meteor::Meteor() {
	mind = new TriggerCylinder (5);
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
