// **** dEdx **************************************************************************************
// *                                                                                              *
// * Manuel Weber                                                                                 *
// * LHEP, University of Bern                                                                     *
// * 08.2010                                                                                      *
// * v1.0                                                                                         *
// *                                                                                              *
// ************************************************************************************************

#include "dEdx.h"

int main(int argc, char* argv[])
{
  if (argc < 3 || argc > 4) {
     cout << "Usage:" << endl;
     cout << "dEdx rootFile.root output.root" << endl;
     return -1;
  }

  fname = argv[1];
  oname = argv[2];

  if (argc == 4) {
     if (string(argv[3]) == "-q") {quiteMode = true;}
     else {quiteMode = false;}
  }

  // initialize
  init();

  // loop over all events
  if (quiteMode) {cout << "Processing " << nEvents << " events..." << endl;}
  for (int evtID = 0; evtID < nEvents; evtID++) {
     if (!quiteMode) {cout << "Processing event " << evtID << endl;}
     int result = dQdx(evtID);

     if (!quiteMode) {
        if (!result) {cout << "-> Discard this event!" << endl;}
        else {cout << "Event " << evtID << " successfully processed!" << endl;}
     }
     else {
        double procD = round(50.0 / nEvents * evtID);
        int proc = int(procD);
        cout << "\r|";
        for (int i = 0; i < 50; i++) {
           if (i < proc) {cout << "*";}
           else {cout << " ";}
        }
        cout << "| " << 2 * proc << " %";
     }
  }
  if (quiteMode) {cout << endl;}

  // write the tree
  fROOT->Write();

  // close output file
  fROOT->Close();

  // close the run
  run->CloseRun();
  delete run;
}

void init()
{
  sigmaThreshold = 4.0; // sigma threshold for noise estimation
  binScalingFactor = 20; // bin scaling factor
  chStartX = 0; // first channel in X
  chStartY = 30; // first channel in Y
  chEndX = 30; // last channel in X + 1
  chEndY = 61; // last chanenl in Y + 1
  gridCH = 62; // grid channel

  // define constants
  clusterChargeTreshold = 50.0; // fC - minimum charge of the smaller cluster (default: 10)
  largestClusterThreshold = 150.0; // fC - minimum charge of the largest cluster (default: 50)
  minimumEventChargeThreshold = 300.0; // fC - minimum amount of charge on all channels (default: 200)
  chi2Threshold = 0.5; // chi2 threshold for the cluster fitting
  smallClusterThreshold = 0.3; // if size of smaller cluster is larger than size of largest cluster times this factor the tracks must fit
  trackMatchThreshold = 2.0; // cm - threshold within the tracks in X-Z and Y-Z projection must fit

  //driftVelocity = 4.7; // drift velocity in cm/us
  stripDistance = 0.31113; // distance between readout strips in cm
  rebinFactor = 2; // rebinning factor

  // create a Run_t object
  run = new Run_t();

  // open access to a run ...
  run->OpenRun(fname.c_str());

  // load first channel to have access to the number of events and number of samples
  run->SetCurrentData(0,1);

  // get number of events
  nEvents = run->GetNumberOfEvents();

  // get number of samples
  nsamples = run->GetNumberOfSamples();
  nsamplesTMP = nsamples - 1;

  // get handle on time axis and calibrated data ...
  timeAxis = run->GetTimeScale();
  data = run->GetCalibratedData();

  // calculate drift velocity from values in database
  driftVelocity = run->GetDriftVelocity(0);

  // create integrated time and convert into cm using the drift velocity
  intTime = new double[nsamplesTMP];
  for (int i = 0; i < nsamplesTMP; i++) {
     intTime[i] = driftVelocity*(timeAxis[i] + timeAxis[i+1])/2;
  }

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

  // create 2D histograms
  h21 = new TH2D("h21", "x-z projection", nsamplesTMP, intTime[0], intTime[nsamplesTMP-1], n_channelsX, channelsX[0], channelsX[n_channelsX-1]);
  h22 = new TH2D("h22", "y-z projection", nsamplesTMP, intTime[0], intTime[nsamplesTMP-1], n_channelsY, channelsY[0], channelsY[n_channelsY-1]);
  h23 = new TH2D("h23", "clusterX", nsamplesTMP, intTime[0], intTime[nsamplesTMP-1], n_channelsX, channelsX[0], channelsX[n_channelsX-1]);
  h24 = new TH2D("h24", "clusterY", nsamplesTMP, intTime[0], intTime[nsamplesTMP-1], n_channelsY, channelsY[0], channelsY[n_channelsY-1]);

  // bin sizes
  h2DBinSizeX = rebinFactor * (intTime[nsamplesTMP-1] - intTime[0]) / nsamplesTMP;
  h2DBinSizeY1 = (channelsX[n_channelsX-1] - channelsX[0]) / n_channelsX;
  h2DBinSizeY2 = (channelsY[n_channelsY-1] - channelsY[0]) / n_channelsY;

  // set histogram properties
  h21->GetXaxis()->SetTitle("z-axis [cm]");
  h21->GetYaxis()->SetTitle("x-axis [cm]");
  h21->GetZaxis()->SetTitle("charge [fC]");
  h21->GetYaxis()->SetTitleOffset(0.5);

  h22->GetXaxis()->SetTitle("z-axis [cm]");
  h22->GetYaxis()->SetTitle("y-axis [cm]");
  h22->GetZaxis()->SetTitle("charge [fC]");
  h22->GetYaxis()->SetTitleOffset(0.5);

  // rebin histograms to reduce number of cells
  h23->RebinX(rebinFactor);
  h24->RebinX(rebinFactor);

  // get number of bins in histograms
  n_binsX = h23->GetNbinsX();
  n_binsY = h23->GetNbinsY();

  maxncells = n_binsX * n_binsY;

  // open output root file
  fROOT = new TFile(oname.c_str(),"UPDATE");
  Tree = new TTree("t","Muon Rec");

  // set tree branches
  Tree->Branch("x1",&TrackX1,"x1/D");
  Tree->Branch("x2",&TrackX2,"x2/D");
  Tree->Branch("y1",&TrackY1,"y1/D");
  Tree->Branch("y2",&TrackY2,"y2/D");
  Tree->Branch("z1",&TrackZ1,"z1/D");
  Tree->Branch("z2",&TrackZ2,"z2/D");
  Tree->Branch("charge",&finalCharge,"charge/D");
  Tree->Branch("trackLength",&trackLength,"trackLength/D");
  Tree->Branch("dQdx",&TrackdQdx,"dQdx/D");
  Tree->Branch("theta",&TrackTheta,"theta/D");
  Tree->Branch("phi",&TrackPhi,"phi/D");

  // get channel mapping
  int mappingID;
  chMapping = run->GetChannelMapping(&mappingID);
  cout << "Using channel mapping: " << mappingID << endl;

  return;
}

