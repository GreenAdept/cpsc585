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
 * #10 m_rSpringScale -> double
 * #11 m_rDamperScale -> double
 * #12 m_rSteeringPower -> double
 *
 * When running the game, hit 'L' to load the variables from
 * variables.txt.
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include "Constants.h"
#include "Simulator.h"
#include "Renderer.h"

using namespace std;

const int NUM_VARIABLES = 14;
const int NUM_SHADOW_VARIABLES = 2;

class VarLoader {
public:
	bool loadVars( Simulator* sim );
	void loadVars( Renderer* r );

	void parse(string strings[]);
	Vec3 parseVec(string s);
	double getDouble( string s );
	bool getBool( string s );

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
	double springScale;
	double damperScale;
	double steeringPower;
	double brakingFriction;
	double vehicleMass;

	
private:
	ifstream file;

};