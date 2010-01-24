#include "Input.h"
#include "AI.h"

class VehicleAI : public AI
{
public:
	VehicleAI();
	void setInput(Input::Arrow dir, bool isKeyDown);
	bool* getInput();

private:
	Input input;

};