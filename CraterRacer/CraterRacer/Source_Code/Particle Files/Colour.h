#pragma once
#ifndef COLOUR_H
#define COLOUR_H

class Colour {
public:
	Colour();
	Colour(int r, int g, int b, float transparency);

	Colour operator*(const float a);
	Colour operator+(const Colour a);

	int r, g, b;
	float transparency;
};

#endif COLOUR_H