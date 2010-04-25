#include "ContactReport.h"
#include "MessageManager.h"

void ContactReport::onContactNotify(NxContactPair& pair, NxU32 events)
{
	if (pair.actors[0] && pair.actors[1])
	{
		//for vibrations
		UserData *user1 = (UserData*)pair.actors[0]->userData;
		UserData *user2 = (UserData*)pair.actors[1]->userData;
		if (user1->id < 2)
			Emit(Events::EVibrate, user1->id, 80, 30);
		if (user2->id < 2)
			Emit(Events::EVibrate, user2->id, 80, 30);
		//ActorUserData* ud = (ActorUserData*)pair.actors[0]->userData;
		//if (ud)  ud->contactEvents = events;
	}
}