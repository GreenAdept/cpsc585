#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "Constants.h"

using namespace std;

class VarLoader {
public:
	bool loadVars();

	void parse(string strings[]);
	Vec3 parseVec(string s);

	// constants to read in
	Vec3 forceVec;
	double forceStrength;
	bool forceMode;
	Vec3 gravity;
	double restitution;
	double sFriction;
	double dFriction;
	double maxAngularVelocity;
	double maxWheelAngle;
	
private:
	ifstream file;

};