#include "Colour.h"

Colour::Colour() {}

Colour::Colour(int r, int g, int b, float transparency) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->transparency = transparency;
}

Colour Colour::operator*(const float a) {
	return Colour(r*a, g*a, b*a, transparency*a);
}

Colour Colour::operator+(const Colour a) {
	return Colour(r+a.r, g+a.g, b+a.b, transparency+a.transparency);
}