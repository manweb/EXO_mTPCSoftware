#ifndef mTPCLightDAQ_hh
#define mTPCLightDAQ_hh

#include <iostream>
#include <TSystem.h>
#include <TObject.h>
#include <TSocket.h>
#include <TFile.h>
#include <TTree.h>
#include <TTimeStamp.h>

#include "mTPCEventData.hh"
#include "mTPCEventHeader.hh"

using namespace std;

class mTPCLightDAQ : public TObject {
public:
    
    mTPCLightDAQ();
    ~mTPCLightDAQ();
	
	int ConnectToCAMAC(const char *addr, int port, int tcpwindowsize);
	int ConnectToCAMAC();
	void SetHostname(const char* hostname) {fHostname = hostname;}
	void SetPort(int port) {fPort = port;}
	void SetTCPWindowSize(int tcpwindowsize) {fTCPWindowSize = tcpwindowsize;}
	void SetOutputFilename(const char *fname) {fFilename = fname;} // Set the output filename
	void ReadChannels();
	void OpenFile(const char *fname);
	void OpenFile();
	void Finalize();

protected:
	
	int CSFA(int fcn, int slot, int channel, int *data = 0, int *Q = 0, int *X = 0);
	int CTML(int slot);
	int ResetChannels();
	int WaitForADC(int timeout = 1000);
	
	TSocket *fSocket; // The socket for the connection
	const char *fHostname; // The address of the host
	int fPort; // Port number for the connection
	int fTCPWindowSize;
	TFile *fFile; // The output file
	TTree *fTree; // The output tree
	TTimeStamp *fTimeStamp; // Event time stamp
	mTPCEventData *ed; // The event data object
	mTPCEventHeader *eh; // The event header object
	const char *fFilename; // The name of the output file
	int fEvt; // Internal event counter
    
    ClassDef(mTPCLightDAQ,1)
};

#endif /* mTPCLightDAQ_hh */
