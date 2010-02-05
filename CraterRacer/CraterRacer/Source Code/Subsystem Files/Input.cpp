/*
The input system.
*/

#include "Input.h"

/*
The constructor for Input. Just sets all the values to false.
*/
Input::Input() 
{
	reset();
	reversing = false;
}

/*
Makes all the directions false.
*/
void Input::reset()
{
	inputs[Arrow::LEFT] = false;
	inputs[Arrow::UP] = false;
	inputs[Arrow::RIGHT] = false;
	inputs[Arrow::DOWN] = false;

	direction = Vec3(0.0, 0.0, 0.0);
}

/*
Set the direction to true if the key is currently down, and false 
when the player releases the key.
*/
void Input::setInput(Input::Arrow dir, bool isKeyDown)
{
	inputs[dir] = isKeyDown;
}

void Input::setDir(Vec3 vec)
{
	direction = vec;
}

/*
Returns the directions array.
*/
bool* Input::getInput()
{
	return inputs;
}

Vec3 Input::getDir()
{
	return direction;
}

//--------------------------------------------------------------------------------------
// Function: drive
// Determines which direction to go in, and force if applicable.
//--------------------------------------------------------------------------------------
void Input::drive(Vec3 dir, bool accelerate, bool decelerate, Vec3 velocity)
{
	if (accelerate)
	{
		reversing = false;
		dir.z = 1.0;
		//if speed >= max_speed, maintain max_speed
		direction = 10 * dir;
	}
	else if (decelerate)
	{
		//if speed > 0, slow down but go in user's direction
		//else reverse, go in opposite direction
		double speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

		//writer.writeToFile(speed);	
		if ((speed >= 0.01) && (!reversing))
		{
			dir.z = 0.5;
			Vec3 oppositeForce = (-5) * velocity; //the -1 needs to be CHANGED to a braking constant
			direction = oppositeForce + dir * 0.2 * speed;
		}
		else
		{
			reversing = true;
			dir.z = 1.0;
			direction = (-10) * dir; //say we can go backwards as fast as we can go forwards
		}
	}
	else
	{
		//go in user's direction, and let friction slow it down
		direction = dir;
	}

	//writer.writeToFile(direction);
}

void Input::drive(Input::Arrow dir, bool isKeyDown)
{
}
/*
void Input::drive(Input::Arrow dir, bool isKeyDown)
{
	if (isKeyDown)
	{
		switch(dir) {
			case (Arrow::UP):
				{
					reversing = false;
				}
			case (Arrow::LEFT):
				{

				}
			case (Arrow::RIGHT):
				{
				}
			case (Arrow::DOWN):
			default:
				break;
		}
	}
}
*/