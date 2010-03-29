#ifndef CONTACTREPORT_H
#define CONTACTREPORT_H

//#include "Constants.h"
#include "NxUserContactReport.h"

class ContactReport : public NxUserContactReport
{
public:
    virtual void onContactNotify(NxContactPair& pair, NxU32 events);
//	{
//		if (pair.actors[0] && pair.actors[1])
//		{
//			Emit (Events::EVehicleCollision, pair.actors[0], pair.actors[1]);
			//ActorUserData* ud = (ActorUserData*)pair.actors[0]->userData;
			//if (ud)  ud->contactEvents = events;
//		}

//		if(events & NX_NOTIFY_ON_START_TOUCH)	printf("Start touch\n");
//		if(events & NX_NOTIFY_ON_END_TOUCH)		printf("End touch\n");
/*
		// Iterate through contact points
		NxContactStreamIterator i(pair.stream);
		//user can call getNumPairs() here
		while(i.goNextPair())
		{
			//user can also call getShape() and getNumPatches() here
			while(i.goNextPatch())
			{
				//user can also call getPatchNormal() and getNumPoints() here
				const NxVec3& contactNormal = i.getPatchNormal();
				while(i.goNextPoint())
				{
					//user can also call getShape() and getNumPatches() here
					const NxVec3& contactPoint = i.getPoint();

					NxVec3 normalForceVec = NxVec3(0,0,0);
					NxVec3 frictionForceVec = NxVec3(0,0,0);
					NxVec3 penetrationVec = NxVec3(0,0,0);
					NxVec3 contactArrowForceTip = NxVec3(0,0,0);
					NxVec3 contactArrowFrictionTip = NxVec3(0,0,0);
					NxVec3 contactArrowPenetrationTip = NxVec3(0,0,0);

					ContactPoint cp(contactPoint, NxVec3(1,0,0));
					cpArray.pushBack(cp);

					// Get the normal force vector
					normalForceVec = pair.sumNormalForce;
//					normalForceVec = pair.sumNormalForce * 0.05;
					if (normalForceVec.magnitude() > 0.01)
					{
						normalForceVec.normalize();
						NxVec3 contactArrowForceTip = contactPoint + normalForceVec*3;
						ContactLine cl(contactPoint, contactArrowForceTip, NxVec3(0,0,1));
						clArray.pushBack(cl);
					}

					// Get the friction force vector
					frictionForceVec = pair.sumFrictionForce;
//					frictionForceVec = pair.sumFrictionForce* 0.05;
					if (frictionForceVec.magnitude() > 0.01)
					{
						frictionForceVec.normalize();
					    contactArrowFrictionTip = contactPoint + frictionForceVec*3;
					}

					// Get the penetration vector
					penetrationVec = - contactNormal * i.getSeparation();
//					penetrationVec = - contactNormal * i.getSeparation() * 20.0;
					if (penetrationVec.magnitude() > 0.01)
					{
						penetrationVec.normalize();
					    contactArrowPenetrationTip = contactPoint + penetrationVec*3;
					}
				}
			}
		}*/
	//}

};// gContactReport;

#endif  // CONTACTREPORT_H

