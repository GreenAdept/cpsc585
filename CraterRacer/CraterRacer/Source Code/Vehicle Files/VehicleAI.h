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
	void think (EntityManager *em, int myList, int myIndex);

	void setInput (Input::Arrow dir, bool isKeyDown);
	void setDir(Vec3 vec);
	bool* getInput();
	Vec3 getDir();
	void resetInput();
	void setPosition (Vec3 position);
	void setController(bool controller);
	bool getController();
	void setThumbstick(float x);
	float getThumbstick();

	void drive(Vec3 dir, bool accelerate, bool decelerate, Vec3 velocity);
	void drive(Input::Arrow dir, bool isKeyDown);

protected:
	Input input;
	Vec3 position;
};