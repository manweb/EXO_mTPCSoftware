// **** EventDisplay ******************************************************************************
// *                                                                                              *
// * Manuel Weber                                                                                 *
// * LHEP, University of Bern                                                                     *
// * 05.2010                                                                                      *
// * v1.3                                                                                         *
// *                                                                                              *
// ************************************************************************************************

#include "EventDisplay.h"

enum ECommandIdentifiers {
   M_FILE_OPEN,
   M_FILE_SAVE,
   M_FILE_CLOSE,
   M_FILE_IMPORT,
   M_FILE_EXPORT,
   M_FILE_PRINT,
   M_FILE_EXIT,
   M_OPTIONS_STATS,
   M_OPTIONS_NAMES,
   M_OPTIONS_CROSSHAIR,
   M_TOOLS_NEWTAB,
   M_TOOLS_CLOSETAB,
   M_TOOLS_CLEARTABS,
   M_TOOLS_NEWCANVAS,
   M_TOOLS_OPENBROWSER,
   M_TOOLS_RUNINFO,
   TB_BUTTON_NEW,
   TB_BUTTON_OPEN,
   TB_BUTTON_SAVE,
   TB_BUTTON_CLOSE,
   TB_BUTTON_PREVIOUS,
   TB_BUTTON_NEXT,
   TB_BUTTON_INFO,
   PRINT_BUTTON,
   CB_UNIT,
   CB_F1UNIT,
   CB_F2UNIT,
   CB_FILTERTYPE,
   CB_GAS,
   CB_CHMAP
};

display::display(const TGWindow *p, UInt_t w, UInt_t h)
    : TGMainFrame(p, w, h)
{
// **** MENU BAR **********************************************************************************
   // popup menu "File"
   fMenuFile = new TGPopupMenu(gClient->GetRoot());

   // adding menu entries
   fMenuFile->AddEntry("&Open...",M_FILE_OPEN);
   fMenuFile->AddEntry("&Save as...",M_FILE_SAVE);
   fMenuFile->AddEntry("&Close", M_FILE_CLOSE);

   // adding separator
   fMenuFile->AddSeparator();

   fMenuFile->AddEntry("&Import settings...",M_FILE_IMPORT);
   fMenuFile->AddEntry("&Export settings...",M_FILE_EXPORT);

   // adding separator
   fMenuFile->AddSeparator();

   // next group of menu entries
   fMenuFile->AddEntry("&Print...",M_FILE_PRINT);

   fMenuFile->AddSeparator();
   fMenuFile->AddEntry("E&xit",M_FILE_EXIT);

   fMenuFile->Associate(this);

   //fMenuFile->DisableEntry(M_FILE_SAVE);

   // popup menu "Options"
   fMenuOptions = new TGPopupMenu(gClient->GetRoot());

   // adding menu entries
   fMenuOptions->AddEntry("Statistics",M_OPTIONS_STATS);
   fMenuOptions->AddEntry("Graph titles",M_OPTIONS_NAMES);
   fMenuOptions->AddEntry("Set crosshair",M_OPTIONS_CROSSHAIR);

   fMenuOptions->CheckEntry(M_OPTIONS_NAMES);

   fMenuOptions->Associate(this);

   // popup menu "Tools"
   fMenuTools = new TGPopupMenu(gClient->GetRoot());

   // adding menu entries
   fMenuTools->AddEntry("&New tab",M_TOOLS_NEWTAB);
   fMenuTools->AddEntry("Close tab",M_TOOLS_CLOSETAB);
   fMenuTools->AddEntry("Clear tabs",M_TOOLS_CLEARTABS);

   // adding separator
   fMenuTools->AddSeparator();

   fMenuTools->AddEntry("New canvas",M_TOOLS_NEWCANVAS);
   fMenuTools->AddEntry("Open browser",M_TOOLS_OPENBROWSER);

   fMenuTools->AddSeparator();

   fMenuTools->AddEntry("Run info",M_TOOLS_RUNINFO);

   fMenuTools->Associate(this);

   // menu bar
   fMenuBar = new TGMenuBar(this,w,20,kHorizontalFrame);

   // adding popup menus
   fMenuBar->AddPopup("&File",fMenuFile,new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 4));
   fMenuBar->AddPopup("&Options",fMenuOptions,new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 4));
   fMenuBar->AddPopup("&Tools",fMenuTools,new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 4));
// ************************************************************************************************

// **** TOOL BAR **********************************************************************************
   // structure containing toolbar button information
   ToolBarData_t t[7];

   // create toolbar object
   TGToolBar *tb = new TGToolBar(this,w,80);

   char *installDir = getenv("READMIDASSYS");

   if (!installDir) {
      cout << "READ_MIDASSYS not set!" << endl;
      cout << "Aborted!" << endl;
      return;
   }

   char picDir[50];
   sprintf(picDir, "%s/icons/new.xpm", installDir);
   t[0].fPixmap = picDir; // icon file
   //t[0].fPixmap = "icons/new.xpm"; // icon file
   t[0].fTipText = "New tab"; // tool tip text
   t[0].fStayDown = kFALSE; // button behavior if clicked
   t[0].fId = TB_BUTTON_NEW; // button id
   t[0].fButton = NULL; // button pointer
   tb->AddButton(this,&t[0]);

   char picDir2[50];
   sprintf(picDir2, "%s/icons/open.xpm", installDir);
   t[1].fPixmap = picDir2; // icon file
   //t[1].fPixmap = "icons/open.xpm"; // icon file
   t[1].fTipText = "Open file"; // tool tip text
   t[1].fStayDown = kFALSE; // button behavior if clicked
   t[1].fId = TB_BUTTON_OPEN; // button id
   t[1].fButton = NULL; // button pointer
   tb->AddButton(this,&t[1]);

   char picDir3[50];
   sprintf(picDir3, "%s/icons/save.xpm", installDir);
   t[2].fPixmap = picDir3; // icon file
   //t[2].fPixmap = "icons/save.xpm"; // icon file
   t[2].fTipText = "Save canvas"; // tool tip text
   t[2].fStayDown = kFALSE; // button behavior if clicked
   t[2].fId = TB_BUTTON_SAVE; // button id
   t[2].fButton = NULL; // button pointer
   tb->AddButton(this,&t[2]);

   char picDir4[50];
   sprintf(picDir4, "%s/icons/close.xpm", installDir);
   t[3].fPixmap = picDir4; // icon file
   //t[3].fPixmap = "icons/close.xpm"; // icon file
   t[3].fTipText = "Close tab"; // tool tip text
   t[3].fStayDown = kFALSE; // button behavior if clicked
   t[3].fId = TB_BUTTON_CLOSE; // button id
   t[3].fButton = NULL; // button pointer
   tb->AddButton(this,&t[3]);

   char picDir5[50];
   sprintf(picDir5, "%s/icons/backNew.xpm", installDir);
   t[4].fPixmap = picDir5; // icon file
   //t[4].fPixmap = "icons/backNew.xpm"; // icon file
   t[4].fTipText = "Previous event"; // tool tip text
   t[4].fStayDown = kFALSE; // button behavior if clicked
   t[4].fId = TB_BUTTON_PREVIOUS; // button id
   t[4].fButton = NULL; // button pointer
   tb->AddButton(this,&t[4]);

   char picDir6[50];
   sprintf(picDir6, "%s/icons/forwardNew.xpm", installDir);
   t[5].fPixmap = picDir6; // icon file
   //t[5].fPixmap = "icons/forwardNew.xpm"; // icon file
   t[5].fTipText = "Next event"; // tool tip text
   t[5].fStayDown = kFALSE; // button behavior if clicked
   t[5].fId = TB_BUTTON_NEXT; // button id
   t[5].fButton = NULL; // button pointer
   tb->AddButton(this,&t[5]);

   char picDir7[50];
   sprintf(picDir7, "%s/icons/about.xpm", installDir);
   t[6].fPixmap = picDir7; // icon file
   //t[6].fPixmap = "icons/about.xpm"; // icon file
   t[6].fTipText = "Run information"; // tool tip text
   t[6].fStayDown = kFALSE; // button behavior if clicked
   t[6].fId = TB_BUTTON_INFO; // button id
   t[6].fButton = NULL; // button pointer
   tb->AddButton(this,&t[6],5);

   TGHorizontal3DLine *lh = new TGHorizontal3DLine(this);
// ************************************************************************************************

// **** TABS **************************************************************************************
   // create horizontal frame containing tabs
   TGHorizontalFrame *hFrameTabs = new TGHorizontalFrame(this);
   hFrameTabs->SetLayoutManager(new TGHorizontalLayout(hFrameTabs)); // set horizontal arrangement of the tabs

   // create menu tab
   fTabMenu = new TGTab(hFrameTabs);

   // container for tab "Run"
   TGCompositeFrame *fCompositeFrameRun;
   fCompositeFrameRun = fTabMenu->AddTab("Run");
   fCompositeFrameRun->SetLayoutManager(new TGVerticalLayout(fCompositeFrameRun)); // set vertical arrangement within the tab

   // container for tab "Settings"
   TGCompositeFrame *fCompositeFrameSettings;
   fCompositeFrameSettings = fTabMenu->AddTab("Settings");
   fCompositeFrameSettings->SetLayoutManager(new TGVerticalLayout(fCompositeFrameSettings)); // set vertical arrangement within the tab

   // container for tab "Filtering"
   TGCompositeFrame *fCompositeFrameFiltering;
   fCompositeFrameFiltering = fTabMenu->AddTab("Filtering");
   fCompositeFrameFiltering->SetLayoutManager(new TGVerticalLayout(fCompositeFrameFiltering)); // set vertical arrangement within the tab

   // create plots tab
   fTabPlots = new TGTab(hFrameTabs);

   // container of "Tab1"
   //TGCompositeFrame *fCompositeFrame1;
   //fCompositeFrame1 = fTabPlots->AddTab("Welcome");
   //fCompositeFrame1->SetLayoutManager(new TGVerticalLayout(fCompositeFrame1));

   hFrameTabs->AddFrame(fTabMenu, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
   hFrameTabs->AddFrame(fTabPlots, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,2,2,2,2));

   // select tab1
   fTabPlots->SetTab(0);
   fTabMenu->SetTab(0);
// ************************************************************************************************

