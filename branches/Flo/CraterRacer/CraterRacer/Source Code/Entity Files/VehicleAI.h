#include "Input.h"
#include "AI.h"
#include "Constants.h"

//-----------------------------------------
// Class: VehicleAI
// Implementation of a computer-controlled
// vehicle's AI.
//-----------------------------------------

class VehicleAI : public AI {
public:
	VehicleAI();
	void think();

	void setInput (Input::Arrow dir, bool isKeyDown);
	void setDir(Vec3 vec);
	bool* getInput();
	Vec3 getDir();
	void resetInput();
	void setPosition (Vec3 position);

private:
	Input input;
	Vec3 position;
};