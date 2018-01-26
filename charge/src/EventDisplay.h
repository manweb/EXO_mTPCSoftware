#include <TROOT.h>
#include <TApplication.h>
#include <TGClient.h>
#include <TQObject.h>
#include <TGButton.h>
#include <TGLayout.h>
#include <TGXYLayout.h>
#include <TGWindow.h>
#include <TGFileBrowser.h>
#include <TGFileDialog.h>
#include <TGFrame.h>
#include <TGMenu.h>
#include <TRootEmbeddedCanvas.h>
#include <TGTab.h>
#include <TF1.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TGToolBar.h>
#include <TGStatusBar.h>
#include <TPostScript.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGComboBox.h>
#include <TG3DLine.h>
#include <TSystem.h>
#include <TRootCanvas.h>
#include <TGraph.h>
#include <TH2.h>
#include <TStyle.h>
#include <TBrowser.h>
#include <TLine.h>
#include <TText.h>
#include <TGIcon.h>
#include <TGaxis.h>
#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>
#include <TColor.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <time.h>

#include "TMath.h"
#include "Data.h"

using namespace std;

const char version[]="Event Display v1.3";

class TRootEmbeddedCanvasDerived;

class display : public TGMainFrame
{
private:
   TGMainFrame *PrintDialog;
   TGPopupMenu *fMenuFile;
   TGPopupMenu *fMenuOptions;
   TGPopupMenu *fMenuTools;
   //TGPopupMenu *fMenuTest;tttttttttttttttttttttt
   TGMenuBar *fMenuBar;
   TGTab *fTabMenu;
   TGTab *fTabPlots;
   TRootEmbeddedCanvas *fEcanvasMulti[100];
   //TRootEmbeddedCanvasDerived *fEcanvasMulti[100];
   TGCheckButton *fCheck[11];
   TGCheckButton *fCheckNewtab;
   TGCheckButton *fCheckSameRange;
   TGCheckButton *fCheckIntegratedSignals;
   TGCheckButton *fCheckSingleCH;
   TGCheckButton *fCheckConvert;
   TGCheckButton *fCheckXt;
   TGCheckButton *fCheckYt;
   TGCheckButton *fCheckGridt;
   TGCheckButton *fCheckManColorScale;
   TGCheckButton *fCheckFiltering;
   TGCheckButton *fCheckDispSpec;
   TGCheckButton *fCheckShowFilter;
   TGCheckButton *fCheckDBValues;
   TGNumberEntry *fEventID;
   TGComboBox *fCBUnit;
   TGComboBox *fCBFilterType;
   TGComboBox *fCBf1Unit;
   TGComboBox *fCBf2Unit;
   TGComboBox *fCBGas;
   TGComboBox *fChMap;
   TGTextButton *fButtonDraw;
   TGTextButton *fButtonClear;
   TGNumberEntry *TGdriftVelocity;
   TGNumberEntry *TGchannelmapping; //tttt*/
   TGNumberEntry *TGstripDistance;
   TGNumberEntry *TGsignalThreshold;
   TGNumberEntry *TGbinScaling;
   TGNumberEntry *TGgridChannel;
   TGNumberEntry *TGMaxTab;
   TGNumberEntry *TGSingleCH;
   TGNumberEntry *TGcutoff1;
   TGNumberEntry *TGcutoff2;
   TGNumberEntry *TGFilterOrder;
   TGNumberEntryField *TGmaxColorRange;
   TGTextEntry *TGCommand;
   TGTextEntry *TGPrinter;
   TGStatusBar *stat;
   TGStatusBar *stat2;

   Run_t * run; // only object needed for data aceess

   TH2D *h21;
   TH2D *h22;

   string currentRunName;
   int nevent;
   int nchannel;
   int nsamples;
   int ngroup;
   double duration;
   time_t date;
   char printfName[80];
   int *chMapping;
   double SQLchannelMapping;
   double SQLdriftVelocity;

public:
   display(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~display();
   void Init();
   void CheckUpAll();
   void DisableAll();
   void OpenFile();
   void ChangeSameRange();
   void ChangeFiltering();
   void ChangeUseDB();
   void ChangeChMap();
   void ChangeMaxColorRange();
   void CloseTab();
   void CloseAllTabs();
   void Draw();
   void GetPreviousEvent();
   void GetNextEvent();
   void CreateCanvas(string title);
   void SaveCanvas();
   void PrintCanvas();
   void ExecPrint();
   void GetRunInfo();
   int GetFileExt(string filename);
   void GetMinMaxData(double *min, double *max);
   void ImportSettings();
   void ExportSettings();
   Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

   ClassDef(display,1)
};

/*class TRootEmbeddedCanvasDerived : public TRootEmbeddedCanvas
{
public:
   TRootEmbeddedCanvasDerived(const char* name = 0, const TGWindow* p = 0, UInt_t w = 10, UInt_t h = 10, UInt_t options = kSunkenFrame|kDoubleBorder, Pixel_t back = GetDefaultFrameBackground()) : TRootEmbeddedCanvas(name, p, w, h, options, back) { }
   Bool_t HandleContainerMotion(Event_t *event);
};*/
