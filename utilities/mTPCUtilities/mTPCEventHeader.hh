#ifndef mTPCEventHeader_hh
#define mTPCEventHeader_hh

#include <TObject.h>

class mTPCEventHeader : public TObject {
public:
    
    mTPCEventHeader();
    ~mTPCEventHeader();
	
	void Clear();
	
	unsigned int fRunNumber; // Run number
	unsigned int fEventNumber; // Event number
	unsigned int fTriggerTime; // Time of the trigger in seconds
	unsigned int fTriggerTimeMicro; // Micro seconds of the trigger time
	unsigned int fNumberOfGroups; // Number of groups
	unsigned int fNumberOfChannels; // Number of channels
	unsigned int fNumberOfSamples; // Number of samples
	
	// Midas variables
	unsigned int fMidasEventID;
	unsigned int fMidasTriggerMask;
	unsigned int fMidasSerialNb;
	unsigned int fMidasTimestamp;
	unsigned int fMidasDataSize;
	
	// Caen variables
	unsigned int fCaenID;
	unsigned int fCaenEventSize;
	unsigned int fCaenBoardID;
	unsigned int fCaenRes;
	unsigned int fCaenPattern;
	unsigned int fCaenGroupMask;
	unsigned int fCaenReserved;
	unsigned int fCaenEventCount; // <- Same as fEventNumber
	unsigned int fCaenTriggerTimeTag;

protected:
    
    ClassDef(mTPCEventHeader,1)
};

#endif /* mTPCEventHeader_hh */
