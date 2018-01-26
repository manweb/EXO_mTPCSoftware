#ifndef mTPCLightSignal_hh
#define mTPCLightSignal_hh

#include <TObject.h>

class mTPCLightSignal : public TObject {
public:
    
    mTPCLightSignal();
    ~mTPCLightSignal();
	
	int GetChannel() {return fChannel;} // returns the channel of the light signal
	int GetADCCounts() {return fADCCounts;} // returns the number of ADC counts of the light signal
	
	int fChannel; // DAQ channel of the light signal
	int fADCCounts; // Number of ADC counts
	double fCorrectedSignal; // Corrected light signal

protected:
    
    ClassDef(mTPCLightSignal,1)
};

#endif /* mTPCLightSignal_hh */
