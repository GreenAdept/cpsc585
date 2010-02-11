/*
 * The format of the variables.txt file needs to be as follows:
 * Let #1 be the first line in the file, etc. The format of the
 * vectors should be x,y,z.
 * 
 * #1 m_vForceVec -> x,y,z
 * #2 m_rForceStrength -> double
 * #3 m_bForceMode -> bool
 * #4 m_vDefaultGravity -> x,y,z
 * #5 m_rRestitution -> double
 * #6 m_rStaticFriction -> double
 * #7 m_rDynamicFriction -> double
 * #8 m_rMaxAngularVelocity -> double
 * #9 m_rMaxWheelAngle -> double
 *
 * When running the game, hit 'L' to load the variables from
 * variables.txt.
 *
 */
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
	// public because it'll just be easier to get to
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