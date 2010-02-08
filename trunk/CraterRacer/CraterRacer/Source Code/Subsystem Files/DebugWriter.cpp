#include "DebugWriter.h"

void DebugWriter::open() {}

void DebugWriter::writeToFile(string s) {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out | ios::app);
  myfile << s << endl;
  myfile.close();
}

void DebugWriter::writeToFile(double d) {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out | ios::app);
  myfile << d << endl;
  myfile.close();
}

void DebugWriter::writeToFile(float f) {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out | ios::app);
  myfile << f << endl;
  myfile.close();
}

void DebugWriter::writeToFile(NxVec3 vec) {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out | ios::app);
  myfile << vec.x << " " << vec.y << " " << vec.z << endl;
  myfile.close();
}

void DebugWriter::writeToFile(Vec3 vec) {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out | ios::app);
  myfile << vec.x << " " << vec.y << " " << vec.z << endl;
  myfile.close();
}

/*void DebugWriter::writeToFile(bool set) {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out | ios::app);
  if (set) {
	myfile << "true" << endl;
  }
  else {
		myfile << "false" << endl;
  }
  myfile.close();
}*/

void DebugWriter::clearFile() {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out);
  myfile << "";
  myfile.close();
}


void DebugWriter::close() {}