int dQdx(int evtID)
{
  // reset histograms
  h21->Reset();
  h22->Reset();
  h23->Reset();
  h24->Reset();

  // variables for total charge on X- and Y-Stripes
  double totalChargeX = 0;
  double totalChargeY = 0;
  double totalCharge = 0;

  for (int k=chStartX; k<chEndY; k++) {
     int chan = chMapping[k];
     run->SetCurrentData(evtID,chan);

     totalCharge = 0;

     // get integrated data with noise subtracted for current event and channel
     run->GetNoiseSubtractedIntegratedSignal(&totalCharge, binScalingFactor, sigmaThreshold, gridCH);

     // compute total charge in X and Y
     if (k < chEndX && k != gridCH) {totalChargeX += totalCharge;}
     else {if (k != gridCH) {totalChargeY += totalCharge;}}

     // fill 2D histograms
     for (int i = 0; i < nsamplesTMP; i++) {
        if (k < chEndX && k != gridCH) {h21->SetBinContent(i+1,k-chStartX+1,-1 * data[i]);}
        else {if (k != gridCH) {h22->SetBinContent(i+1,k-chStartY+1,-1 * data[i]);}}
     }
  }

  if (!quiteMode) {
     cout << "Total charge x: " << totalChargeX << "fC" << endl;
     cout << "Total charge y: " << totalChargeY << "fC" << endl;
   }

  if (-1*(totalChargeX + totalChargeY) < minimumEventChargeThreshold) {
     if (!quiteMode) {cout << "Event didn't pass minimum charge amount criteria ";}
     return 0;
  }

  // arrays containing coordinates of hot cells
  int clusterDataXM[maxncells];
  int clusterDataXN[maxncells];
  int clusterDataXID[maxncells];
  double clusterDataXC[maxncells];

  int clusterDataYM[maxncells];
  int clusterDataYN[maxncells];
  int clusterDataYID[maxncells];
  double clusterDataYC[maxncells];

  // number of found clusters and number of cells
  int nClustersX = 0;
  int nFoundCellsX = 0;

  int nClustersY = 0;
  int nFoundCellsY = 0;

  int min;
  int max;
  int slope;

  // find the clusters in X
  if (!quiteMode) {cout << "Getting clusters in X... " << endl;}
  FindClusters(clusterDataXM,clusterDataXN,clusterDataXID,clusterDataXC,h21,nClustersX,nFoundCellsX);
  if (!quiteMode) {cout << "   -> Found " << nClustersX << " cluster(s)" << endl;}
  if (nClustersX == 0) {
     if (!quiteMode) {cout << "Nr of clusters in X-Z plane = 0 ";}
     return 0;
  }

  double clusterPointXM[200];
  double clusterPointXN[200];
  double chargeX[100];

  // determine endpoints of clusters in X
  for (int k = 0; k < nClustersX; k++) {
     chargeX[k] = 0;
     for (int i = 0; i < nFoundCellsX; i++) {
        if (clusterDataXID[i] == k) {
           h23->SetBinContent(clusterDataXN[i]+1, clusterDataXM[i]+1, k+1);
           chargeX[k] = chargeX[k] + clusterDataXC[i];
        }
     }
     if (!quiteMode) {cout << "      " << k << ": " << chargeX[k] << " fC" << " - (";}
     GetMinMax(clusterDataXM, clusterDataXN, clusterDataXID, k, nFoundCellsX, slope, min, max);
     clusterPointXN[2*k] = h2DBinSizeX * min + intTime[0];
     clusterPointXN[2*k+1] = h2DBinSizeX * (max + 1) + intTime[0];

     if (slope == 1) {
        clusterPointXM[2*k] = h2DBinSizeY1 * GetMin(clusterDataXM, clusterDataXID, k, nFoundCellsX);
        clusterPointXM[2*k+1] = h2DBinSizeY1 * (GetMax(clusterDataXM, clusterDataXID, k, nFoundCellsX) + 1);
     }
     else {
        clusterPointXM[2*k] = h2DBinSizeY1 * (GetMax(clusterDataXM, clusterDataXID, k, nFoundCellsX)  + 1);
        clusterPointXM[2*k+1] = h2DBinSizeY1 * GetMin(clusterDataXM, clusterDataXID, k, nFoundCellsX);
     }

     if (!quiteMode) {cout << clusterPointXN[2*k] << "," << clusterPointXM[2*k] << "|" << clusterPointXN[2*k+1] << "," << clusterPointXM[2*k+1] << ") " << slope << endl;}
  }

  // find the clusters in Y
  if (!quiteMode) {cout << "Getting clusters in Y... " << endl;}
  FindClusters(clusterDataYM,clusterDataYN,clusterDataYID,clusterDataYC,h22,nClustersY,nFoundCellsY);
  if (!quiteMode) {cout << "   -> Found " << nClustersY << " cluster(s)" << endl;}
  if (nClustersY == 0) {
     if (!quiteMode) {cout << "Nr of clusters in Y-Z plane = 0 ";}
     return 0;}

  double clusterPointYM[200];
  double clusterPointYN[200];
  double chargeY[100];

  // determine endpoints of clusters in Y
  for (int k = 0; k < nClustersY; k++) {
     chargeY[k] = 0;
     for (int i = 0; i < nFoundCellsY; i++) {
        if (clusterDataYID[i] == k) {
           h24->SetBinContent(clusterDataYN[i]+1, clusterDataYM[i]+1, k+1);
           chargeY[k] = chargeY[k] + clusterDataYC[i];
        }
     }
     if (!quiteMode) {cout << "      " << k << ": " << chargeY[k] << " fC" << " - (";}
     GetMinMax(clusterDataYM, clusterDataYN, clusterDataYID, k, nFoundCellsY, slope, min, max);
     clusterPointYN[2*k] = h2DBinSizeX * min + intTime[0];
     clusterPointYN[2*k+1] = h2DBinSizeX * (max + 1) + intTime[0];

     if (slope == 1){
        clusterPointYM[2*k] = h2DBinSizeY2 * GetMin(clusterDataYM, clusterDataYID, k, nFoundCellsY);
        clusterPointYM[2*k+1] = h2DBinSizeY2 * (GetMax(clusterDataYM, clusterDataYID, k, nFoundCellsY) + 1);
     }
     else {
        clusterPointYM[2*k] = h2DBinSizeY2 * (GetMax(clusterDataYM, clusterDataYID, k, nFoundCellsY) + 1);
        clusterPointYM[2*k+1] = h2DBinSizeY2 * GetMin(clusterDataYM, clusterDataYID, k, nFoundCellsY);
     }

     if (!quiteMode) {cout << clusterPointYN[2*k] << "," << clusterPointYM[2*k] << "|" << clusterPointYN[2*k+1] << "," << clusterPointYM[2*k+1] << ") " << slope << endl;}
  }

  double finalTrackXM[2];
  double finalTrackXN[2];
  double finalChargeX;

  // Try to fit largest cluster with smaller ones X
  TGraph *grX = new TGraph(4);
  TF1 *fitX;
  double chi2X;

  if (!quiteMode) {cout << "Fitting X:" << endl;}
  int IDmaxX = IDmax(chargeX, nClustersX);
  if (!quiteMode) {cout << "   ->Largest cluster: " << chargeX[IDmaxX] << " (ID = " << IDmaxX << ")" << endl;}
  if (chargeX[IDmaxX] < largestClusterThreshold) {
     if (!quiteMode) {cout << "Largest cluster in X-Z plane didn't pass minimum amount of charge criteria ";}
     return 0;
  }
  finalTrackXM[0] = clusterPointXM[2*IDmaxX];
  finalTrackXM[1] = clusterPointXM[2*IDmaxX+1];
  finalTrackXN[0] = clusterPointXN[2*IDmaxX];
  finalTrackXN[1] = clusterPointXN[2*IDmaxX+1];
  finalChargeX = chargeX[IDmaxX];
  for (int k = 0; k < nClustersX; k++) {
     if (k != IDmaxX && chargeX[k] > clusterChargeTreshold) {
        grX->SetPoint(0,finalTrackXN[0],finalTrackXM[0]);
        grX->SetPoint(1,finalTrackXN[1],finalTrackXM[1]);
        grX->SetPoint(2,clusterPointXN[2*k],clusterPointXM[2*k]);
        grX->SetPoint(3,clusterPointXN[2*k+1],clusterPointXM[2*k+1]);

        grX->Fit("pol1","q");
        fitX = grX->GetFunction("pol1");
        chi2X = fitX->GetChisquare();
        if (!quiteMode) {cout << "   Chi2: " << chi2X << " (ID = " << k << ")" << " (Charge = " << chargeX[k] << ")" << endl;}
        if (chargeX[k] > smallClusterThreshold * chargeX[IDmaxX] && chi2X > chi2Threshold) {
           if (!quiteMode) {cout << "Too many tracks in X-Z plane ";}
           return 0;
         }

        if (chi2X < chi2Threshold) {
           if (clusterPointXN[2*k] < finalTrackXN[0]) {
              finalTrackXM[0] = clusterPointXM[2*k];
              finalTrackXN[0] = clusterPointXN[2*k];
           }
           else {
              finalTrackXM[1] = clusterPointXM[2*k+1];
              finalTrackXN[1] = clusterPointXN[2*k+1];
           }
           finalChargeX = finalChargeX + chargeX[k];
        }
     }
  }

  double finalTrackYM[2];
  double finalTrackYN[2];
  double finalChargeY;

  // Try to fit largest cluster with smaller ones Y
  TGraph *grY = new TGraph(4);
  TF1 *fitY;
  double chi2Y;

  if (!quiteMode) {cout << "Fitting Y:" << endl;}
  int IDmaxY = IDmax(chargeY, nClustersY);
  if (!quiteMode) {cout << "   ->Largest cluster: " << chargeY[IDmaxY] << " (ID = " << IDmaxY << ")" << endl;}
  if (chargeY[IDmaxY] < largestClusterThreshold) {
     if (!quiteMode) {cout << "Largest cluster in Y-Z plane didn't pass minimum amount of charge criteria ";}
     return 0;
  }
  finalTrackYM[0] = clusterPointYM[2*IDmaxY];
  finalTrackYM[1] = clusterPointYM[2*IDmaxY+1];
  finalTrackYN[0] = clusterPointYN[2*IDmaxY];
  finalTrackYN[1] = clusterPointYN[2*IDmaxY+1];
  finalChargeY = chargeY[IDmaxY];
  for (int k = 0; k < nClustersY; k++) {
     if (k != IDmaxY && chargeY[k] > clusterChargeTreshold) {
        grY->SetPoint(0,finalTrackYN[0],finalTrackYM[0]);
        grY->SetPoint(1,finalTrackYN[1],finalTrackYM[1]);
        grY->SetPoint(2,clusterPointYN[2*k],clusterPointYM[2*k]);
        grY->SetPoint(3,clusterPointYN[2*k+1],clusterPointYM[2*k+1]);

        grY->Fit("pol1","q");
        fitY = grY->GetFunction("pol1");
        chi2Y = fitY->GetChisquare();
        if (!quiteMode) {cout << "   Chi2: " << chi2Y << " (ID = " << k << ")" << " (Charge = " << chargeY[k] << ")" << endl;}
        if (chargeY[k] > smallClusterThreshold * chargeY[IDmaxY] && chi2Y > chi2Threshold) {
           if (!quiteMode) {cout << "Too many tracks in Y-Z plane ";}
           return 0;
        }

        if (chi2Y < chi2Threshold) {
           if (clusterPointYN[2*k] < finalTrackYN[0]) {
              finalTrackYM[0] = clusterPointYM[2*k];
              finalTrackYN[0] = clusterPointYN[2*k];
           }
           else {
              finalTrackYM[1] = clusterPointYM[2*k+1];
              finalTrackYN[1] = clusterPointYN[2*k+1];
           }
           finalChargeY = finalChargeY + chargeY[k];
        }
     }
  }

  if (!quiteMode) {cout << "Final chargeX = " << finalChargeX << " - Final chargeY = " << finalChargeY << endl;}

  if (TMath::Abs(finalTrackXN[0] - finalTrackYN[0]) > trackMatchThreshold || TMath::Abs(finalTrackXN[1] - finalTrackYN[1]) > trackMatchThreshold) {
     if (!quiteMode) {cout << "Track in X and Y do not match ";}
     return 0;
  }

  finalCharge = finalChargeX + finalChargeY;
  TrackX1 = finalTrackXM[0];
  TrackX2 = finalTrackXM[1];
  TrackY1 = finalTrackYM[0];
  TrackY2 = finalTrackYM[1];
  TrackZ1 = (finalTrackXN[0] + finalTrackYN[0]) / 2;
  TrackZ2 = (finalTrackXN[1] + finalTrackYN[1]) / 2;

  double TrackX = TrackX2 - TrackX1;
  double TrackY = TrackY2 - TrackY1;
  double TrackZ = TrackZ2 - TrackZ1;
  double rp = TMath::Sqrt(TrackX*TrackX + TrackY*TrackY);

  trackLength = TMath::Sqrt(TrackX*TrackX + TrackY*TrackY + TrackZ*TrackZ);
  TrackdQdx = -1 * finalCharge / trackLength;

  TrackTheta = TMath::ATan2(TrackY,TrackX);
  TrackPhi = TMath::ATan2(TrackZ,rp);

  if (!quiteMode) {cout << "Track length: " << trackLength << " - Charge = " << finalCharge << " - dQdx = " << TrackdQdx << endl;}

  // fill the tree
  Tree->Fill();

  return 1;
}

