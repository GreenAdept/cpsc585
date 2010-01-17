#include "prop.h"

Prop::Prop() {
	renderable.canBeRendered = false;
	Entity();
	cout << renderable.canBeRendered << endl;
}