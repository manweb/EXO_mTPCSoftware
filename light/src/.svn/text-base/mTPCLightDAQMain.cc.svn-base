#include <iostream>
#include <signal.h>

#include "mTPCLightDAQ.hh"

using namespace std;

mTPCLightDAQ *daq;

void sighandler(int sig)
{
	cout << "Caught signal " << sig << endl;
    cout << "Data acquisition has been stopped by user" << endl;
	
	daq->Finalize();
	
	exit(1);
	
	return;
}

int main(int argc, char *argv[])
{
    signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);
	
	if (argc > 3) {cout << "This program only supports two arguments: the filename and the number of events" << endl; cout << "If you don't specify the number of events the program acquires data until ctrl+c is pressed" << endl;}
	
	bool forever = false;
	const char *fname;
	int nevents = 0;
	int evt = 0;
	
	if (argc == 1) {fname = "NO_NAME.root"; forever = true; cout << "Acquiring data. Press ctrl+c to stop data acquisition." << endl;}
	if (argc == 2) {fname = argv[1]; forever = true; cout << "Acquiring data. Press ctrl+c to stop data acquisition." << endl;}
	if (argc == 3) {fname = argv[1]; nevents = atoi(argv[2]); cout << "Acquiring " << nevents << " events." << endl;}
	
	daq = new mTPCLightDAQ();
	if (daq->ConnectToCAMAC() != 1) {exit(1);}
	daq->OpenFile(fname);
	
	while(evt < nevents || forever) {
		daq->ReadChannels();
		
		evt++;
	}
	
	cout << "Done acquiring data. Exiting." << endl;
	
	daq->Finalize();
	
	return 0;
}