// **** CONTENT OF RUN TAB ************************************************************************
   // create group frame "Group"
   TGGroupFrame *GFGroup = new TGGroupFrame(fCompositeFrameRun,"Group selection");
   GFGroup->SetLayoutManager(new TGXYLayout(GFGroup)); // set XY layout to set size of group frame
   GFGroup->MoveResize(0,0,220,280);

   // create check buttons for group selection
   fCheck[0] = new TGCheckButton(GFGroup, new TGHotString("Group 0  (ch  0 to  7)"));
   fCheck[1] = new TGCheckButton(GFGroup, new TGHotString("Group 1  (ch  8 to 15)"));
   fCheck[2] = new TGCheckButton(GFGroup, new TGHotString("Group 2  (ch 16 to 23)"));
   fCheck[3] = new TGCheckButton(GFGroup, new TGHotString("Group 3  (ch 24 to 31)"));
   fCheck[4] = new TGCheckButton(GFGroup, new TGHotString("Group 4  (ch 32 to 39)"));
   fCheck[5] = new TGCheckButton(GFGroup, new TGHotString("Group 5  (ch 40 to 47)"));
   fCheck[6] = new TGCheckButton(GFGroup, new TGHotString("Group 6  (ch 48 to 55)"));
   fCheck[7] = new TGCheckButton(GFGroup, new TGHotString("Group 7  (ch 56 to 63)"));
   fCheck[8] = new TGCheckButton(GFGroup, new TGHotString("2D Projections"));
   fCheck[9] = new TGCheckButton(GFGroup, new TGHotString("Select all"));

   fCheck[9]->Connect("Clicked()","display",this,"CheckUpAll()");

   // add check buttons to the frame
   for (int i=0; i<10; i++) {
      GFGroup->AddFrame(fCheck[i], new TGXYLayoutHints(2,2+i*2,25,1.5));
      fCheck[i]->SetState(kButtonDisabled);
   }

   // create group frame "Single channel"
   TGGroupFrame *GFSingleCH = new TGGroupFrame(fCompositeFrameRun,"Single channel");
   GFSingleCH->SetLayoutManager(new TGXYLayout(GFSingleCH));
   GFSingleCH->MoveResize(0,0,220,100);

   TGLabel *fLabelSCH = new TGLabel(GFSingleCH,"Display single channel:");
   fLabelSCH->SetTextJustify(kTextLeft);
   GFSingleCH->AddFrame(fLabelSCH,new TGXYLayoutHints(2,2,20,1.8));

   fCheckSingleCH = new TGCheckButton(GFSingleCH);
   fCheckSingleCH->SetTextJustify(kTextRight);
   fCheckSingleCH->SetState(kButtonDisabled);
   GFSingleCH->AddFrame(fCheckSingleCH, new TGXYLayoutHints(27,2,2,1.8));

   fCheckSingleCH->Connect("Clicked()","display",this,"DisableAll()");

   TGLabel *fLabelCHSelect = new TGLabel(GFSingleCH,"Select channel:");
   fLabelCHSelect->SetTextJustify(kTextLeft);
   GFSingleCH->AddFrame(fLabelCHSelect,new TGXYLayoutHints(2,4.5,20,1.8));

   TGSingleCH = new TGNumberEntry(GFSingleCH,(int)0,5,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 0, (TGNumberFormat::ELimit) 3,0,63);
   TGSingleCH->SetNumber(0);
   GFSingleCH->AddFrame(TGSingleCH, new TGXYLayoutHints(22,4.5,7,1.8));

   // create group frame "Drawing"
   TGGroupFrame *GFDrawing = new TGGroupFrame(fCompositeFrameRun,"Drawing");
   GFDrawing->SetLayoutManager(new TGXYLayout(GFDrawing));
   GFDrawing->MoveResize(0,0,220,130);

   TGLabel *fLabelEvtID = new TGLabel(GFDrawing,"Event ID:");
   fLabelEvtID->SetTextJustify(kTextLeft);
   GFDrawing->AddFrame(fLabelEvtID,new TGXYLayoutHints(2,2,10,1.8));

   // Event selcetion
   fEventID = new TGNumberEntry(GFDrawing,(int)0,5,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 0,(TGNumberFormat::ELimit)3,0,0);
   GFDrawing->AddFrame(fEventID,new TGXYLayoutHints(15,2,8,1.8));

   TGLabel *fLabelUnit = new TGLabel(GFDrawing,"Unit:",kTextLeft);
   fLabelUnit->SetTextJustify(kTextLeft);
   GFDrawing->AddFrame(fLabelUnit,new TGXYLayoutHints(2,4.5,10,1.8));

   // Unit selection
   fCBUnit = new TGComboBox(GFDrawing,CB_UNIT);
   fCBUnit->AddEntry("nA",0);
   fCBUnit->AddEntry("mV",1);
   fCBUnit->AddEntry("ADC",2);
   GFDrawing->AddFrame(fCBUnit, new TGXYLayoutHints(15,4.5,8,1.8));
   fCBUnit->Select(0);

   fButtonDraw = new TGTextButton(GFDrawing,"Draw");
   fButtonDraw->MoveResize(40,85,60,30);
   fButtonDraw->Connect("Clicked()","display",this,"Draw()");
   fButtonDraw->SetState(kButtonDisabled);

   fButtonClear = new TGTextButton(GFDrawing,"Clear");
   fButtonClear->MoveResize(120,85,60,30);
   fButtonClear->Connect("Clicked()","display",this,"CloseAllTabs()");
   fButtonClear->SetState(kButtonDisabled);

   // add group frames to the tab
   fCompositeFrameRun->AddFrame(GFGroup, new TGLayoutHints(kLHintsLeft,5,5,5,5));
   fCompositeFrameRun->AddFrame(GFSingleCH, new TGLayoutHints(kLHintsLeft,5,5,5,5));
   fCompositeFrameRun->AddFrame(GFDrawing, new TGLayoutHints(kLHintsLeft,5,5,5,5));
// ************************************************************************************************

// **** CONTENT OF SETTINGS TAB *******************************************************************
   // create group frame "Properties"
   TGGroupFrame *GFProperties = new TGGroupFrame(fCompositeFrameSettings,"Properties");
   GFProperties->SetLayoutManager(new TGXYLayout(GFProperties)); // set XY layout to set size of group frame
   GFProperties->MoveResize(0,0,220,280);

   // drift velocity selection
   TGdriftVelocity = new TGNumberEntry(GFProperties,(int)0,5,-1,(TGNumberFormat::EStyle) 2,(TGNumberFormat::EAttribute) 0, (TGNumberFormat::ELimit) 1,0);
   TGdriftVelocity->SetNumber(4.7);
   GFProperties->AddFrame(TGdriftVelocity, new TGXYLayoutHints(14,2,9,1.8));

   // strip distance selection
   TGstripDistance = new TGNumberEntry(GFProperties,(int)0,5,-1,(TGNumberFormat::EStyle) 4,(TGNumberFormat::EAttribute) 0,(TGNumberFormat::ELimit) 1,0);
   TGstripDistance->SetNumber(2.0);
   GFProperties->AddFrame(TGstripDistance, new TGXYLayoutHints(14,4.5,9,1.8));

   // signal threshold selection
   TGsignalThreshold = new TGNumberEntry(GFProperties,(int)0,5,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 0,(TGNumberFormat::ELimit) 1,0);
   TGsignalThreshold->SetNumber(5);
   GFProperties->AddFrame(TGsignalThreshold, new TGXYLayoutHints(14,7,9,1.8));

   // histogram bin scaling factor selection
   TGbinScaling = new TGNumberEntry(GFProperties,(int)0,5,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 0,(TGNumberFormat::ELimit) 1,0);
   TGbinScaling->SetNumber(20);
   GFProperties->AddFrame(TGbinScaling, new TGXYLayoutHints(14,9.5,9,1.8));

   // grid channel
   TGgridChannel = new TGNumberEntry(GFProperties,(int)0,5,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 0,(TGNumberFormat::ELimit) 3,0,63);
   TGgridChannel->SetNumber(63);
   GFProperties->AddFrame(TGgridChannel, new TGXYLayoutHints(14,12,9,1.8));

   // mapping selection //ttttt
   fChMap = new TGComboBox(GFProperties,CB_CHMAP);
   fChMap->AddEntry("mini_TPC",1);
   fChMap->AddEntry("EXO100_Coll",2);
   GFProperties->AddFrame(fChMap, new TGXYLayoutHints(14,14.5,15,1.8));
   fChMap->Select(1);
   fChMap->Associate(this);
 
   // gas selection
   fCBGas = new TGComboBox(GFProperties,CB_GAS);
   fCBGas->AddEntry("P10",0);
   fCBGas->AddEntry("Xe(98)CF4(2)",1);
   fCBGas->AddEntry("LCF4",2);
   fCBGas->AddEntry("LXe",3);
   GFProperties->AddFrame(fCBGas, new TGXYLayoutHints(14,17,15,1.8));
   fCBGas->Select(1);
   fCBGas->Associate(this);

   // use values from database
   fCheckDBValues = new TGCheckButton(GFProperties);
   fCheckDBValues->SetTextJustify(kTextRight);
   GFProperties->AddFrame(fCheckDBValues, new TGXYLayoutHints(27,19.5,2,1.8));
   fCheckDBValues->Connect("Clicked()","display",this,"ChangeUseDB()");
   fCheckDBValues->SetState(kButtonDisabled);

   // property labels
   TGLabel *dVLabel = new TGLabel(GFProperties,"Drift velocity:");
   dVLabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(dVLabel, new TGXYLayoutHints(1,2,12,1.8));

   TGLabel *sDLabel = new TGLabel(GFProperties,"Strip distance:");
   sDLabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(sDLabel, new TGXYLayoutHints(1,4.5,12,1.8));

   TGLabel *sTLabel = new TGLabel(GFProperties,"Threshold:");
   sTLabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(sTLabel, new TGXYLayoutHints(1,7,12,1.8));

   TGLabel *bSLabel = new TGLabel(GFProperties,"Bin scaling:");
   bSLabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(bSLabel, new TGXYLayoutHints(1,9.5,12,1.8));

   TGLabel *gCHLabel = new TGLabel(GFProperties,"Grid channel:");
   gCHLabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(gCHLabel, new TGXYLayoutHints(1,12,12,1.8));

   TGLabel *cMLabel = new TGLabel(GFProperties,"Channel map:"); //ttttt
   cMLabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(cMLabel, new TGXYLayoutHints(1,14.5,12,1.8));

   TGLabel *gasLabel = new TGLabel(GFProperties,"Gas mixture:");
   gasLabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(gasLabel, new TGXYLayoutHints(1,17,12,1.8));

   TGLabel *dbVLabel = new TGLabel(GFProperties,"Use DB values:");
   dbVLabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(dbVLabel, new TGXYLayoutHints(1,19.5,14,1.8));

   // property unit labels
   TGLabel *dVULabel = new TGLabel(GFProperties,"[cm/us]");
   dVULabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(dVULabel, new TGXYLayoutHints(24,2,6,1.8));

   TGLabel *sDULabel = new TGLabel(GFProperties,"[mm]");
   sDULabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(sDULabel, new TGXYLayoutHints(24,4.5,6,1.8));

   TGLabel *sTULabel = new TGLabel(GFProperties,"[sig]");
   sTULabel->SetTextJustify(kTextLeft);
   GFProperties->AddFrame(sTULabel, new TGXYLayoutHints(24,7,6,1.8));

   // create group frame "Tabs/Graphs"
   TGGroupFrame *GFTabs = new TGGroupFrame(fCompositeFrameSettings,"Tabs/Graphs");
   GFTabs->SetLayoutManager(new TGXYLayout(GFTabs)); // set XY layout to set size of group frame
   GFTabs->MoveResize(0,0,220,140);

   // max tab selection
   TGMaxTab = new TGNumberEntry(GFTabs,(int)0,5,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 0, (TGNumberFormat::ELimit) 3,0,100);
   TGMaxTab->SetNumber(10);
   GFTabs->AddFrame(TGMaxTab, new TGXYLayoutHints(22,2,7,1.8));

   fCheckNewtab = new TGCheckButton(GFTabs);
   fCheckNewtab->SetTextJustify(kTextRight);
   GFTabs->AddFrame(fCheckNewtab, new TGXYLayoutHints(27,4.2,2,1.8));

   fCheckSameRange = new TGCheckButton(GFTabs);
   fCheckSameRange->SetTextJustify(kTextRight);
   GFTabs->AddFrame(fCheckSameRange, new TGXYLayoutHints(27,6.4,2,1.8));

   fCheckIntegratedSignals = new TGCheckButton(GFTabs);
   fCheckIntegratedSignals->SetTextJustify(kTextRight);
   GFTabs->AddFrame(fCheckIntegratedSignals, new TGXYLayoutHints(27,8.6,2,1.8));
   fCheckIntegratedSignals->Connect("Clicked()","display",this,"ChangeSameRange()");

   // tabs labels
   TGLabel *mTLabel = new TGLabel(GFTabs,"Max. tabs:");
   mTLabel->SetTextJustify(kTextLeft);
   GFTabs->AddFrame(mTLabel, new TGXYLayoutHints(1,2,12,1.8));

   TGLabel *nTLabel = new TGLabel(GFTabs,"New tab for every event:");
   nTLabel->SetTextJustify(kTextLeft);
   GFTabs->AddFrame(nTLabel, new TGXYLayoutHints(1,4.2,20,1.8));

   TGLabel *sRLabel = new TGLabel(GFTabs,"Same range for all graphs:");
   sRLabel->SetTextJustify(kTextLeft);
   GFTabs->AddFrame(sRLabel, new TGXYLayoutHints(1,6.4,22,1.8));

   TGLabel *iSLabel = new TGLabel(GFTabs,"Plot integrated signals:");
   iSLabel->SetTextJustify(kTextLeft);
   GFTabs->AddFrame(iSLabel, new TGXYLayoutHints(1,8.6,22,1.8));

   // create group frame "2D Projections"
   TGGroupFrame *GF2D = new TGGroupFrame(fCompositeFrameSettings,"2D Projections");
   GF2D->SetLayoutManager(new TGXYLayout(GF2D)); // set XY layout to set size of group frame
   GF2D->MoveResize(0,0,220,200);

   fCheckConvert = new TGCheckButton(GF2D);
   fCheckConvert->SetTextJustify(kTextRight);
   fCheckConvert->SetState(kButtonDown);
   GF2D->AddFrame(fCheckConvert, new TGXYLayoutHints(27,2,2,1.8));

   fCheckXt = new TGCheckButton(GF2D);
   fCheckXt->SetTextJustify(kTextRight);
   fCheckXt->SetState(kButtonDown);
   GF2D->AddFrame(fCheckXt, new TGXYLayoutHints(27,4.2,2,1.8));

   fCheckYt = new TGCheckButton(GF2D);
   fCheckYt->SetTextJustify(kTextRight);
   fCheckYt->SetState(kButtonDown);
   GF2D->AddFrame(fCheckYt, new TGXYLayoutHints(27,6.4,2,1.8));

   fCheckGridt = new TGCheckButton(GF2D);
   fCheckGridt->SetTextJustify(kTextRight);
   fCheckGridt->SetState(kButtonDown);
   GF2D->AddFrame(fCheckGridt, new TGXYLayoutHints(27,8.6,2,1.8));

   fCheckManColorScale = new TGCheckButton(GF2D);
   fCheckManColorScale->SetTextJustify(kTextRight);
   fCheckManColorScale->SetState(kButtonUp);
   fCheckManColorScale->Connect("Clicked()","display",this,"ChangeMaxColorRange()");
   GF2D->AddFrame(fCheckManColorScale, new TGXYLayoutHints(27,10.8,2,1.8));

   TGmaxColorRange = new TGNumberEntryField(GF2D);
   TGmaxColorRange->SetState(kFALSE);
   TGmaxColorRange->SetNumber(2);
   GF2D->AddFrame(TGmaxColorRange, new TGXYLayoutHints(20,13,6,1.8));

   // 2D Projections labels
   TGLabel *convLabel = new TGLabel(GF2D,"Convert z-axis:");
   convLabel->SetTextJustify(kTextLeft);
   GF2D->AddFrame(convLabel, new TGXYLayoutHints(1,2,15,1.8));

   TGLabel *xtLabel = new TGLabel(GF2D,"x-z projection:");
   xtLabel->SetTextJustify(kTextLeft);
   GF2D->AddFrame(xtLabel, new TGXYLayoutHints(1,4.2,15,1.8));

   TGLabel *ytLabel = new TGLabel(GF2D,"y-z projection:");
   ytLabel->SetTextJustify(kTextLeft);
   GF2D->AddFrame(ytLabel, new TGXYLayoutHints(1,6.4,15,1.8));

   TGLabel *gridtLabel = new TGLabel(GF2D,"grid-z projection:");
   gridtLabel->SetTextJustify(kTextLeft);
   GF2D->AddFrame(gridtLabel, new TGXYLayoutHints(1,8.6,15,1.8));

   TGLabel *manColorScaleLabel = new TGLabel(GF2D,"Fixed color range:");
   manColorScaleLabel->SetTextJustify(kTextLeft);
   GF2D->AddFrame(manColorScaleLabel, new TGXYLayoutHints(1,10.8,18,1.8));

   TGLabel *maxColorRangeLabel = new TGLabel(GF2D,"max:");
   maxColorRangeLabel->SetTextJustify(kTextLeft);
   GF2D->AddFrame(maxColorRangeLabel, new TGXYLayoutHints(15,13,5,1.8));

   TGLabel *maxColorRangeUnit = new TGLabel(GF2D,"fC");
   maxColorRangeUnit->SetTextJustify(kTextLeft);
   GF2D->AddFrame(maxColorRangeUnit, new TGXYLayoutHints(27,13,2,1.8));

   // add group frames to the tab
   fCompositeFrameSettings->AddFrame(GFProperties, new TGLayoutHints(kLHintsLeft,5,5,5,5));
   fCompositeFrameSettings->AddFrame(GFTabs, new TGLayoutHints(kLHintsLeft,5,5,5,5));
   fCompositeFrameSettings->AddFrame(GF2D, new TGLayoutHints(kLHintsLeft,5,5,5,5));
