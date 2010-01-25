/*
 The VehicleAI class.
*/
#include "Input.h"
#include "AI.h"

class VehicleAI : public AI
{
public:
	VehicleAI();
	void setInput(Input::Arrow dir, bool isKeyDown);
	bool* getInput();
	void resetInput();
	void think();

private:
	Input input;

};