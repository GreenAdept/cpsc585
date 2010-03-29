#include "ContactReport.h"
#include "MessageManager.h"

void ContactReport::onContactNotify(NxContactPair& pair, NxU32 events)
{
	if (pair.actors[0] && pair.actors[1])
	{
		UserData *user1 = (UserData*)pair.actors[0]->userData;
		UserData *user2 = (UserData*)pair.actors[1]->userData;
		Emit (Events::EVehicleCollision, user1->id, user2->id);
		//ActorUserData* ud = (ActorUserData*)pair.actors[0]->userData;
		//if (ud)  ud->contactEvents = events;
	}
}