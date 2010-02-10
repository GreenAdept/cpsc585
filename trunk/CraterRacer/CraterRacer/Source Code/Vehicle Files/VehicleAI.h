#include "Input.h"
#include "AI.h"
#include "Constants.h"
#include "DebugWriter.h"

//-----------------------------------------
// Class: VehicleAI
// Implementation of a computer-controlled
// vehicle's AI.
//-----------------------------------------

class VehicleAI : public AI {
public:
	VehicleAI();
	void think (EntityManager *em, int myList, int myIndex);
	Input *getInputObj () { return &input; }

protected:
	Input input;
	DebugWriter writer;
};