int FindClusters(int *clX,int *clY, int *clID, double *clC, TH2D *h, int &nCluster, int &nFoundCells)
{
  // make copy of histogram
  TH2D *h2D = (TH2D*)h->Clone("h2D");

  // rebin histograms to reduce number of cells
  h2D->RebinX(rebinFactor);

  // arrays containing coordinates of hot cells
  int foundCells[maxncells][3];

  int n_cellsFound = 0;
  int n_cellsProcessed = 0;

  // index variables
  int m;
  int n;

  double currentCell;
  double cellThreshold = 0.2;
  int lastCell = 0;

  int fndFirstCell;
  int fndProcessedCellFirst;
  int fndCell;
  int clusterID = 0;

  // search for clusters
  while(lastCell == 0) {
     m = 0;
     n = 0;

     // find first hot cell
     fndFirstCell = 0;
     while(m < n_binsY && fndFirstCell == 0) {
        n = 0;
        while(n < n_binsX && fndFirstCell == 0) {
           currentCell = h2D->GetBinContent(n+1, m+1);
           fndProcessedCellFirst = 0;
           for (int i = 0; i < n_cellsFound; i++) { // check if cell has already been processed
              if (m == foundCells[i][0] && n == foundCells[i][1]) {fndProcessedCellFirst = 1;}
           }
           if (currentCell > cellThreshold && fndProcessedCellFirst == 0) {fndFirstCell = 1;}
           n++;
        }
        m++;
     }

     // continue if last cell was not reached
     if (fndFirstCell == 1) {
        m--;
        n--;

        // put first cell in array
        foundCells[n_cellsFound][0] = m;
        foundCells[n_cellsFound][1] = n;
        foundCells[n_cellsFound][2] = clusterID;

        //cout << "first cell: (" << foundCells[n_cellsFound][0] << "," << foundCells[n_cellsFound][1] << ")" << endl;
        n_cellsFound++;

        double charge = 0.0;
        while (n_cellsProcessed < n_cellsFound) {
           // find neighboring cells
           for (int i = 0; i < 8; i++) {
              switch(i) {
                 case 0: m--; n++; break;
                 case 1: m++; break;
                 case 2: m++; break;
                 case 3: n--; break;
                 case 4: n--; break;
                 case 5: m--; break;
                 case 6: m--; break;
                 case 7: n++; break;
              }

              fndCell = 0;
              for (int k = 0; k < n_cellsFound; k++) { // check if cell has already been found ...
                 if (m == foundCells[k][0] && n == foundCells[k][1]) {fndCell = 1;}
              }

              if (fndCell == 0 && m < n_binsY && n < n_binsX) {
                 currentCell = h2D->GetBinContent(n+1, m+1);
                 if (currentCell > cellThreshold) { // ... if not and content is larger than treshold, put cell in vector
                    foundCells[n_cellsFound][0] = m;
                    foundCells[n_cellsFound][1] = n;
                    foundCells[n_cellsFound][2] = clusterID;
                    charge = charge + currentCell;
                    clC[n_cellsFound] = currentCell;
                    n_cellsFound++;
                 }
              }
           }

           // the cell has been processed
           m++;
           n_cellsProcessed++;

           //cout << "Nr. Found cells: " <<  n_cellsFound << endl;
           //cout << "Nr. Processed cells: " << n_cellsProcessed << endl;

           // find new cell which has not yet been processed
           if (n_cellsProcessed < n_cellsFound) {
              m = foundCells[n_cellsProcessed][0];
              n = foundCells[n_cellsProcessed][1];
           }
        }
        clusterID++;
     }
     else {lastCell = 1;}
  }

  nCluster = clusterID;
  nFoundCells = n_cellsFound;

  for (int i = 0; i < maxncells; i++) {
     clX[i] = foundCells[i][0];
     clY[i] = foundCells[i][1];
     clID[i] = foundCells[i][2];
  }

  delete h2D;

  return 0;
}

