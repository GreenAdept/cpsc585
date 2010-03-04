#pragma once
#ifndef DEBUGWRITER_H
#define DEBUGWRITER_H

#include <iostream>
#include <fstream>
#include <string>

//#define NOMINMAX
//#include "NxPhysics.h"
#include "Constants.h"

using namespace std;

class DebugWriter {
public:
	void open();
	void writeToFile(string s);
	void writeToFile(double d);
	void writeToFile(float f);
	void writeToFile(int i);
	//void writeToFile(NxVec3 vec);
	void writeToFile(Vec3 vec);
	//void writeToFile(bool set);
	void clearFile();
	void close();
};

#endif