// ************************************************************************************************

// **** CONTENT OF FILTERING TAB ******************************************************************
   // create group frame "On/Off"
   TGGroupFrame *GFOnOff = new TGGroupFrame(fCompositeFrameFiltering,"On/Off");
   GFOnOff->SetLayoutManager(new TGXYLayout(GFOnOff)); // set XY layout to set size of group frame
   GFOnOff->MoveResize(0,0,220,120);

   fCheckFiltering = new TGCheckButton(GFOnOff);
   fCheckFiltering->SetTextJustify(kTextRight);
   GFOnOff->AddFrame(fCheckFiltering, new TGXYLayoutHints(27,2,2,1.8));
   fCheckFiltering->Connect("Clicked()","display",this,"ChangeFiltering()");

   fCheckDispSpec = new TGCheckButton(GFOnOff);
   fCheckDispSpec->SetTextJustify(kTextRight);
   GFOnOff->AddFrame(fCheckDispSpec, new TGXYLayoutHints(27,4.2,2,1.8));

   fCheckShowFilter = new TGCheckButton(GFOnOff);
   fCheckShowFilter->SetTextJustify(kTextRight);
   GFOnOff->AddFrame(fCheckShowFilter, new TGXYLayoutHints(27,6.4,2,1.8));
   fCheckShowFilter->SetState(kButtonDisabled);

   TGLabel *fONLabel = new TGLabel(GFOnOff,"Turn filtering on:");
   fONLabel->SetTextJustify(kTextLeft);
   GFOnOff->AddFrame(fONLabel, new TGXYLayoutHints(1,2,20,1.8));

   TGLabel *fDispSpecLabel = new TGLabel(GFOnOff,"Display spectra:");
   fDispSpecLabel->SetTextJustify(kTextLeft);
   GFOnOff->AddFrame(fDispSpecLabel, new TGXYLayoutHints(1,4.2,20,1.8));

   TGLabel *fShowFilterLabel = new TGLabel(GFOnOff,"Show filter shape:");
   fShowFilterLabel->SetTextJustify(kTextLeft);
   GFOnOff->AddFrame(fShowFilterLabel, new TGXYLayoutHints(1,6.4,20,1.8));

   // create group frame "Filter Settings"
   TGGroupFrame *GFFilterSettings = new TGGroupFrame(fCompositeFrameFiltering,"Filter Settings");
   GFFilterSettings->SetLayoutManager(new TGXYLayout(GFFilterSettings)); // set XY layout to set size of group frame
   GFFilterSettings->MoveResize(0,0,220,160);

   // filter type selection
   fCBFilterType = new TGComboBox(GFFilterSettings,CB_FILTERTYPE);
   fCBFilterType->AddEntry("Bandpass",0);
   fCBFilterType->AddEntry("Bandstop",1);
   GFFilterSettings->AddFrame(fCBFilterType, new TGXYLayoutHints(14,2,15,1.8));
   fCBFilterType->Select(0);

   // cutoff frequency 1
   TGcutoff1 = new TGNumberEntry(GFFilterSettings,(int)0,5,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 0,(TGNumberFormat::ELimit) 3,0.1,1000);
   TGcutoff1->SetNumber(0.1);
   GFFilterSettings->AddFrame(TGcutoff1, new TGXYLayoutHints(14,4.5,9,1.8));

   // f1 unit selection
   fCBf1Unit = new TGComboBox( GFFilterSettings,CB_F1UNIT);
   fCBf1Unit->AddEntry("Hz",0);
   fCBf1Unit->AddEntry("kHz",1);
   fCBf1Unit->AddEntry("MHz",2);
   GFFilterSettings->AddFrame(fCBf1Unit, new TGXYLayoutHints(23.5,4.5,7,1.8));
   fCBf1Unit->Select(2);

   // cutoff frequency 2
   TGcutoff2 = new TGNumberEntry(GFFilterSettings,(int)0,5,-1,(TGNumberFormat::EStyle) 1,(TGNumberFormat::EAttribute) 0,(TGNumberFormat::ELimit) 3,0.1,1000);
   TGcutoff2->SetNumber(0.1);
   GFFilterSettings->AddFrame(TGcutoff2, new TGXYLayoutHints(14,7,9,1.8));

   // f2 unit selection
   fCBf2Unit = new TGComboBox( GFFilterSettings,CB_F1UNIT);
   fCBf2Unit->AddEntry("Hz",0);
   fCBf2Unit->AddEntry("kHz",1);
   fCBf2Unit->AddEntry("MHz",2);
   GFFilterSettings->AddFrame(fCBf2Unit, new TGXYLayoutHints(23.5,7,7,1.8));
   fCBf2Unit->Select(2);

   // Butterworth filter order
   TGFilterOrder = new TGNumberEntry(GFFilterSettings,(int)0,5,-1,(TGNumberFormat::EStyle) 0,(TGNumberFormat::EAttribute) 0,(TGNumberFormat::ELimit) 3,1,15);
   TGFilterOrder->SetNumber(10);
   GFFilterSettings->AddFrame(TGFilterOrder, new TGXYLayoutHints(14,9.5,9,1.8));

   // filter settings labels
   TGLabel *fFilterTypeLabel = new TGLabel(GFFilterSettings,"Filter type:");
   fFilterTypeLabel->SetTextJustify(kTextLeft);
   GFFilterSettings->AddFrame(fFilterTypeLabel, new TGXYLayoutHints(1,2,10,1.8));

   TGLabel *fCutOff1Label = new TGLabel(GFFilterSettings,"Cut off f1:");
   fCutOff1Label->SetTextJustify(kTextLeft);
   GFFilterSettings->AddFrame(fCutOff1Label, new TGXYLayoutHints(1,4.5,10,1.8));

   TGLabel *fCutOff2Label = new TGLabel(GFFilterSettings,"Cut off f2:");
   fCutOff2Label->SetTextJustify(kTextLeft);
   GFFilterSettings->AddFrame(fCutOff2Label, new TGXYLayoutHints(1,7,10,1.8));

   TGLabel *fFilterOrderLabel = new TGLabel(GFFilterSettings,"Filter order:");
   fFilterOrderLabel->SetTextJustify(kTextLeft);
   GFFilterSettings->AddFrame(fFilterOrderLabel, new TGXYLayoutHints(1,9.5,10,1.8));

   // add group frames to the tab
   fCompositeFrameFiltering->AddFrame(GFOnOff, new TGLayoutHints(kLHintsLeft,5,5,5,5));
   fCompositeFrameFiltering->AddFrame(GFFilterSettings, new TGLayoutHints(kLHintsLeft,5,5,5,5));
// ************************************************************************************************

