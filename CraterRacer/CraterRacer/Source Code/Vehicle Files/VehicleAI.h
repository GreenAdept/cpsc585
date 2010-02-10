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

	void setInput (Input::Arrow dir, bool isKeyDown);
	bool* getInput();
	bool* getButtons();
	void resetInput();
	void setPosition (Vec3 position);
	void setController(bool controller);
	bool getController();
	void setThumbstick(float x);
	float getThumbstick();

	void setDir(float x, Input::Button button);
	void setDir(float x);
	void setDir(Input::Arrow, bool isKeyDown);

protected:
	Input input;
	Vec3 position;
	DebugWriter writer;
};