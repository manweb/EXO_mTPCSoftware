// This is an mTPC template class

#include "mTPCEventHeader.hh"

ClassImp(mTPCEventHeader)

// Constructor
mTPCEventHeader::mTPCEventHeader()
{
    Clear();
}

void mTPCEventHeader::Clear()
{
	// Resets all variables
	
	fRunNumber = 0;
	fEventNumber = 0;
	fTriggerTime = 0;
	fTriggerTimeMicro = 0;
}

// Destructor
mTPCEventHeader::~mTPCEventHeader() {;}