// **** STATUS BAR 2 ******************************************************************************
   int nrStatParts2 = 5;
   int statPartsSize2[] = {52, 12, 12, 12, 12};
   stat2 = new TGStatusBar(this,w,30);
   stat2->SetParts(statPartsSize2, nrStatParts2);
// ************************************************************************************************

// **** STATUS BAR 1 ******************************************************************************
   int nrStatParts = 5;
   int statPartsSize[] = {40, 15, 15, 15, 15};
   stat = new TGStatusBar(this,w,30);
   stat->SetParts(statPartsSize, nrStatParts);
   stat->AddText("No run loaded",0);
// ************************************************************************************************

// **** ADD WIDGETS TO MAINFRAIM ******************************************************************
   AddFrame(fMenuBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
   AddFrame(tb, new TGLayoutHints(kLHintsTop | kLHintsExpandX,4,4,0,0));
   AddFrame(lh, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,2,2));
   AddFrame(hFrameTabs, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,2,2,2,2));
   AddFrame(stat2, new TGLayoutHints(kLHintsExpandX));
   AddFrame(stat, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));
// ************************************************************************************************

   MapSubwindows();

   Layout();

   SetWindowName(version);
   SetIconName(version);

   MapWindow();

   // create new Run_t opject
   run = 0;
   run = new Run_t();
  
   run->UseCalibDB(true); // use calibration values stored in the database
}

display::~display()
{

}

void display::Init()
{
   gStyle->SetOptStat(0);

   const Int_t NRGBs = 6;
   const Int_t NCont = 200;

   Double_t stops[] = { 0.00, 0.20, 0.40, 0.60, 0.80, 1.00};

   Double_t red[]   = { 0.57, 0.00, 0.00, 0.00, 1.00, 1.00};
   Double_t green[] = { 0.00, 0.00, 1.00, 1.00, 1.00, 0.00};
   Double_t blue[]  = { 0.76, 1.00, 1.00, 0.00, 0.00, 0.00};

   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);

   // create 2D histograms
   h21 = new TH2D();
   h21->SetName("h21");
   h21->SetTitle("x-z projection");

   h22 = new TH2D();
   h22->SetName("h22");
   h22->SetTitle("y-z projection");

   // set histogram properties
   h21->GetXaxis()->SetTitle("z-axis [cm]");
   h21->GetYaxis()->SetTitle("x-axis [cm]");
   //h21->GetZaxis()->SetTitle("charge [fC]");
   h21->GetYaxis()->SetTitleOffset(0.6);
   //h21->GetZaxis()->SetTitleOffset(0.6);

   h22->GetXaxis()->SetTitle("z-axis [cm]");
   h22->GetYaxis()->SetTitle("y-axis [cm]");
   //h22->GetZaxis()->SetTitle("charge [fC]");
   h22->GetYaxis()->SetTitleOffset(0.6);
   //h22->GetZaxis()->SetTitleOffset(0.6);
}

void display::CheckUpAll()
{
   int itemp;

   if (fCheck[9]->GetState() == kButtonUp) {
      for (int i = 0; i < ngroup; i++) {
         itemp = run->GetGroupID(i);
         fCheck[itemp]->SetState(kButtonUp);
      }
      fCheck[8]->SetState(kButtonUp);
   }
   else {
      if (fCheck[9]->GetState() == kButtonDown) {
         for (int i = 0; i < ngroup; i++) {
            itemp = run->GetGroupID(i);
            fCheck[itemp]->SetState(kButtonDown);
         }
         fCheck[8]->SetState(kButtonDown);
      }
   }
}

void display::DisableAll()
{
   if (fCheckSingleCH->GetState() == kButtonUp) {
      for (int i=0; i<10; i++) {
         fCheck[i]->SetState(kButtonUp);
      }
   }
   else {
      for (int i=0; i<10; i++) {
         fCheck[i]->SetState(kButtonDisabled);
      }
   }
}

void display::ChangeSameRange()
{
   if (fCheckIntegratedSignals->GetState() == kButtonDown) {
      fCheckSameRange->SetState(kButtonDisabled);
      if (fCheckFiltering->GetState() == kButtonDown) {
         fCheckDispSpec->SetState(kButtonDisabled);
         fCheckShowFilter->SetState(kButtonDisabled);
      }
   }
   else {
      fCheckSameRange->SetState(kButtonUp);
      if (fCheckFiltering->GetState() == kButtonDown) {
         fCheckDispSpec->SetState(kButtonUp);
         fCheckShowFilter->SetState(kButtonUp);
      }
   }
}

void display::ChangeFiltering()
{
   if (fCheckFiltering->GetState() == kButtonDown && fCheckIntegratedSignals->GetState() == kButtonUp) {
      fCheckShowFilter->SetState(kButtonUp);
   }
   else {
      fCheckShowFilter->SetState(kButtonDisabled);
   }
}

void display::ChangeUseDB()
{
   if (fCheckDBValues->GetState() == kButtonDown) {
      int gasID = 0;
      switch (fCBGas->GetSelected()) {
         case 0:
            gasID = 0;
            break;
         case 1:
            gasID = 1;
            break;
	case 2:
            gasID = 2;
            break;
	case 3:
            gasID = 3;
            break;
      }
      // get drift velocity for curent run
      SQLdriftVelocity = run->GetDriftVelocity(gasID);
      TGdriftVelocity->SetNumber(SQLdriftVelocity);
      
     }
}

void display::ChangeChMap()
{
    int mappingID;
    int DchMap;
    switch (fChMap->GetSelected()){
	case 1:
 	  DchMap = 1;
	  break;
	case 2:
	  DchMap = 2;
	  break;
       }

   chMapping = run->GetChannelMapping(&mappingID, DchMap);
   
   char statMappingID[50];
   sprintf(statMappingID, "Using channel mapping: %i", mappingID);
   stat2->AddText(statMappingID,0);
}

void display::ChangeMaxColorRange()
{
   if (fCheckManColorScale->GetState() == kButtonDown) {
      TGmaxColorRange->SetState(kTRUE);
   }
   else {
      TGmaxColorRange->SetState(kFALSE);
   }
}

void display::OpenFile()
{
   string fName;
   TGFileInfo fi;
   fi.fIniDir = StrDup(".");
   const char * filetypes[] = { "Root file", "*.root", "Midas file", "*.mid", "All files", "*", 0, 0};
   fi.fFileTypes = filetypes;
   new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, &fi);

   if(fi.fFilename == NULL) {
      stat->AddText("No run loaded",0);
      fButtonDraw->SetState(kButtonDisabled);
      for (int i = 0; i < 10; i++) {
         fCheck[i]->SetState(kButtonDisabled);
      }
      fCheckSingleCH->SetState(kButtonDisabled);
      fButtonDraw->SetState(kButtonDisabled);
      fCheckDBValues->SetState(kButtonDisabled);
      return;
   }

   // convert MIDAS file
   if(GetFileExt(fi.fFilename) == 2) {
      char cmd[256];
      sprintf(cmd,"mid2root %s", fi.fFilename);
      gSystem->Exec(cmd);

      string fNameTmp = fi.fFilename;
      int limit = fNameTmp.find(".mid");
      fName = fNameTmp.substr(0,limit);
      fName += ".root";

      run->OpenRun(fName.c_str());
      stat->AddText(fName.c_str(),0);
   }
   else {
      fName = fi.fFilename;
      run->OpenRun(fi.fFilename);
      stat->AddText(fi.fFilename,0);
   }

   // load first event
   run->SetCurrentData((unsigned int)0, (unsigned int)0);

   // retreive run information
   nevent = run->GetNumberOfEvents();
   nchannel = run->GetNumberOfChannels();
   nsamples = run->GetNumberOfSamples();
   ngroup = nchannel / 8;
   duration = run->GetRunDuration();
   date = time_t(run->GetLowPrecisionEventTime());

   // set limits in event number entry
   fEventID->SetLimitValues(0, nevent - 1);

   // convert unix time
   struct tm* runDate = gmtime(&date);
   char runDateH[80];
   strftime(runDateH,80,"Run date: %Y-%m-%d",runDate);

   char statEvent[50];
   char statDuration[50];
   sprintf(statEvent, "Event: 0/%i", nevent);
   sprintf(statDuration, "Duration: %.0f s", duration);

   // set status information
   stat->AddText(statEvent,1);
   stat->AddText(statDuration,2);
   stat->AddText("",3);
   stat->AddText(runDateH,4);

   // enable group checkboxes
   int itemp;
   for (int i = 0; i < ngroup; i++) {
      itemp = run->GetGroupID(i);
      fCheck[itemp]->SetState(kButtonUp);
   }
   fCheck[8]->SetState(kButtonUp);
   fCheck[9]->SetState(kButtonUp);
   fCheckSingleCH->SetState(kButtonUp);
   fButtonDraw->SetState(kButtonUp);
   fCheckDBValues->SetState(kButtonUp);

   int cutAt1 = fName.find_last_of("/");
   int cutAt2 = fName.find(".");
   currentRunName = fName.substr(cutAt1+1, cutAt2 - cutAt1 - 1);


   // get channel mapping
   int mappingID;
   int DchMap;
       switch (fChMap->GetSelected()){
	case 1:
		DchMap = 1;
		break;
	case 2:
		DchMap = 2;
		break;
       }
 
   chMapping = run->GetChannelMapping(&mappingID, DchMap);
   char statMappingID[50];
   sprintf(statMappingID, "Using channel mapping: %i", mappingID);
   stat2->AddText(statMappingID,0);

   //get corresponding drift velocity for gas ID
   int gasID = 0;
      switch (fCBGas->GetSelected()) {
         case 0:
            gasID = 0;
            break;
         case 1:
            gasID = 1;
            break;
	case 2:
            gasID = 2;
            break;
	case 3:
            gasID = 3;
            break;
      }
      // get drift velocity for curent run
      SQLdriftVelocity = run->GetDriftVelocity(gasID);
      TGdriftVelocity->SetNumber(SQLdriftVelocity);

   // calculate drift velocity
   ChangeUseDB();
}

void display::CloseTab()
{
   int currentTab = fTabPlots->GetCurrent();
   fTabPlots->RemoveTab(currentTab);

   for (int i = currentTab; i < 99; i++) {fEcanvasMulti[i] = fEcanvasMulti[i+1];}

   int nTabs = fTabPlots->GetNumberOfTabs();
   if (nTabs == 0) {fButtonClear->SetState(kButtonDisabled);}
}

void display::CloseAllTabs()
{
   int nTabs = fTabPlots->GetNumberOfTabs();
   for (int i = 0; i < nTabs; i++) {
      fTabPlots->RemoveTab(0);
   }
   fButtonClear->SetState(kButtonDisabled);

   stat2->AddText("",1);
   stat2->AddText("",2);
   stat2->AddText("",3);
   stat2->AddText("",4);
}

