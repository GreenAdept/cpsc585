/*
The Prop entity.
*/

#include "Prop.h"

Prop::Prop() {
	//renderable.canBeRendered = false;
	//Entity();
	//cout << renderable.canBeRendered << endl;
}

void Prop::setInput(Input::Arrow dir, bool isKeyDown)
{

}

bool* Prop::getInput()
{
	bool input[] = { false, false, false, false };
	return input;
}