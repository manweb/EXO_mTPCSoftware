#ifndef mTPCLightSignalDisplay_hh
#define mTPCLightSignalDisplay_hh

#include <TCanvas.h>
#include <TPolyLine.h>
#include <TMath.h>
#include <TClonesArray.h>
#include <TH2.h>

class mTPCLightSignalDisplay : public TCanvas {
public:
    
    mTPCLightSignalDisplay();
    ~mTPCLightSignalDisplay();
	
	void InitializeArrays();
	TClonesArray *GetLightBars() {return fLightBars;}
	void SetBarColorAt(int i, int color) {(static_cast <TPolyLine*>(fLightBars->At(i)))->SetFillColor(color); this->RedrawAxis();}
	
	TClonesArray *fLightBars;

protected:
    
    ClassDef(mTPCLightSignalDisplay,1)
};

#endif /* mTPCLightSignalDisplay_hh */
