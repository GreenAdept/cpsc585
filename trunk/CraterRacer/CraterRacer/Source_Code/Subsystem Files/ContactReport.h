#ifndef CONTACTREPORT_H
#define CONTACTREPORT_H

#include "NxUserContactReport.h"

class ContactReport : public NxUserContactReport
{
public:
    virtual void onContactNotify(NxContactPair& pair, NxU32 events);
};

#endif

