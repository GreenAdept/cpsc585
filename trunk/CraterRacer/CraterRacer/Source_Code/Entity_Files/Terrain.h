#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

//--------------------------------------------------------
//		INCLUDES
//--------------------------------------------------------

#include "Entity.h"
#include "AIPath.h"


//---------------------------------------------------------
// Class: Terrain
//---------------------------------------------------------

class Terrain: public Entity {
public:
	//Public Interface ------------------------------------
	int       getNumberOfLaps ()             { return track.getNumberOfLaps (); }
	void      setNumberOfLaps (int l)        { track.setNumberOfLaps (l); }
	AIPath*   getTrack        ()             { return &track; }
	
private:
	//Data Members ----------------------------------------
	AIPath track;
};

#endif //TERRAIN_H