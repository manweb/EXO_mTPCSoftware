#ifndef mTPCEventData_hh
#define mTPCEventData_hh

#include <TObject.h>
#include <TClonesArray.h>
#include "mTPCLightSignal.hh"
#include "mTPCChargeSignal.hh"
#include "mTPCEventHeader.hh"

class mTPCEventData : public TObject {
public:
    
    mTPCEventData();
    ~mTPCEventData();
	
	void InitializeArrays();
	mTPCLightSignal *GetNewLightSignal(int idx);
	mTPCChargeSignal *GetNewChargeSignal(int idx);
	TClonesArray *GetLightSignals() {return fLightSignals;}    // returns the array of light signals
	TClonesArray *GetChargeSignals() {return fChargeSignals;}  // returns the array of charge signals
	mTPCLightSignal *GetLightSignalAt(int n) {return static_cast <mTPCLightSignal*>(fLightSignals->At(n));} // returns the light signal at entry n
	mTPCChargeSignal *GetChargeSignalAt(int n) {return static_cast <mTPCChargeSignal*> (fChargeSignals->At(n));} // returns the charge signal at entry n
	mTPCEventHeader *GetEventHeader() {return fEventHeader;} // returns the event header
	int GetNumLightSignals() {return fLightSignals->GetEntriesFast();} // returns the number of light signals
	int GetNumChargeSignals() {return fChargeSignals->GetEntriesFast();} // returns the number of charge signals

protected:
	
	TClonesArray *fLightSignals; //->
	
	TClonesArray *fChargeSignals; //->
	
	mTPCEventHeader *fEventHeader; //->
    
    ClassDef(mTPCEventData,1)
};

#endif /* mTPCEventData_hh */