void display::Draw()
{
   char title[50];
   int ch = 0, gp = 0, gp_old = -1, currentTab = 0, j = 0, oldTab = 0;
   double min, max;

   // variables for total charge on X- and Y-Stripes, sum of both and grid
   double totalChargeX = 0;
   double totalChargeY = 0;
   double totalChargeGrid = 0;
   double totalCharge = 0;

   int chStartX = 0;
   int chStartY = 30;
   int chEndX = 30;
   int chEndY = 61;

   // get selected properties
   double driftVelocity; // drift velocity in cm/us
   if (fCheckDBValues->GetState() == kButtonDown) {driftVelocity = SQLdriftVelocity;}
   else {driftVelocity = TGdriftVelocity->GetNumber();}
   double stripDistance = TGstripDistance->GetNumber() * 0.1; // distance between readout strips in cm
   double sigmaThreshold = TGsignalThreshold->GetNumber(); // signal threshold for noise estimation
   int binScalingFactor = int(TGbinScaling->GetNumber()); // bin scaling factor for noise estimation
   int gridCH = int(TGgridChannel->GetNumber()); // grid channel

   // filter frequency units
   double f1Unit = 0.0;
   double f2Unit = 0.0;
   switch (fCBf1Unit->GetSelected()) {
      case 0:
         f1Unit = 1.0;
         break;
      case 1:
         f1Unit = pow(10.0,3);
         break;
      case 2:
          f1Unit = pow(10.0,6);
          break;
   }

   switch (fCBf2Unit->GetSelected()) {
      case 0:
         f2Unit = 1.0;
         break;
      case 1:
         f2Unit = pow(10.0,3);
         break;
      case 2:
          f2Unit = pow(10.0,6);
          break;
   }

   int eventID = int(fEventID->GetNumber());
   int ntabs = fTabPlots->GetNumberOfTabs();

   TCanvas *c = NULL;

   // get handle on data
   double * timeAxis = run->GetTimeScale();
   int * rawAxis = run->GetRawData();
   double * scaledAxis = run->GetScaledData();
   double * calibratedAxis = run->GetCalibratedData();
   double * frequencySpectrum = run->GetFrequencySpectrum();
   double * filterShape = run->GetFilterShape();

   // create frequency axis
   double fs = run->GetSamplingFrequency();
   fs = fs*pow(10.0,6); // sampling frequency is in MHz
   int nFFT = int(ceil(log(nsamples)/log(2))); // get next power of 2
   int nrFFT = int(pow(2.0, nFFT)); // number of FFT samples

   double *f = new double[nrFFT/2];
   double stp = 2. / nrFFT;
   for (int i = 0; i < nrFFT/2; i++) {
      f[i] = fs/2*i*stp;
   }

   // create integrated time and convert into cm using the drift velocity
   int nsamplesTMP = nsamples - 1;
   double *intTime = new double[nsamplesTMP];

   if (fCheckConvert->GetState() == kButtonDown) {
      for (int i = 0; i < nsamplesTMP; i++) {
         intTime[i] = driftVelocity*(timeAxis[i] + timeAxis[i+1])/2;
      }
   }
   else {
      for (int i = 0; i < nsamplesTMP; i++) {
         intTime[i] = (timeAxis[i] + timeAxis[i+1])/2;
      }
   }

   // draw single channel
   if (fCheckSingleCH->GetState() == kButtonDown) {
      int singleCH = int(TGSingleCH->GetNumber());
      run->SetCurrentData(eventID, singleCH);
      sprintf(title, "Evt%i_CH%i", eventID, singleCH);
      if (!(ntabs > currentTab && fCheckNewtab->GetState() == kButtonUp)) {
         CreateCanvas(string(title));
         currentTab = fTabPlots->GetCurrent();
         oldTab = 0;
      }
      else {
         fTabPlots->SetTab(currentTab);
         fTabPlots->SetText(title);
         oldTab = 1;
      }
      c = fEcanvasMulti[currentTab]->GetCanvas();
      c->Clear();

      TGraph * graph = NULL;
      TGraph * graphFilter = NULL;

      // get frequency spectrum
      run->GetSpectrum();

      // apply frequency filter
      if (fCheckFiltering->GetState() == kButtonDown) {
         run->ApplyFilter(fCBFilterType->GetSelected(), (TGcutoff1->GetNumber())*f1Unit, (TGcutoff2->GetNumber())*f2Unit, int(TGFilterOrder->GetNumber()));
      }

      if (fCheckIntegratedSignals->GetState() == kButtonDown) { // integrate signal and subtract noise
         run->GetNoiseSubtractedIntegratedSignal(&totalCharge, binScalingFactor, sigmaThreshold, gridCH);
         graph = new TGraph(nsamplesTMP, intTime, calibratedAxis); // plot integrated signal
         graph->GetYaxis()->SetTitle("charge [fC]");
         if (fCheckConvert->GetState() == kButtonDown) {graph->GetXaxis()->SetTitle("z-axis [cm]");}
         else {graph->GetXaxis()->SetTitle("Time [#mus]");}
      }
      else {
         if (fCheckDispSpec->GetState() == kButtonDown) {
            graph = new TGraph(nrFFT/2, f, frequencySpectrum); // plot the frequency spectrum
            if (fCheckShowFilter->GetState() == kButtonDown && fCheckFiltering->GetState() == kButtonDown) {
               graphFilter = new TGraph(nrFFT/2, f, filterShape); // plot filter shape
               graphFilter->SetLineColor(kRed);
               graphFilter->SetLineStyle(7);
            }
            graph->GetXaxis()->SetTitle("frequency [Hz]");
            graph->GetYaxis()->SetTitle("Magnitude");
         }
         else {
            switch (fCBUnit->GetSelected()) {
               case 0:
                  graph = new TGraph(nsamples, timeAxis, calibratedAxis); // plot calibrated data
                  graph->GetYaxis()->SetTitle("Amplitude [nA]");
                  break;
               case 1:
                  graph = new TGraph(nsamples, timeAxis, scaledAxis); // plot scaled data
                  graph->GetYaxis()->SetTitle("Amplitude [mV]");
                  break;
               case 2:
                  double * tempRawAxis = new double[nsamples];
                  for (int m = 0; m < nsamples; m++) {
                     tempRawAxis[m] = (double)rawAxis[m];
                  }
                  graph = new TGraph(nsamples, timeAxis, tempRawAxis); // plot raw data
                  graph->GetYaxis()->SetTitle("ADC");
                  delete [] tempRawAxis;
                  break;
            }
            graph->GetXaxis()->SetTitle("Time [#mus]");
         }
      }

      sprintf(title, "Channel %i, Event %i", singleCH, eventID);
      graph->SetTitle(title);
      graph->Draw("ALP");
      if (fCheckDispSpec->GetState() == kButtonDown && fCheckShowFilter->GetState() == kButtonDown) {graphFilter->Draw("L");}

      if (oldTab == 1) {c->Update();}
   }

   // get min max of all channels
   if (fCheckSameRange->GetState() == kButtonDown) {
      GetMinMaxData(&min, &max);
   }

   // draw groups
   for (int i = 0; i < nchannel; i++) {
      ch = run->GetChannelID(i); // get channel ID
      gp = ch / 8; // get corresponding group
      if (fCheck[gp]->GetState() == kButtonDown && gp != gp_old) { // create new tab for new group
         sprintf(title, "Evt%i_Gp%i", eventID, gp);
         if (!(ntabs > currentTab && fCheckNewtab->GetState() == kButtonUp)) {
            CreateCanvas(string(title));
            currentTab = fTabPlots->GetCurrent();
            oldTab = 0;
         }
         else {
            fTabPlots->SetTab(currentTab);
            fTabPlots->SetText(title);
            oldTab = 1;
         }
         c = fEcanvasMulti[currentTab]->GetCanvas();
         c->Clear();
         c->Divide(4,2);
         currentTab++;
         gp_old = gp;
         j = 1;
      }
      if (fCheck[gp]->GetState() == kButtonDown) {
         c->cd(j);
         run->SetCurrentData(eventID, ch);

         TGraph * graph = NULL;
         TGraph * graphFilter = NULL;

         // get frequency spectrum
         run->GetSpectrum();

         // apply frequency filter
         if (fCheckFiltering->GetState() == kButtonDown) {
            run->ApplyFilter(fCBFilterType->GetSelected(), (TGcutoff1->GetNumber())*f1Unit, (TGcutoff2->GetNumber())*f2Unit, int(TGFilterOrder->GetNumber()));
         }

         if (fCheckIntegratedSignals->GetState() == kButtonDown) { // integrate signal and subtract noise
            run->GetNoiseSubtractedIntegratedSignal(&totalCharge, binScalingFactor, sigmaThreshold, gridCH);
            graph = new TGraph(nsamplesTMP, intTime, calibratedAxis); // plot integrated signal
            graph->GetYaxis()->SetTitle("charge [fC]");
            if (fCheckConvert->GetState() == kButtonDown) {graph->GetXaxis()->SetTitle("z-axis [cm]");}
            else {graph->GetXaxis()->SetTitle("Time [#mus]");}
         }
         else {
            if (fCheckDispSpec->GetState() == kButtonDown) {
               graph = new TGraph(nrFFT/2, f, frequencySpectrum); // plot the frequency spectrum
               if (fCheckShowFilter->GetState() == kButtonDown && fCheckFiltering->GetState() == kButtonDown) {
                  graphFilter = new TGraph(nrFFT/2, f, filterShape); // plot filter shape
                  graphFilter->SetLineColor(kRed);
                  graphFilter->SetLineStyle(7);
               }
               graph->GetXaxis()->SetTitle("frequency [Hz]");
               graph->GetYaxis()->SetTitle("Magnitude");
            }
            else {
               switch (fCBUnit->GetSelected()) {
                  case 0:
                     graph = new TGraph(nsamples, timeAxis, calibratedAxis); // plot calibrated data
                     graph->GetYaxis()->SetTitle("Amplitude [nA]");
                     break;
                  case 1:
                     graph = new TGraph(nsamples, timeAxis, scaledAxis); // plot scaled data
                     graph->GetYaxis()->SetTitle("Amplitude [mV]");
                     break;
                  case 2:
                     double * tempRawAxis = new double[nsamples];
                     for (int m = 0; m < nsamples; m++) {
                        tempRawAxis[m] = (double)rawAxis[m];
                     }
                     graph = new TGraph(nsamples, timeAxis, tempRawAxis); // plot raw data
                     graph->GetYaxis()->SetTitle("ADC");
                     delete [] tempRawAxis;
                     break;
               }
               graph->GetXaxis()->SetTitle("Time [#mus]");
            }
         }

         sprintf(title, "Channel %i, Event %i", ch, eventID);
         graph->SetTitle(title);
         graph->Draw("ALP");
         if (fCheckDispSpec->GetState() == kButtonDown && fCheckShowFilter->GetState() == kButtonDown) {graphFilter->Draw("L");}
         if (fCheckSameRange->GetState() == kButtonDown) {graph->GetYaxis()->SetRangeUser(min, max);}
         if (oldTab == 1) {c->Update();}
         j++;
      }
   }

   // get number of 2D projections to display according to settings
   int nrCk = 0;
   if (fCheckXt->GetState() == kButtonDown) {nrCk++;}
   if (fCheckYt->GetState() == kButtonDown) {nrCk++;}
   if (fCheckGridt->GetState() == kButtonDown) {nrCk++;}

   // 2D projections
   if (fCheck[8]->GetState() == kButtonDown && nrCk != 0) {
      sprintf(title, "Evt%i_2D", eventID);
      if (!(ntabs > currentTab && fCheckNewtab->GetState() == kButtonUp)) {
         CreateCanvas(string(title));
         currentTab = fTabPlots->GetCurrent();
         oldTab = 0;
      }
      else {
         fTabPlots->SetTab(currentTab);
         fTabPlots->SetText(title);
         oldTab = 1;
      }
      c = fEcanvasMulti[currentTab]->GetCanvas();

      int n_channelsX = chEndX-chStartX;
      int n_channelsY = chEndY-chStartY;
      double *channelsX = new double[n_channelsX];
      double *channelsY = new double[n_channelsY];
      for (int i = 0; i < n_channelsX; i++) {
         channelsX[i] = stripDistance*i;
      }
      for (int i = 0; i < n_channelsY; i++) {
         channelsY[i] = stripDistance*i;
      }

      // clear 2D histograms
      h21->Reset();
      h22->Reset();
      h21->SetMaximum(-1111);
      h22->SetMaximum(-1111);

      // set 2D histograms
      h21->SetBins(nsamplesTMP, intTime[0], intTime[nsamplesTMP-1], n_channelsX, channelsX[0], channelsX[n_channelsX-1]);
      h22->SetBins(nsamplesTMP, intTime[0], intTime[nsamplesTMP-1], n_channelsY, channelsY[0], channelsY[n_channelsY-1]);

      for (int k=chStartX; k<chEndY; k++) {
         int chan = chMapping[k];
         run->SetCurrentData(eventID,chan);

         totalCharge = 0;
         if (fCheckFiltering->GetState() == kButtonDown) { // apply filter
            run->ApplyFilter(fCBFilterType->GetSelected(), (TGcutoff1->GetNumber())*f1Unit, (TGcutoff2->GetNumber())*f2Unit, int(TGFilterOrder->GetNumber()));
         }

         // get integrated data with noise subtracted
         run->GetNoiseSubtractedIntegratedSignal(&totalCharge, binScalingFactor, sigmaThreshold, gridCH);

         // compute total charge in X and Y
         if (k < chEndX && k != gridCH) {totalChargeX += totalCharge;}
         else {if (k != gridCH) {totalChargeY += totalCharge;}}

         // fill 2D histograms
         for (int i = 0; i < nsamplesTMP; i++) {
            if (k < chEndX && k != gridCH) {h21->SetBinContent(i+1,k-chStartX+1,-1 * calibratedAxis[i]);}
            else {if (k != gridCH) {h22->SetBinContent(i+1,k-chStartY+1,-1 * calibratedAxis[i]);}}
         }
      }

      if (fCheckConvert->GetState() == kButtonDown) {
         h21->GetXaxis()->SetTitle("z-axis [cm]");
         h22->GetXaxis()->SetTitle("z-axis [cm]");
      }
      else {
         h21->GetXaxis()->SetTitle("z-axis [#mus]");
         h22->GetXaxis()->SetTitle("z-axis [#mus]");
      }

      // load grid channel ...
      run->SetCurrentData(eventID, gridCH);

      if (fCheckFiltering->GetState() == kButtonDown) { // apply filter
         run->ApplyFilter(fCBFilterType->GetSelected(), (TGcutoff1->GetNumber())*f1Unit, (TGcutoff2->GetNumber())*f2Unit, int(TGFilterOrder->GetNumber()));
      }

      // get integrated data with noise subtracted
      run->GetNoiseSubtractedIntegratedSignal(&totalChargeGrid, binScalingFactor, sigmaThreshold, gridCH);

      TGraph * graph = NULL;
      graph = new TGraph(nsamplesTMP, intTime, calibratedAxis);
      //graph->SetTitle("Grid charge");
      graph->SetTitle("Grid signal");
      if (fCheckConvert->GetState() == kButtonDown) {graph->GetXaxis()->SetTitle("z-axis[cm]");}
      else {graph->GetXaxis()->SetTitle("z-axis[#mus]");}
      //graph->GetYaxis()->SetTitle("charge [fC]");
      graph->GetYaxis()->SetTitle("current [nA]");
      graph->GetYaxis()->SetTitleOffset(0.6);
      graph->GetXaxis()->SetRangeUser(intTime[0], intTime[nsamplesTMP-1]);

      if (fCheckManColorScale->GetState() == kButtonDown) { // Set manually entered maximum of histograms
         h21->SetMaximum(TGmaxColorRange->GetNumber());
         h22->SetMaximum(TGmaxColorRange->GetNumber());
      }
      else { // Set same range for both histogram to have the same color palette
         double h21Min = h21->GetMaximum();
         double h22Min = h22->GetMaximum();

         if (h21Min > h22Min) {h22->SetMaximum(h21Min);}
         else {h21->SetMaximum(h22Min);}
      }

      c->Clear();
      c->Divide(1,nrCk);

      int currentDev = 1;

      // plot x-z projection
      if (fCheckXt->GetState() == kButtonDown) {
         c->cd(currentDev);

         // z-axis title
         TText *zTitleX = new TText(0.99*(h21->GetXaxis()->GetXmax()), 1.02*(h21->GetYaxis()->GetXmax()), "neg. charge [fC]");
         zTitleX->SetTextSize(0.04);
         h21->Draw("colz");
         zTitleX->Draw();

         // create channel axis
         TGaxis *axis1 = new TGaxis(h21->GetXaxis()->GetXmax(),h21->GetYaxis()->GetXmin(),h21->GetXaxis()->GetXmax(),h21->GetYaxis()->GetXmax(),chStartX,chStartX+h21->GetYaxis()->GetNbins(),510,"+L");
         axis1->SetTitle("channel");
         axis1->SetTitleOffset(-1);
         axis1->SetLabelOffset(-0.045);
         axis1->Draw();

         // draw border line
         TLine *l1 = new TLine(h21->GetXaxis()->GetXmin(), h21->GetYaxis()->GetXmax(), h21->GetXaxis()->GetXmax(), h21->GetYaxis()->GetXmax());
         l1->Draw();

         // update status bar
         char statChargeX[50];
         sprintf(statChargeX, "Charge X: %.1f fC", totalChargeX);
         stat2->AddText(statChargeX,1);

         currentDev++;
      }
      else {stat2->AddText("",1);}

      // plot y-z projection
      if (fCheckYt->GetState() == kButtonDown) {
         c->cd(currentDev);

         // z-axis title
         TText *zTitleY = new TText(0.99*(h22->GetXaxis()->GetXmax()), 1.02*(h22->GetYaxis()->GetXmax()), "neg. charge [fC]");
         zTitleY->SetTextSize(0.04);
         h22->Draw("colz");
         zTitleY->Draw();

         // create channel axis
         TGaxis *axis2 = new TGaxis(h22->GetXaxis()->GetXmax(),h22->GetYaxis()->GetXmin(),h22->GetXaxis()->GetXmax(),h22->GetYaxis()->GetXmax(),chStartY,chStartY+h22->GetYaxis()->GetNbins(),510,"+L");
         axis2->SetTitle("channel");
         axis2->SetTitleOffset(-1);
         axis2->SetLabelOffset(-0.045);
         axis2->Draw();

         // draw border line
         TLine *l2 = new TLine(h22->GetXaxis()->GetXmin(), h22->GetYaxis()->GetXmax(), h22->GetXaxis()->GetXmax(), h22->GetYaxis()->GetXmax());
         l2->Draw();

         // update status bar
         char statChargeY[50];
         sprintf(statChargeY, "Charge Y: %.1f fC", totalChargeY);
         stat2->AddText(statChargeY,2);

         currentDev++;
      }
      else {stat2->AddText("",2);}

      if (fCheckXt->GetState() == kButtonDown && fCheckYt->GetState() == kButtonDown) {
         // update status bar
         char statChargeSum[50];
         sprintf(statChargeSum, "Sum: %.1f fC", totalChargeX + totalChargeY);
         stat2->AddText(statChargeSum,3);
      }
      else {stat2->AddText("",3);}

      // grid-z projection
      if (fCheckGridt->GetState() == kButtonDown) {
         c->cd(currentDev);
         graph->Draw("ALP");

         // update status bar
         char statChargeGrid[50];
         sprintf(statChargeGrid, "Charge Grid: %.1f fC", totalChargeGrid);
         stat2->AddText(statChargeGrid,4);
      }
      else {stat2->AddText("",4);}

      if (oldTab == 1) {c->Update();};
   }
   else {
      stat2->AddText("",1);
      stat2->AddText("",2);
      stat2->AddText("",3);
      stat2->AddText("",4);
   }

   fButtonClear->SetState(kButtonUp);

   // get trigger rate
   // get time of first trigger
   run->SetCurrentData(0, 0);
   double firstTrigger = run->GetLowPrecisionEventTime();

   // get time of current event
   run->SetCurrentData(eventID, 0);
   double currentTrigger = run->GetLowPrecisionEventTime();

   // update status bar
   char triggerRate[50];
   if (eventID > 0) {sprintf(triggerRate, "Trigger rate: %.3f Hz", (eventID + 1) / (currentTrigger - firstTrigger));}
   else {sprintf(triggerRate, "Trigger rate: 0 Hz");}
   stat->AddText(triggerRate,3);

   // update status bar
   char statEvent[50];
   sprintf(statEvent, "Event: %i/%i", eventID, nevent);
   stat->AddText(statEvent,1);
}

