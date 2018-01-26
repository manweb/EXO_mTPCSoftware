// This is the program that reads midas files and converts it into
// root files.

#include "mTPCConvertMidasToRoot.hh"

ClassImp(mTPCConvertMidasToRoot)

// Constructor
mTPCConvertMidasToRoot::mTPCConvertMidasToRoot()
{
    
}

int mTPCConvertMidasToRoot::SetInputFile(const char *fname)
{
	// Sets the input file.
	
	string name = fname;
	int pos1 = name.find(".mid",0);
	int diff = name.length() - pos1;
	if (diff != 4) {std::cout << "Error: The input file must be of type .mid" << std::endl; return 0;}
	
	fInputFile = fname;
	
	return 1;
}

int mTPCConvertMidasToRoot::SetOutputFile(const char *fname)
{
	// Sets the output file.
	
	string name = fname;
	int pos1 = name.find(".root",0);
	int diff = name.length() - pos1;
	if (diff != 5) {std::cout << "Error: The input file must be of type .root" << std::endl; return 0;}
	
	fOutputFile = fname;
	
	return 1;
}

void mTPCConvertMidasToRoot::Run()
{
	// Loops over all events and converts the midas file into the root file.
	// Before calling Run() an input file must be specified by calling
	// SetInputFile(). An output file can be specified by calling
	// SetOutputFile(). If no output file is specified, the same name as the
	// input file will be used.
	
	if (!fInputFile) {std::cout << "Warning: No input file is specified" << std::endl; return;}
	
	if (!fOutputFile) {
		string name = fInputFile;
		int pos = name.find(".mid");
		string name2 = name.substr(0,pos);
		name2 += ".root";
		
		fOutputFile = name2.c_str();
	}
}

// Destructor
mTPCConvertMidasToRoot::~mTPCConvertMidasToRoot() {;}
