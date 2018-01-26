// This is the DAQ program for the light system

#include "mTPCLightDAQ.hh"
#include "mTPCDimensions.hh"

ClassImp(mTPCLightDAQ)

// Constructor
mTPCLightDAQ::mTPCLightDAQ()
{
	fHostname = "130.92.139.12";
	fPort = 2000;
	fTCPWindowSize = 65536;
	
	fEvt = 0;
}

int mTPCLightDAQ::ConnectToCAMAC(const char *addr, int port, int tcpwindowsize)
{
	// Establishes a connection to the CAMAC module at the address given by addr
	// over the specified port. Returns 1 if connection was successful and 0 if
	// connection failed.
	
	fSocket = new TSocket(addr,port,tcpwindowsize);
	fSocket->SetOption(kNoBlock,1);
	
	if (fSocket->IsValid()) {return 1;}
	else {return 0;}
}

int mTPCLightDAQ::ConnectToCAMAC()
{
	// Establishes a connection to the CAMAC module at the address given by fHostname
	// over the port specified by fPort. Default values for fHostname and fPort are set
	// in the constructor and can be changed with SetHostname() and SetPort(). Returns 1
	// if connection was successful and 0 if connection failed.
	
	fSocket = new TSocket(fHostname,fPort,fTCPWindowSize);
	fSocket->SetOption(kNoBlock,1);
	
	if (fSocket->IsValid()) {return 1;}
	else {return 0;}
}

void mTPCLightDAQ::OpenFile(const char *fname)
{
	// Opens the output file and prepares the tree. The filename is specified by fname.
	
	fFilename = fname;
	OpenFile();
	
	return;
}

void mTPCLightDAQ::OpenFile()
{
	// Opens the output file and prepares the tree. The filename can be set with
	// SetOutputFilename().
	
	if (fFilename == NULL) {cout << "Error: No filename specified" << endl; return;}
	
	fFile = new TFile(fFilename,"RECREATE");
	fTree = new TTree("tree","tree");
	
	ed = new mTPCEventData();
	eh = ed->GetEventHeader();
	
	fTree->Branch("EventBranch",&ed);
	
	return;
}

int mTPCLightDAQ::CSFA(int fcn, int slot, int channel, int *data, int *Q, int *X)
{
	// Sends the command CSFA to the CAMAC controller. The function is specified by fcn.
	// Returns 1 if the data was retrieved successfully and 0 if an error occurred.
	
	char buf[1024];
	char ibuf[1024];
	int rep,qrep,pdat;
	
	if (!fSocket) {cout << "Error: No connection to CAMAC controller. Call ConnectToCAMAC() first." << endl; return 0;}
	
	sprintf(buf,"CFSA %d %d %d %d\n\r",fcn,slot,channel,*data);
	fSocket->SendRaw(buf, strlen(buf)); // Send the command
	if (fSocket->Select(TSocket::kRead,1000) == 0) {cout << "Error: Timeout while reading from CAMAC." << endl; return 0;}
	
	gSystem->Sleep(2);
	fSocket->RecvRaw(ibuf, 1024); // Recieve the data
	sscanf(ibuf,"%d %d %x",&rep, &qrep, &pdat);
	
	*data = pdat;
	*Q = qrep;
	*X = rep;
	
	return 1;
}

int mTPCLightDAQ::CTML(int slot)
{
	char buf[1024];
	char ibuf[1024];
	int rep, lam;
	
	if (!fSocket) {cout << "Error: No connection to CAMAC controller. Call ConnectToCAMAC() first." << endl; return 0;}
	
	sprintf(buf,"CTLM %d\n\r",slot);
	fSocket->SendRaw(buf, strlen(buf)); // Send the command
	if (fSocket->Select(TSocket::kRead,1000) == 0) {cout << "Error: Timeout while reading from CAMAC." << endl; return 0;}
	
	gSystem->Sleep(2);
	fSocket->RecvRaw(ibuf, 1024); // Recieve the data
	sscanf(ibuf,"%d %d",&rep,&lam);
	
	return lam;
}

int mTPCLightDAQ::ResetChannels()
{
	// Resets all channels
	
	int ret;
	int data = 0;
	int Q = 0;
	int X = 0;
	
	for (int i = 0; i < N_LIGHT_CHANNELS; i++) {
		int slot = 23 - int(i/12);
		int channel = i%12;
		
		ret = CSFA(9,slot,channel,&data,&Q,&X);
	}
	
	return 1;
}

int mTPCLightDAQ::WaitForADC(int timeout)
{
	// Waits for the ADC to be ready.
	
	int nslots = N_LIGHT_CHANNELS / 12;
	int tme = 0;
	
	for (int i = 0; i < nslots; i++) {
		tme = timeout;
		int slot = 23 - i;
		while(CTML(slot) != 1 && tme > 0) {tme--; gSystem->Sleep(1);}
	}
	if(tme==0) {return 0;}
	
	return 1;
}

void mTPCLightDAQ::ReadChannels()
{
	// Reads the ADC values of all channels. If an output file is open it saves
	// the values to the tree. Otherwise it prints the values.
	
	ResetChannels();
	
	if (WaitForADC() == 0) {return;}
	
	cout << fEvt << "> ";
	
	for (int i = 0; i < N_LIGHT_CHANNELS; i++) {
		int data = 0;
		int Q = 0;
		int X = 0;
		int slot = 23 - int(i/12);
		int channel = i%12;
		
		CSFA(0,slot,channel,&data,&Q,&X);
		
		if (fTree) {
			mTPCLightSignal *lh = ed->GetNewLightSignal(i);
			lh->fADCCounts = data;
			lh->fChannel = i;
			lh->fCorrectedSignal = DUMMY_DOUBLE;
		}
		
		cout << data << "\t";
	}
	
	if (fTree) {
		fTimeStamp = new TTimeStamp();
		eh->fTriggerTime = fTimeStamp->GetSec();
		eh->fTriggerTimeMicro = fTimeStamp->GetNanoSec()/1000;
		eh->fRunNumber = 0;
		eh->fEventNumber = fEvt;
		
		fTree->Fill();
	}
	
	fEvt++;
	
	cout << endl;
	
	return;
}

void mTPCLightDAQ::Finalize()
{
	if (fTree) {
		fTree->Write();
		fFile->Close();
	}
	
	if (fSocket) {fSocket->Close();}
	
	return;
}

// Destructor
mTPCLightDAQ::~mTPCLightDAQ() {;}