void display::GetPreviousEvent()
{
   int eventID = int(fEventID->GetNumber());
   if (eventID > 0) {
      eventID--;
      fEventID->SetNumber(eventID);
      Draw();
   }
}

void display::GetNextEvent()
{
   int eventID = int(fEventID->GetNumber());
   if (eventID < nevent - 1) {
      eventID++;
      fEventID->SetNumber(eventID);
      Draw();
   }
}

void display::CreateCanvas(string title)
{
   int tabID = fTabPlots->GetNumberOfTabs();
   int maxTabs = int(TGMaxTab->GetNumber());
   //char title[50];
   //sprintf(title, "Tab%i", tabID+1);

   if (tabID >= maxTabs) {return;}

   // create new tab container
   TGCompositeFrame *fCompositeFrame;
   fCompositeFrame = fTabPlots->AddTab(title.c_str());
   fCompositeFrame->SetLayoutManager(new TGVerticalLayout(fCompositeFrame));

   // create TCanvas
   fEcanvasMulti[tabID] = new TRootEmbeddedCanvas(title.c_str(),fCompositeFrame);
   //fEcanvasMulti[tabID] = new TRootEmbeddedCanvasDerived(title.c_str(),fCompositeFrame);
   fCompositeFrame->AddFrame(fEcanvasMulti[tabID], new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,2,2,2,2));

   fTabPlots->SetTab(tabID);

   MapSubwindows();
   Layout();

   return;
}

void display::SaveCanvas()
{
   TGFileInfo fi;
   fi.fIniDir = StrDup(".");
   const char * filetypes[] = { "eps", "*.eps", "gif", "*.gif", "png", "*.png", "ps", "*.ps", "root", "*root", "All files", "*", 0, 0};
   fi.fFileTypes = filetypes;
   new TGFileDialog(gClient->GetRoot(), 0, kFDSave, &fi);
   if (fi.fFilename) {
      string filename = fi.fFilename;
      const char *filetype = fi.fFileTypes[fi.fFileTypeIdx];
      size_t pos = filename.find_last_of(".");
      string ext =  filename.substr(pos + 1);
      int currentTab = fTabPlots->GetCurrent();
      TCanvas *c = fEcanvasMulti[currentTab]->GetCanvas();
      if (ext == filetype) {c->SaveAs(fi.fFilename);}
      else {
         char fname[50];
         sprintf(fname, "%s.%s", fi.fFilename, filetype);
         c->SaveAs(fname);
      }
   }
}

