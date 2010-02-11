#include "VarLoader.h"

bool VarLoader::loadVars()
{
	file.open("variables.txt");
	if (!file.is_open()) return false;

	//read in the 9 variables from the file
	string strings[9];
	for (int i = 0; i < 9; i++)
	{
		if (!file.eof())
			getline(file, strings[i]);
		else
			return false; //there aren't 9 lines in the file
	}

	parse(strings);
	file.close();
	return true;
}

void VarLoader::parse(string strings[])
{
	forceVec = parseVec(strings[0]);
	forceStrength = atof(strings[1].c_str());
	
	if (strings[2].compare("true") == 0)
		forceMode = true;
	else
		forceMode = false;

	gravity = parseVec(strings[3]);
	restitution = atof(strings[4].c_str());
	sFriction = atof(strings[5].c_str());
	dFriction = atof(strings[6].c_str());
	maxAngularVelocity = atof(strings[7].c_str());
	maxWheelAngle = atof(strings[8].c_str());
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