void GetMinMax(int *clM, int *clN, int *clID, int currentID, int size, int &slope, int &min, int &max) 
{
  // Get min X
  int minX = 1000;
  int lowestCellY = 0;
  for (int i = 0; i < size; i++) {
     if (clN[i] < minX && clID[i] == currentID) {
        minX = clN[i];
        lowestCellY = clM[i];
     }
  }

  // Get max X
  int maxX = 0;
  int highestCellY = 0;
  for (int i = 0; i < size; i++) {
     if (clN[i] > maxX && clID[i] == currentID) {
        maxX = clN[i];
        highestCellY = clM[i];
     }
  }

  if (lowestCellY < highestCellY) {slope = 1;}
  else {slope = -1;}

  min = minX;
  max = maxX;
}

int GetMin(int *data, int *clusterID, int currentCluster, int size)
{
  int min = 1000;
  for (int i = 0; i < size; i++) {
     if (data[i] < min && clusterID[i] == currentCluster) {min = data[i];}
  }

  return min;
}

int GetMax(int *data, int *clusterID, int currentCluster, int size)
{
  int max = 0;
  for (int i = 0; i < size; i++) {
     if (data[i] > max && clusterID[i] == currentCluster) {max = data[i];}
  }

  return max;
}

int IDmax(double *data, int size)
{
  double max = 0.0;
  int k = 0;
  for (int i = 0; i < size; i++) {
     if (data[i] > max) {
        max = data[i];
        k = i;
     }
  }

  return k;
}