void display::PrintCanvas()
{
   char *installDir = getenv("READMIDASSYS");

   if (!installDir) {
      cout << "READ_MIDASSYS not set!" << endl;
      cout << "Printing aborted!" << endl;
      return;
   }

   char picDir[50];
   sprintf(picDir, "%s/icons/printer.xpm", installDir);

   // get timestamp
   time_t rawtime;
   struct tm * timeinfo;

   time ( &rawtime );
   timeinfo = localtime ( &rawtime );

   // generate filename
   strftime(printfName,80,"EventDisplay%Y%m%d%H%M%S.ps",timeinfo);

   PrintDialog = new TGMainFrame(gClient->GetRoot(), 400, 100);
   PrintDialog->SetLayoutManager(new TGXYLayout(PrintDialog));

   TGCompositeFrame *pic = new TGCompositeFrame(PrintDialog);
   pic->MoveResize(10,26,48,48);

   const TGPicture *ipic =(TGPicture *)gClient->GetPicture(picDir);
   TGIcon *icon = new TGIcon(pic,ipic,48,48);

   pic->AddFrame(icon);
   //icon->MoveResize(10,34,32,32);

   TGLabel *fLabelCommand = new TGLabel(PrintDialog,"Print command:");
   fLabelCommand->SetTextJustify(kTextLeft);
   fLabelCommand->MoveResize(95,29,100,10);

   TGLabel *fLabelPrinter = new TGLabel(PrintDialog,"Printer:");
   fLabelPrinter->SetTextJustify(kTextLeft);
   fLabelPrinter->MoveResize(95,61,100,10);

   TGCommand = new TGTextEntry(PrintDialog);
   TGCommand->MoveResize(200,23,110,22);

   TGPrinter = new TGTextEntry(PrintDialog);
   TGPrinter->MoveResize(200,55,110,22);

   TGTextButton *fButtonPrint = new TGTextButton(PrintDialog,"Print",PRINT_BUTTON);
   fButtonPrint->MoveResize(330,22,60,24);
   fButtonPrint->Associate(this);
   //fButtonPrint->Connect("Clicked()","display",this,"ExecPrint()");

   TGTextButton *fButtonCancel = new TGTextButton(PrintDialog,"Cancel");
   fButtonCancel->MoveResize(330,54,60,24);
   fButtonCancel->Connect("Clicked()","display",PrintDialog,"DeleteWindow()");

   PrintDialog->MoveResize(0,0,400,100);

   PrintDialog->AddFrame(pic);
   PrintDialog->AddFrame(fLabelCommand);
   PrintDialog->AddFrame(TGCommand);
   PrintDialog->AddFrame(TGPrinter);
   PrintDialog->AddFrame(fButtonPrint);

   PrintDialog->SetWindowName("Print");
   PrintDialog->MapSubwindows();
   PrintDialog->MapWindow();
}

void display::ExecPrint()
{
   char printCMD[100];
   char deleteCMD[100];
   string s1 = TGPrinter->GetText();
   string s2 = TGCommand->GetText();
   if (s2.size() > 0) {
      if (s1.size() == 0) {sprintf(printCMD, "%s %s", TGCommand->GetText(), printfName);}
      else {sprintf(printCMD, "%s -P %s %s", TGCommand->GetText(), TGPrinter->GetText(), printfName);}
      int currentTab = fTabPlots->GetCurrent();
      TCanvas *c = fEcanvasMulti[currentTab]->GetCanvas();
      c->Print(printfName, "psLandscape");
      system(printCMD);
      sprintf(deleteCMD, "rm %s", printfName);
      system(deleteCMD);
   }
}

void display::GetRunInfo()
{
   if (!run->RunLoaded()) {return;}

   // connect to SQL server
   TSQLServer *serv = TSQLServer::Connect("mysql://lheppc83.unibe.ch:3306", "exodaq", "exosql");

   // get first event
   run->SetCurrentData(0, 0);

   char valD[50];
   char valI[50];
   char valC[50];
   char runDateH[80];

   TCanvas *cInfo = new TCanvas("cInfo", "Run information", 500, 600);

   // create table
   TLine *t[7];
   t[0] = new TLine(0.01,0.99,0.99,0.99);
   t[1] = new TLine(0.01,0.95,0.99,0.95);
   t[2] = new TLine(0.01,0.01,0.99,0.01);
   t[3] = new TLine(0.01,0.01,0.01,0.99);
   t[4] = new TLine(0.3,0.01,0.3,0.99);
   t[5] = new TLine(0.6,0.01,0.6,0.99);
   t[6] = new TLine(0.99,0.01,0.99,0.99);

   for (int i = 0; i < 7; i++) {
      t[i]->Draw();
   }

   TText *q[21];
   TText *v[21];
   TText *c[21];

   q[0] = new TText(0.15, 0.97, "Quantity");
   v[0] = new TText(0.45, 0.97, "Value");
   c[0] = new TText(0.8, 0.97, "Command");

   // run date
   time_t date = time_t(run->GetLowPrecisionEventTime());
   // convert unix time
   struct tm* runDate = gmtime(&date);
   strftime(runDateH,80,"%Y-%m-%d %H:%M:%S",runDate);
   q[1] = new TText(0.02, 0.92, "Run date");
   v[1] = new TText(0.31, 0.92, runDateH);
   c[1] = new TText(0.61, 0.92, "GetLowPrecisionEventTime()");

   // run duration
   sprintf(valD,"%.1f s", run->GetRunDuration());
   q[2] = new TText(0.02, 0.88, "Run duration");
   v[2] = new TText(0.31, 0.88, valD);
   c[2] = new TText(0.61, 0.88, "GetRunDuration()");

   // number of events
   sprintf(valI,"%i", run->GetNumberOfEvents());
   q[3] = new TText(0.02, 0.84, "Nb of events");
   v[3] = new TText(0.31, 0.84, valI);
   c[3] = new TText(0.61, 0.84, "GetNumberOfEvents()");

   // number of groups
   sprintf(valI,"%i", run->GetNumberOfGroups());
   q[4] = new TText(0.02, 0.80, "Nb of groups");
   v[4] = new TText(0.31, 0.80, valI);
   c[4] = new TText(0.61, 0.80, "GetNumberOfGroups()");

   // number of channels
   sprintf(valI,"%i", run->GetNumberOfChannels());
   q[5] = new TText(0.02, 0.76, "Nb of channels");
   v[5] = new TText(0.31, 0.76, valI);
   c[5] = new TText(0.61, 0.76, "GetNumberOfChannels()");

   // number of samples
   sprintf(valI,"%i", run->GetNumberOfSamples());
   q[6] = new TText(0.02, 0.72, "Nb of samples");
   v[6] = new TText(0.31, 0.72, valI);
   c[6] = new TText(0.61, 0.72, "GetNumberOfSamples()");

   // sampling frequency
   sprintf(valD,"%.1f MHz", run->GetSamplingFrequency());
   q[7] = new TText(0.02, 0.68, "Sampling frequency");
   v[7] = new TText(0.31, 0.68, valD);
   c[7] = new TText(0.61, 0.68, "GetSamplingFrequency()");

   // sampling duration
   sprintf(valD,"%.1f us", run->GetSamplingDuration());
   q[8] = new TText(0.02, 0.64, "Sampling duration");
   v[8] = new TText(0.31, 0.64, valD);
   c[8] = new TText(0.61, 0.64, "GetSamplingDuration()");

   // global trigger threshold
   sprintf(valD,"%.1f mV", run->GetGlobalTriggerThreshold());
   q[9] = new TText(0.02, 0.60, "Trigger threshold");
   v[9] = new TText(0.31, 0.60, valD);
   c[9] = new TText(0.61, 0.60, "GetGlobalTriggerThreshold()");

   // post trigger fraction
   sprintf(valD,"%.2f", run->GetPostTriggerFraction());
   q[10] = new TText(0.02, 0.56, "Post trigger fraction");
   v[10] = new TText(0.31, 0.56, valD);
   c[10] = new TText(0.61, 0.56, "GetPostTriggerFraction()");

   // pre trigger fraction
   sprintf(valD,"%.2f", run->GetPreTriggerFraction());
   q[11] = new TText(0.02, 0.52, "Pre trigger fraction");
   v[11] = new TText(0.31, 0.52, valD);
   c[11] = new TText(0.61, 0.52, "GetPreTriggerFraction()");

   // external trigger status
   sprintf(valI,"%i", run->GetExternalTriggerStatus());
   q[12] = new TText(0.02, 0.48, "External trigger");
   v[12] = new TText(0.31, 0.48, valI);
   c[12] = new TText(0.61, 0.48, "GetExternalTriggerStatus()");

   // local trigger status
   sprintf(valI,"%i", run->GetLocalTriggerStatus());
   q[13] = new TText(0.02, 0.44, "Local trigger");
   v[13] = new TText(0.31, 0.44, valI);
   c[13] = new TText(0.61, 0.44, "GetLocalTriggerStatus()");

   // group multiplicity
   sprintf(valI,"%i", run->GetGroupMultiplicity());
   q[14] = new TText(0.02, 0.40, "Group multiplicity");
   v[14] = new TText(0.31, 0.40, valI);
   c[14] = new TText(0.61, 0.40, "GetGroupMultiplicity()");

   if (serv) {
      serv->SelectDataBase("exo");
      char sqlQuery[50];
      sprintf(sqlQuery, "select * from runlist where runNumber = '%s'", currentRunName.c_str());
      TSQLResult *res = serv->Query(sqlQuery);
      TSQLRow *row = res->Next();

      // Pressure
      if (row->GetField(4) != NULL) {sprintf(valC,"%s bar", row->GetField(4));}
      else {sprintf(valC,"%s bar", "-");}
      q[15] = new TText(0.02, 0.32, "Pressure");
      v[15] = new TText(0.31, 0.32, valC);
      c[15] = new TText(0.61, 0.32, "SQL query");

      // Grid Voltage
      if (row->GetField(5) != NULL) {sprintf(valC,"%s V", row->GetField(5));}
      else {sprintf(valC,"%s V", "-");}
      q[16] = new TText(0.02, 0.28, "Grid voltage");
      v[16] = new TText(0.31, 0.28, valC);
      c[16] = new TText(0.61, 0.28, "SQL query");

      // Cathode Voltage
      if (row->GetField(6) != NULL) {sprintf(valC,"%s V", row->GetField(6));}
      else {sprintf(valC,"%s V", "-");}
      q[17] = new TText(0.02, 0.24, "Cathode voltage");
      v[17] = new TText(0.31, 0.24, valC);
      c[17] = new TText(0.61, 0.24, "SQL query");

      // First ring
      if (row->GetField(7) != NULL) {sprintf(valC,"%s V", row->GetField(7));}
      else {sprintf(valC,"%s V", "-");}
      q[18] = new TText(0.02, 0.20, "First ring");
      v[18] = new TText(0.31, 0.20, valC);
      c[18] = new TText(0.61, 0.20, "SQL query");

      // Leakage Current
      if (row->GetField(8) != NULL) {sprintf(valC,"%s nA", row->GetField(8));}
      else {sprintf(valC,"%s nA", "-");}
      q[19] = new TText(0.02, 0.16, "Leakage current");
      v[19] = new TText(0.31, 0.16, valC);
      c[19] = new TText(0.61, 0.16, "SQL query");

      // Comment
      if (row->GetField(14) != NULL) {sprintf(valC,"%s", row->GetField(14));}
      else {sprintf(valC,"%s", "-");}
      q[20] = new TText(0.02, 0.12, "Comment");
      v[20] = new TText(0.31, 0.12, valC);
      c[20] = new TText(0.61, 0.12, "");
   }
   else {cout << "Could not connect to SQL server!" << endl;}

   q[0]->SetTextSize(0.03);
   v[0]->SetTextSize(0.03);
   c[0]->SetTextSize(0.03);

   q[0]->SetTextAlign(22);
   v[0]->SetTextAlign(22);
   c[0]->SetTextAlign(22);

   q[0]->Draw();
   v[0]->Draw();
   c[0]->Draw();

   for (int i = 1; i < 21; i++) {
      q[i]->SetTextSize(0.03);
      v[i]->SetTextSize(0.03);
      c[i]->SetTextSize(0.03);
      q[i]->SetTextFont(52);
      v[i]->SetTextFont(52);
      c[i]->SetTextFont(12);
      q[i]->Draw();
      v[i]->Draw();
      c[i]->Draw();
   }

   cInfo->Update();

}

int display::GetFileExt(string filename)
{
   size_t pos = filename.find_last_of(".");
   if(filename.substr(pos + 1) == "root") {return 1;}
   else {
      if(filename.substr(pos + 1) == "mid") {return 2;}
      else {return 0;}
   }
}

