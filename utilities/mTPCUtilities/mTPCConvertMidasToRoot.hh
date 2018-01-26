#ifndef mTPCConvertMidasToRoot_hh
#define mTPCConvertMidasToRoot_hh

#include <TObject.h>

class mTPCConvertMidasToRoot : public TObject {
public:
    
    mTPCConvertMidasToRoot();
    ~mTPCConvertMidasToRoot();
	
	int SetInputFile(const char *fname);
	int SetOutputFile(const char *fname);

protected:
	
	const char *fInputFile; // The input file
	const char *fOutputFile; // The output file
    
    ClassDef(mTPCConvertMidasToRoot,1)
};

#endif /* mTPCConvertMidasToRoot_hh */
