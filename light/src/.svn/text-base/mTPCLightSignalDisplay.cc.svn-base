// Class to display the light signals

#include "mTPCLightSignalDisplay.hh"

ClassImp(mTPCLightSignalDisplay)

// Constructor
mTPCLightSignalDisplay::mTPCLightSignalDisplay() : TCanvas("c1","Light Display",600,600)
{
    InitializeArrays();
}

void mTPCLightSignalDisplay::InitializeArrays()
{
	fLightBars = new TClonesArray(TPolyLine::Class(),22);
	
	TH2F *h = new TH2F("h","h",100,-1,1,100,-1,1);
	h->Draw();
	
	TClonesArray &tmp = *fLightBars;
	
	double stp = TMath::Pi()/11;
	double a1 = 0.06241880999595735;
	double a2 = 0.05875582271572271;
	double r1 = 0.8015609770940698;
	double r2 = 0.85146931829632;
	
	for (int i = 0; i < 22; i++) {
		double x[5] = {TMath::Cos(i*stp+a1)*r1,TMath::Cos(i*stp-a1)*r1,TMath::Cos(i*stp-a2)*r2,TMath::Cos(i*stp+a2)*r2,TMath::Cos(i*stp+a1)*r1};
		double y[5] = {TMath::Sin(i*stp+a1)*r1,TMath::Sin(i*stp-a1)*r1,TMath::Sin(i*stp-a2)*r2,TMath::Sin(i*stp+a2)*r2,TMath::Sin(i*stp+a1)*r1};
		
		TPolyLine *p = new(tmp[i]) TPolyLine(5,x,y);
		
		p->SetFillStyle(1001);
		p->SetLineColor(1);
		p->SetFillColor(1);
		
		p->Draw("Fsame");
	}
	
	return;
}

// Destructor
mTPCLightSignalDisplay::~mTPCLightSignalDisplay() {;}