void display::GetMinMaxData(double *min, double *max)
{
   double lMin = 0, lMax = 0;
   int ch = 0, lMinINT = 0, lMaxINT = 0;
   int eventID = int(fEventID->GetNumber());

   // initialize
   ch = run->GetChannelID(0);
   run->SetCurrentData(eventID, ch);
   switch (fCBUnit->GetSelected()) {
      case 0:
         run->GetMinMaxCalibratedData(&lMin, &lMax, false);
         break;
      case 1:
         run->GetMinMaxScaledData(&lMin, &lMax, false);
         break;
      case 2:
         {
         run->GetMinMaxRawData(&lMinINT, &lMaxINT, false);
         lMin = double(lMinINT);
         lMax = double(lMaxINT);
         }
         break;
   }

   for (int i = 1; i < nchannel; i++) {
      ch = run->GetChannelID(i);
      run->SetCurrentData(eventID, ch);
      switch (fCBUnit->GetSelected()) {
         case 0:
            run->GetMinMaxCalibratedData(&lMin, &lMax, true);
            break;
         case 1:
            run->GetMinMaxScaledData(&lMin, &lMax, true);
            break;
         case 2:
            {
            run->GetMinMaxRawData(&lMinINT, &lMaxINT, true);
            lMin = double(lMinINT);
            lMax = double(lMaxINT);
            }
            break;
      }
   }
   * min = lMin;
   * max = lMax;

   return;
}

void display::ImportSettings()
{
   string fName;
   TGFileInfo fi;
   fi.fIniDir = StrDup(".");
   const char * filetypes[] = { "Settings File", "*.setting", 0, 0};
   fi.fFileTypes = filetypes;
   new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, &fi);

   if(fi.fFilename == NULL) {return;}

   ifstream settingsFile;
   settingsFile.open(fi.fFilename, ios::in);

   string currentSetting;
   vector<string> settings;
   while (settingsFile >> currentSetting) {
      settings.push_back(currentSetting);
   }

   int nsettings = settings.size();
   for (int i = 0; i < nsettings; i++) {
      int cutAt = settings.at(i).find_first_of("=");
      string quantity;
      double value = 0.0;
      if (cutAt > 0) {
         quantity = settings.at(i).substr(0, cutAt);
         value = atof((settings.at(i).substr(cutAt+1)).c_str());
      }

      if (strstr(quantity.c_str(),"DRIFTVELOCITY") != NULL) {
         if (value >= TGdriftVelocity->GetNumMin()) {TGdriftVelocity->SetNumber(value);}
         else {cout << quantity << " out of range! Setting default values." << endl;}
      }
      if (strstr(quantity.c_str(),"STRIPDISTANCE") != NULL) {
         if (value >= TGstripDistance->GetNumMin()) {TGstripDistance->SetNumber(value);}
         else {cout << quantity << " out of range! Setting default values." << TGstripDistance->GetNumMin() << " " << TGstripDistance->GetNumMax() <<endl;}
      }
      if (strstr(quantity.c_str(),"THRESHOLD") != NULL) {
         if (value >= TGsignalThreshold->GetNumMin()) {TGsignalThreshold->SetNumber(value);}
         else {cout << quantity << " out of range! Setting default values." << endl;}
      }
      if (strstr(quantity.c_str(),"BINSCALING") != NULL) {
         if (value >= TGbinScaling->GetNumMin()) {TGbinScaling->SetNumber(value);}
         else {cout << quantity << " out of range! Setting default values." << endl;}
      }
      if (strstr(quantity.c_str(),"GRIDCHANNEL") != NULL) {
         if (value >= TGgridChannel->GetNumMin() && value <= TGgridChannel->GetNumMax()) {TGgridChannel->SetNumber(value);}
         else {cout << quantity << " out of range! Setting default values." << endl;}
      }
      if (strstr(quantity.c_str(),"FILTERING") != NULL) {if (value == 1) {fCheckFiltering->SetState(kButtonDown); ChangeFiltering();}}
      if (strstr(quantity.c_str(),"FILTERTYPE") != NULL) {
         if (int(value) == 0 || int(value) == 1) {fCBFilterType->Select(int(value));}
         else {cout << quantity << " out of range! Must be 0 or 1." << endl;}
      }
      if (strstr(quantity.c_str(),"LOWERFREQ") != NULL){
         if (value >= TGcutoff1->GetNumMin() && value <= TGcutoff1->GetNumMax()) {TGcutoff1->SetNumber(value);}
         else {cout << quantity << " out of range! Setting default values." << endl;}
      }
      if (strstr(quantity.c_str(),"UPPERFREQ") != NULL){
         if (value >= TGcutoff2->GetNumMin() && value <= TGcutoff1->GetNumMax()) {TGcutoff2->SetNumber(value);}
         else {cout << quantity << " out of range! Setting default values." << endl;}
      }
      if (strstr(quantity.c_str(),"FILTERORDER") != NULL){
         if (value >=TGFilterOrder->GetNumMin() && value <= TGFilterOrder->GetNumMax()) {TGFilterOrder->SetNumber(value);}
         else {cout << quantity << " out of range! Setting default values." << endl;}
      }
   }

   settingsFile.close();
}

void display::ExportSettings()
{
   TGFileInfo fi;
   fi.fIniDir = StrDup(".");
   const char * filetypes[] = {"setting", "*.setting", 0, 0};
   fi.fFileTypes = filetypes;
   new TGFileDialog(gClient->GetRoot(), 0, kFDSave, &fi);
   if (fi.fFilename) {
      string filename = fi.fFilename;
      char fname[50];
      const char *filetype = fi.fFileTypes[fi.fFileTypeIdx];
      size_t pos = filename.find_last_of(".");
      string ext =  filename.substr(pos + 1);
      if (ext == filetype) {sprintf(fname, "%s", fi.fFilename);}
      else {sprintf(fname, "%s.%s", fi.fFilename, filetype);}
      ofstream settingsFile;
      settingsFile.open(fname, ios::out);
      if (settingsFile.is_open()) {
         settingsFile << "DRIFTVELOCITY=" << TGdriftVelocity->GetNumber() << "\n";
	 settingsFile << "STRIPDISTANCE=" << TGstripDistance->GetNumber() << "\n";
         settingsFile << "THRESHOLD=" << TGsignalThreshold->GetNumber() << "\n";
         settingsFile << "BINSCALING=" << TGbinScaling->GetNumber() << "\n";
         settingsFile << "GRIDCHANNEL=" << TGgridChannel->GetNumber() << "\n";
         if (fCheckFiltering->GetState() == kButtonDown) {settingsFile << "FILTERING=1\n";}
         else {settingsFile << "FILTERING=0\n";}
         settingsFile << "FILTERTYPE=" << fCBFilterType->GetSelected() << "\n";
         settingsFile << "LOWERFREQ=" << TGcutoff1->GetNumber() << "\n";
         settingsFile << "UPPERFREQ=" << TGcutoff2->GetNumber() << "\n";
         settingsFile << "FILTERORDER=" << TGFilterOrder->GetNumber() << "\n";
      }
      settingsFile.close();
   }
}

Bool_t display::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
    // Process events generated by the buttons in the frame.
    switch (GET_MSG(msg)) {
        case kC_COMMAND:
            switch (GET_SUBMSG(msg)) {
                case kCM_BUTTON:
                    switch (parm1) {
                       case TB_BUTTON_NEW:
                          CreateCanvas("New tab");
                          break;
                       case TB_BUTTON_OPEN:
                          OpenFile();
                          break;
                       case TB_BUTTON_SAVE:
                          SaveCanvas();
                          break;
                       case TB_BUTTON_CLOSE:
                          CloseTab();
                          break;
                       case TB_BUTTON_PREVIOUS:
                          GetPreviousEvent();
                          break;
                       case TB_BUTTON_NEXT:
                          GetNextEvent();
                          break;
                       case TB_BUTTON_INFO:
                          GetRunInfo();
                          break;
                       case PRINT_BUTTON:
                          {
                          ExecPrint();
                          PrintDialog->CloseWindow();
                          }
                          break;
                    }
                    break;
                case kCM_MENU:
                   switch (parm1) {
                      case M_FILE_OPEN:
                         OpenFile();
                         break;
                      case M_FILE_SAVE:
                         SaveCanvas();
                         break;
                      case M_FILE_IMPORT:
                         ImportSettings();
                         break;
                      case M_FILE_EXPORT:
                         ExportSettings();
                         break;
                      case M_FILE_CLOSE:
                         CloseAllTabs();
                         break;
                      case M_FILE_PRINT:
                         PrintCanvas();
                         break;
                      case M_FILE_EXIT:
                         gApplication->Terminate(0);
                         break;
                      case M_OPTIONS_STATS:
                         {
                         if (fMenuOptions->IsEntryChecked(M_OPTIONS_STATS)) {
                            fMenuOptions->UnCheckEntry(M_OPTIONS_STATS);
                            gStyle->SetOptStat(0);
                         }
                         else {
                            fMenuOptions->CheckEntry(M_OPTIONS_STATS);
                            gStyle->SetOptStat(1);
                         }
                         }
                         break;
                      case M_OPTIONS_NAMES:
                         {
                         if (fMenuOptions->IsEntryChecked(M_OPTIONS_NAMES)) {
                            fMenuOptions->UnCheckEntry(M_OPTIONS_NAMES);
                            gStyle->SetOptTitle(0);
                         }
                         else {
                            fMenuOptions->CheckEntry(M_OPTIONS_NAMES);
                            gStyle->SetOptTitle(1);
                         }
                         }
                         break;
                      case M_OPTIONS_CROSSHAIR:
                         {
                         if (fMenuOptions->IsEntryChecked(M_OPTIONS_CROSSHAIR)) {
                            fMenuOptions->UnCheckEntry(M_OPTIONS_CROSSHAIR);
                            gPad->SetCrosshair(0);
                         }
                         else {
                            fMenuOptions->CheckEntry(M_OPTIONS_CROSSHAIR);
                            gPad->SetCrosshair(1);
                         }
                         }
                         break;
                      case M_TOOLS_NEWTAB:
                         CreateCanvas("New tab");
                         break;
                      case M_TOOLS_CLOSETAB:
                         CloseTab();
                         break;
                      case M_TOOLS_CLEARTABS:
                         CloseAllTabs();
                         break;
                      case M_TOOLS_NEWCANVAS:
                         new TCanvas();
                         break;
                      case M_TOOLS_OPENBROWSER:
                         new TBrowser();
                         break;
                      case M_TOOLS_RUNINFO:
                         GetRunInfo();
                         break;
                   }
                   break;
                case kCM_COMBOBOX:
                   switch (parm1) {
                      case CB_GAS:
                         ChangeUseDB();
                         break;
		      case CB_CHMAP:
                         ChangeChMap();
                         break;
                   }
                   break;
                default:
                    break;
            }
        default:
            break;
    }
    return kTRUE;
}

Bool_t TRootEmbeddedCanvas::HandleContainerMotion(Event_t *event)
{
   // Handle mouse motion event in the canvas container.
   if (!fCanvas) return kTRUE;

   Int_t x = event->fX;
   Int_t y = event->fY;

   if (fButton == 0)
      fCanvas->HandleInput(kMouseMotion, x, y);
   if (fButton == kButton1)
      fCanvas->HandleInput(kButton1Motion, x, y);

   char cursorPosition[80];
   sprintf(cursorPosition, "[%i,%i] %s", x, y, GetObjectInfo(x,y));
   //disp.stat2->AddText(cursorPosition,0);
   //cout << cursorPosition << endl;

   return kTRUE;
}

int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);
   display *disp = new display(gClient->GetRoot(), 1024, 768);
   disp->Init();
   theApp.Run();
   return 0;
}
