#ifndef AI_H
#define AI_H

//---------------------------------------------
// Class: AI
// AI abstract base class, from which concrete
// AI classes will be derived.
//---------------------------------------------

class AI {
public:
	virtual void think () = 0;
};

#endif