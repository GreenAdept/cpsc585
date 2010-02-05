#pragma once
#ifndef DEBUGWRITER_H
#define DEBUGWRITER_H

#include <iostream>
#include <fstream>
#include <string>

//#include "NxPhysics.h"
#include "Constants.h"

using namespace std;

class DebugWriter {
public:
	void open();
	void writeToFile(string s);
	void writeToFile(double i);
	//void writeToFile(NxVec3 vec);
	void writeToFile(Vec3 vec);
	void clearFile();
	void close();
};

#endif