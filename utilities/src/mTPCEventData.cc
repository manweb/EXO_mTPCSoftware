#include "mTPCEventData.hh"
#include "mTPCDimensions.hh"

ClassImp(mTPCEventData)

mTPCEventData::mTPCEventData()
{
    InitializeArrays();
	fEventHeader = new mTPCEventHeader();
}

void mTPCEventData::InitializeArrays()
{
	// Initializes the light and charge signal arrays with a lengt stored in
	// mTPCDimensions.hh
	
	fLightSignals = new TClonesArray(mTPCLightSignal::Class(),N_LIGHT_SIGNALS);
	fChargeSignals = new TClonesArray(mTPCChargeSignal::Class(),N_CHARGE_SIGNALS);
	
	return;
}

mTPCLightSignal *mTPCEventData::GetNewLightSignal(int idx)
{
	// Creates a new mTPCLightSignal object and adds it to the array of light
	// signals. The TClonesArray must be initialized by calling InitializeArrays()
	// first. Returns 0 if the arrays are not initialized.
	
	if (!fLightSignals) {return 0;}
	
	TClonesArray &tmp = *fLightSignals;
	mTPCLightSignal *LS = new(tmp[idx]) mTPCLightSignal();
	
	return LS;
}

mTPCChargeSignal *mTPCEventData::GetNewChargeSignal(int idx)
{
	// Creates a new mTPCChargeSignal object and adds it to the array of charge
	// signals. The TClonesArray must be initialized by calling InitializeArrays()
	// first. Returns 0 if the arrays are not initialized.
	
	if (!fChargeSignals) {return 0;}
	
	TClonesArray &tmp = *fChargeSignals;
	mTPCChargeSignal *CS = new(tmp[idx]) mTPCChargeSignal();
	
	return CS;
}

mTPCEventData::~mTPCEventData() {;}
