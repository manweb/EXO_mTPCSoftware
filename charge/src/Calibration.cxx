#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TTree.h>
#include <TObject.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TH1.h>
#include <TF1.h>
#include <TMath.h>
#include <TLine.h>

#include "Data.h"
#include <vector>

using namespace std;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
  if (argc!=4) {
     cout << "Usage:" << endl;
     cout << "Calibration Filepath calibFiles.dat output.root" << endl;
     return -1;
  }

  string pname = argv[1];
  string fname = argv[2];
  string oname = argv[3];

  int chStart; // start channel
  int chEnd; // end channel
  int chOffs; // channel offset
  int deg = 1;

  cout << "Specify start channel: ";
  cin >> chStart;
  cout << "Specify end channel: ";
  cin >> chEnd;
  cout << "Specify channel offset: ";
  cin >> chOffs;
  cout << endl;
  cout << "Enter degree of polynomial fit: ";
  cin >> deg;
  cout << endl;
  
  char fitFunction[50];
  sprintf(fitFunction,"pol%i",deg);

  int nChannel = chEnd - chStart;
  
  // read file with run names and correspondig calibration voltages
  ifstream calibFile;
  calibFile.open(fname.c_str(), ios::in);

  // get run names and voltages
  string currentFile;
  vector<string> allFiles;
  vector<double> calibValues;
  while (calibFile >> currentFile) {
     int cutAt = currentFile.find_first_of(";");
     allFiles.push_back(currentFile.substr(0, cutAt));
     calibValues.push_back(atof(currentFile.substr(cutAt + 1).c_str()));
  }

  calibFile.close();
  int nFiles = allFiles.size();
  cout << "Number of calibration runs: " << nFiles << endl;

  // read file with resistor values
  ifstream resFile;
  resFile.open("resValues.dat", ios::in);

  // get resistor values
  string currentRes;
  vector<double> resValues;
  while (resFile >> currentRes) {
     resValues.push_back(atof(currentRes.c_str()));
  }

  resFile.close();
  int nResValues = resValues.size();
  cout << "Number of resistor values: " << nResValues << endl;

  double meanLp[nFiles][nChannel];
  double meanHp[nFiles][nChannel];
  double sigLp[nFiles][nChannel];
  double sigHp[nFiles][nChannel];

  char runName[20];
  char HName[10]; // histogram name
  char HTitle[20]; // histogram title
  char fitFcnName[20]; // fit function 1 name
  char fitFcnName2[20]; // fit function 2 name
  char grName[50]; // graph name
  char grTitle[50]; // graph title
  char chResult[10]; // channel for result display
  char aResult[20]; // parameter a for result display
  char bResult[20]; // parameter b for result display
  char chi2Result[20]; // chi2 for result display

  Double_t par[3];
  Double_t par2[3];
  Double_t par3[deg];

  int rangeSplit = 0; // value where histogram gets split for fitting
  double maxR = 0; // maximum bin of right peak
  double maxL = 0; // maximum bin of left peak
  double mayR = 0; // maximum of right peak
  double mayL = 0; // maximum of left peak

  Run_t *run = new Run_t(); // create run_t object

  unsigned int nevent = 0;
  unsigned int nsamples = 0;
  
  // allocate memory for the histogram and fit functions
  TH1F *h1 = new TH1F("histo","histogram",4500,0,4500);
  TF1 *fitFcn = new TF1("fitFcn","gaus",0,4500);
  TF1 *fitFcn2 = new TF1("fitFcn2","gaus",0,4500);
  TF1 *fitFcn3 = new TF1("fitFcn3",fitFunction,0,4500);
  
  TLine *l1 = new TLine();
  TLine *l2 = new TLine();
  
  l1->SetLineWidth(2);
  l1->SetLineColor(kRed);
  
  l2->SetLineWidth(2);
  l2->SetLineColor(kRed);

  // allocate memory for graphs
  TGraph *gr1 = new TGraph(nFiles);
  TGraphErrors *gr2 = new TGraphErrors(nFiles);

  // set graph properties
  gr1->SetMarkerStyle(20);
  gr1->SetMarkerColor(kRed);
  gr2->SetMarkerStyle(20);
  gr2->SetMarkerColor(kBlue);
  gr2->GetXaxis()->SetTitle("ADC Value");
  gr2->GetYaxis()->SetTitle("I_{in} [#muA]");

  // set fit function parameter names
  fitFcn->SetParNames("a1","x01","sig1");
  fitFcn2->SetParNames("a1","x01","sig1");
  fitFcn3->SetParNames("a","b");

  fitFcn->SetLineWidth(1);
  fitFcn->SetLineColor(kRed);
  fitFcn2->SetLineWidth(1);
  fitFcn2->SetLineColor(kRed);

  for (int i = 0; i < nFiles; i++) { // loop over all calibration files
     sprintf(runName, "run%i (%.0fmV)", i, calibValues.at(i));
     TFile f2(oname.c_str(),"Update");
     f2.mkdir(runName); // create folder for every run
     f2.Close();
     
     string rootFile = pname + allFiles.at(i);

     if (calibValues.at(i) < 0) {rangeSplit = 2200;} // get range split value
     else {rangeSplit = 1900;}

     //run = 0;
     run->OpenRun(rootFile.c_str()); // open run

     // get handle on data
     int * rawAxis = run->GetRawData();

     if (run->RunLoaded()) {cout << "--> Reading file " << rootFile.c_str() << endl;} // check if run is loaded
     else {return 0;}

     nevent = run->GetNumberOfEvents();
     nsamples = run->GetNumberOfSamples();

     cout <<  "--> Nb of Events: " << nevent << endl;
     
     for (int chID = chStart; chID < chEnd; chID++) {
        cout << "--> Processing channel " << chID;
        sprintf(HName, "CH%i", chID);
        sprintf(HTitle, "CH%i, %.0fmV", chID, calibValues.at(i));
        sprintf(fitFcnName, "fitFcn1CH%i", chID);
        sprintf(fitFcnName2, "fitFcn2CH%i", chID);
        h1->Reset();
        h1->SetName(HName);
        h1->SetTitle(HTitle);
        for (unsigned int evtID = 0; evtID < nevent; evtID++) {
           run->SetCurrentData(evtID, chID);
           for (unsigned int k = 0; k < nsamples; k++) {
              h1->Fill(rawAxis[k]); // fill the histogram with the data from the selected channel
           }
        }

        h1->GetXaxis()->SetRange(0,rangeSplit);
        maxL = h1->GetMaximumBin(); // Get first peak from left
        mayL = h1->GetMaximum();
       
        // Get mean value instead of fit
        double mean1 = h1->GetMean();
        double meanErr1 = h1->GetMeanError();
       
        h1->GetXaxis()->SetRange(rangeSplit,4500);
        maxR = h1->GetMaximumBin(); // Get first peak from right
        mayR = h1->GetMaximum();
       
        // Get mean value instead of fit
        double mean2 = h1->GetMean();
        double meanErr2 = h1->GetMeanError();
       
        h1->GetXaxis()->SetRange(0,4500);

        // set fit range
        fitFcn->SetRange(maxL-100,maxL+100);
        fitFcn2->SetRange(maxR-100,maxR+100);

        // set fit parameters according to location of max peak
        fitFcn->SetParameters(mayL,maxL,50);
        fitFcn2->SetParameters(mayR,maxR,50);

        h1->Fit("fitFcn","rqn"); // Fit left peak
        fitFcn->GetParameters(par);
        fitFcn->SetParameters(par);
        h1->Fit("fitFcn2","rqn"); // Fit right peak
        fitFcn2->GetParameters(par2);
        fitFcn2->SetParameters(par2);
       
       l1->SetX1(mean1);
       l1->SetX2(mean1);
       l1->SetY1(0);
       l1->SetY2(mayL);
       l2->SetX1(mean2);
       l2->SetX2(mean2);
       l2->SetY1(0);
       l2->SetY2(mayR);

        TCanvas *c = new TCanvas();

        // plot histogram and fit functions
        h1->Draw();
        //fitFcn->Draw("same");
        //fitFcn2->Draw("same");
        l1->Draw("same");
        l2->Draw("same");

        // save plot to file
        TFile f2(oname.c_str(),"Update"); // open file for writing
        f2.cd(runName);
        c->Write(HName);
        f2.Close();

        delete c;
        c = NULL;

        if (calibValues.at(i) < 0) {
           meanLp[i][chID - chStart] = mean1; //par[1];
           meanHp[i][chID - chStart] = mean2; //par2[1];
           sigLp[i][chID - chStart] = meanErr1; //par[2];
           sigHp[i][chID - chStart] = meanErr2; //par2[2];
        }
        else {
           meanHp[i][chID - chStart] = mean1; //par[1];
           meanLp[i][chID - chStart] = mean2; //par2[1];
           sigHp[i][chID - chStart] = meanErr1; //par[2];
           sigLp[i][chID - chStart] = meanErr2; //par2[2];
        }

        cout << " .... done" << endl;
     }

     run->CloseRun();
     cout << "--> Closing file " << rootFile.c_str() << endl << endl;
  }

  // generate calibration values and display
  cout << "+---------------------"; for (int i = 0; i < deg+1; i++) {cout << "----------------";} cout << "+" << endl;
  cout << "| Calibration results "; for (int i = 0; i < deg+1; i++) {cout << "                ";} cout << "|" << endl;
  cout << "+---------------------"; for (int i = 0; i < deg+1; i++) {cout << "----------------";} cout << "+" << endl;
  cout << "+---------------------"; for (int i = 0; i < deg+1; i++) {cout << "----------------";} cout << "+" << endl;
  cout << "| Channel |"; for (int i = 0; i < deg+1; i++) {cout << "      p" << i << "       |";} cout << "    Chi2   |" << endl;
  cout << "+---------------------"; for (int i = 0; i < deg+1; i++) {cout << "----------------";} cout << "+" << endl;
  
  double grVx[nFiles];
  double grVy[nFiles];
  double grVsx[nFiles];
  double grVsy[nFiles];

  double grLpVx[nFiles];
  
  double R = 0.0;
  double Vin = 0.0;
 
  ofstream oValues("calibValues.txt", ios::app);
  for (int chID = 0; chID < nChannel; chID++) { // get values from channel chID for each run
     for (int runID = 0; runID < nFiles; runID++) {
        Vin = (calibValues.at(runID))*0.001;
        R = (resValues.at(chID + chStart - chOffs))*1000;
        grVx[runID] = meanHp[runID][chID];
        grLpVx[runID] = meanLp[runID][chID];
        grVy[runID] = (Vin/R)*1000000;
        grVsx[runID] = sigHp[runID][chID];
        grVsy[runID] = sqrt((1/R)*(1/R)*(Vin/100)*(Vin/100)+(Vin/(R*R))*(Vin/(R*R))*1000000)*1000000;
        gr1->SetPoint(runID, grLpVx[runID], grVy[runID]);
        gr2->SetPoint(runID, grVx[runID], grVy[runID]);
        gr2->SetPointError(runID, grVsx[runID], grVsy[runID]);
     }

     TFile f2(oname.c_str(),"Update");
     
     gROOT->Reset();
     gROOT->SetStyle("Plain");
     
     TCanvas *c1 = new TCanvas();

     fitFcn3->SetRange(gr2->GetXaxis()->GetXmin(),gr2->GetXaxis()->GetXmax());
     fitFcn3->SetParameters(-0.001,2.8);
     fitFcn3->SetLineWidth(2);
     gr2->Fit("fitFcn3","rq"); // fit the data with linear function
     fitFcn3->GetParameters(par3);
     fitFcn3->SetParameters(par3);

     sprintf(chResult, "%2i", chID + chStart);
     sprintf(aResult, "%.2f", (par3[0])*1000);
     sprintf(bResult, "%.2f", par3[1]);
     sprintf(chi2Result, "%.2f", fitFcn3->GetChisquare());
    
    oValues << chID + chStart << "\t";
    if (chID + chStart > 9) {cout << "|   " << chID + chStart << "    |";}
    else {cout << "|    " << chID + chStart << "    |";}
    for (int i = 0; i < deg+1; i++) {
      oValues << par3[i] << "\t";
      if (par3[i] < 0) {cout << "  " << Form("%.3e",par3[i]) << "   |";}
      else {cout << "   " << Form("%.3e",par3[i]) << "   |";}
    }
    oValues << fitFcn3->GetChisquare() / fitFcn3->GetNDF() << "\n";
    cout << "   " << Form("%.3f",fitFcn3->GetChisquare() / fitFcn3->GetNDF()) << "   |" << endl;
    
    cout << "|         |";
    for (int i = 0; i < deg+1; i++) {
      cout << " +- " << Form("%.3e",fitFcn3->GetParError(i)) << "  |";
    }
    cout << "           |" << endl;

     sprintf(grTitle, "Calibration CH%i", chID + chStart);
     sprintf(grName, "CalibCH%i", chID + chStart);
     
     gr2->SetTitle(grTitle);
     gr2->Draw("AP");
     //gr2->SetMinimum(0);
     //gr2->SetMaximum(4500);
     gr1->Draw("P");

     c1->Write(grName); // save graph
     f2.Close();

     delete c1;
     c1 = NULL;
  }
  oValues.close();
  
  cout << "+---------------------"; for (int i = 0; i < deg+1; i++) {cout << "----------------";} cout << "+" << endl;
  cout << "+---------------------"; for (int i = 0; i < deg+1; i++) {cout << "----------------";} cout << "+" << endl;
  cout << "--> Calibration results written to calibValues.txt" << endl;
 
  return 1;
}
