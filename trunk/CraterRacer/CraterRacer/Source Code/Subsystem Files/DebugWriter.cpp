#include "DebugWriter.h"

void DebugWriter::open() {}

void DebugWriter::writeToFile(string s) {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out | ios::app);
  myfile << s << endl;
  myfile.close();
}

void DebugWriter::writeToFile(int s) {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out | ios::app);
  myfile << s << endl;
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

void DebugWriter::clearFile() {
  ofstream myfile;
  myfile.open ("debug.txt", ios::out);
  myfile << "";
  myfile.close();
}

void DebugWriter::close() {}