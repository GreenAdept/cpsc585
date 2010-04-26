#include "VarLoader.h"

bool VarLoader::loadVars( Simulator* sim )
{
	file.open("variables.txt", ios::in );
	if (!file.is_open()) return false;

	file.seekg (0, ios::beg);

	//read in the variables from the file
	string strings[NUM_VARIABLES];
	for (int i = 0; i < NUM_VARIABLES; i++)
	{
		if (!file.eof())
			getline(file, strings[i]);
		else
			return false; //there aren't the right number of lines in the file
	}

	parse(strings);
	file.close();

	sim->setForceVec(forceVec);
	sim->setForceStr(forceStrength);
	sim->setForceMode(forceMode);
	sim->setGravity(gravity);
	sim->setRestitution(restitution);
	sim->setSFriction(sFriction);
	sim->setDFriction(dFriction);
	sim->setMaxAngularVelocity(maxAngularVelocity);
	sim->setMaxWheelAngle(maxWheelAngle);
	sim->setSpringScale(springScale);
	sim->setDamperScale(damperScale);
	sim->setSteeringPower(steeringPower);
	sim->setBrakingFriction(brakingFriction);
	sim->setVehicleMass(vehicleMass);

	return true;
}


void VarLoader::loadVars( Renderer* r )
{
	file.open("ShadowVars.txt", ios::in );
	if (!file.is_open()) return;

	file.seekg (0, ios::beg);

	//read in the variables from the file
	string strings[NUM_SHADOW_VARIABLES];
	for (int i = 0; i < NUM_SHADOW_VARIABLES; i++)
	{
		if (!file.eof())
			getline(file, strings[i]);
		else
			return; //there aren't the right number of lines in the file
	}
	file.close();

	r->setLightParams( parseVec(strings[0]), parseVec(strings[1]), getDouble(strings[2]) );
}

void VarLoader::parse(string strings[])
{
	forceVec			= parseVec(strings[0]);
	forceStrength		= getDouble( strings[1] );
	forceMode			= getBool(strings[2]);
	gravity				= parseVec(strings[3]);
	restitution			= getDouble(strings[4]);
	sFriction			= getDouble(strings[5]);
	dFriction			= getDouble(strings[6]);
	maxAngularVelocity	= getDouble(strings[7]);
	maxWheelAngle		= getDouble(strings[8]);
	springScale			= getDouble(strings[9]);
	damperScale			= getDouble(strings[10]);
	steeringPower		= getDouble(strings[11]);
	brakingFriction		= getDouble(strings[12]);
	vehicleMass			= getDouble(strings[13]);
}

double VarLoader::getDouble( string s )
{
	stringstream ss (stringstream::in | stringstream::out);
	double temp;
	ss << s;
	ss >> temp;
	return temp;
}

bool VarLoader::getBool( string s )
{
	stringstream ss (stringstream::in | stringstream::out);
	bool temp;
	ss << s;
	ss >> temp;
	return temp;
}

Vec3 VarLoader::parseVec(string s)
{
	Vec3 vec;
	char str[256];
	strcpy(str, s.c_str());
	char * temp;
	
	// get x
	temp = strtok(str, " ,");
	vec.x = atof(temp);

	// get y
	temp = strtok(NULL, " ,");
	vec.y = atof(temp);

	temp = strtok(NULL, " ,");
	vec.z = atof(temp);
/*
	while (temp != NULL)
	{
		printf ("%s\n",temp);
		temp = strtok (NULL, " ,");
	}
*/
	return vec;
}