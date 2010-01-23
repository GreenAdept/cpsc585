#include "Input.h"
#include "AI.h"

class VehicleAI : public AI
{
public:
	VehicleAI();
	~VehicleAI();
private:
	Input input;

};