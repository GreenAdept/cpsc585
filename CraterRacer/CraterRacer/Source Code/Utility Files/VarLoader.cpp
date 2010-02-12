#include "VarLoader.h"

bool VarLoader::loadVars()
{
	file.open("variables.txt");
	if (!file.is_open()) return false;

	//read in the 11 variables from the file
	string strings[11];
	for (int i = 0; i < 11; i++)
	{
		if (!file.eof())
			getline(file, strings[i]);
		else
			return false; //there aren't 11 lines in the file
	}

	parse(strings);
	file.close();
	return true;
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
	//char *str = s.c_str();
	//char str[] ="- This, a sample string.";
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