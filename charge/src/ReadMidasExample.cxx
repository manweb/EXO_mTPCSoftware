// **** example ***********************************************************************************
// *                                                                                              *
// * Manuel Weber                                                                                 *
// * LHEP, University of Bern                                                                     *
// * 05.2010                                                                                      *
// *                                                                                              *
// * This is an example on how to read ROOT files created from Midas files. It is using the       *
// * libData.so with the Run_t class to access the data. Here some basic functions of the Run_t   *
// * class are shown like opening a run, getting run information, accessing calibrated data,      *
// * retreiving integrated data and applying frequency filters on the caliabrated data. For       *
// * further information on the various functions in the class, please refer to the Data.h header *
// * file.                                                                                        *
// *                                                                                              *
// ************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TObject.h>
#include <TCanvas.h>
#include <TGraph.h>

#include "Data.h"

using namespace std;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
   if (argc!=4) {
      cout << "Usage:" << endl;
      cout << "ReadMidasExample filename.root chID evtID" << endl;
      return -1;
   }

   cout << " Example to use ROOT files using libData.so (v052110)" << endl;

   string fname  = argv[1];
   int chID = atoi(argv[2]);
   int evtID = atoi(argv[3]);

   int lgh = fname.length();
   int loc = fname.find( ".root", 0 );
   int diff = lgh-loc;

   if( diff != 5 ){
      cout << endl << "  Wrong file format : must be *.root   !!!" <<  endl << endl;
      return -1;
   }

   //int chID = 17;
   //int evtID = 0;

   // create new Run_t object
   Run_t *MyRun = new Run_t();

   // open root file
   MyRun->OpenRun(fname.c_str());

   // check if run is loaded
   if (MyRun->RunLoaded()) {cout << "Run " << fname << " loaded succesfully." << endl;}
   else {cout << "Run " << fname << " could not be loaded." << endl;}

   // get handles on data
   double * timeAxis = MyRun->GetTimeScale();
   int * rawAxis = MyRun->GetRawData();
   double * scaledAxis = MyRun->GetScaledData();
   double * calibratedAxis = MyRun->GetCalibratedData();
   double * frequencySpectrum = MyRun->GetFrequencySpectrum();
   double * filterShape = MyRun->GetFilterShape();

   // get number of samples
   unsigned int nsamples = MyRun->GetNumberOfSamples();

   // get run duration
   double duration = MyRun->GetRunDuration();

   // load first event and first channel
   MyRun->SetCurrentData(evtID,chID);

   // plot some run informaton
   cout << "Current loaded event: " << evtID << endl;
   cout << "Current loaded channel: " << chID << endl;
   cout << "Number of samples: " << nsamples << endl;
   cout << "Duration of run: " << duration << endl;

   char title[50];

   // let's plot the calibrated data
   TGraph *graphData = new TGraph(nsamples, timeAxis, calibratedAxis);
   sprintf(title, "Channel %i, Event %i: Calibrated data", chID, evtID);
   graphData->SetTitle(title);
   graphData->GetXaxis()->SetTitle("Time [#mus]");
   graphData->GetYaxis()->SetTitle("Amplitude [nA]");

   // now we want to plot the inegrated data, i.e. the charge
   // we need to create a new time axis with one sample less
   unsigned int nsamplesTMP = nsamples - 1;
   double *intTime = new double[nsamplesTMP];
   for (unsigned int i = 0; i < nsamplesTMP; i++) {
      intTime[i] = (timeAxis[i] + timeAxis[i+1])/2;
   }

   // get integrated data by calling GetNoiseSubtractedIntegratedSignal. It will overwrite the calibratedAxis. It will also return the charge on the channel
   double charge = 0;
   MyRun->GetNoiseSubtractedIntegratedSignal(&charge, 20, 5, 62);
   cout << "Charge on current channel: " << charge << " fC" << endl;

   // plot the inegrated, noise subtracted signal
   TGraph *graphInt = new TGraph(nsamplesTMP, intTime, calibratedAxis);
   sprintf(title, "Channel %i, Event %i: Inegrated data", chID, evtID);
   graphInt->SetTitle(title);
   graphInt->GetXaxis()->SetTitle("Time [#mus]");
   graphInt->GetYaxis()->SetTitle("charge [fC]");

   // now we want to apply a frequency filter on the calibrated data. Let's choose a bandstop filter with cutoff frequency between 6 MHz and 9 MHz. The second parameter specifies which data to return. 0 = filtered data, 1 = frequency spectrum, 2 = filter shape.
   // first we need to reload the data since calibratedAxis was overwritten by the integrated signal
   MyRun->SetCurrentData(evtID,chID);

   // applying the filter will overwrite the loaded data
   MyRun->ApplyFilter(1, 6*pow(10.0,6), 9*pow(10.0,6), 10);

   // in order to plot the spectrum, we need to create a frequency axis
   double fs = MyRun->GetSamplingFrequency();
   fs = fs*pow(10.0,6); // sampling frequency is in MHz
   int nFFT = ceil(log(nsamples)/log(2)); // get next power of 2
   int nrFFT = pow(2.0, nFFT); // number of FFT samples
   double *f = new double[nrFFT];
   double stp = 1. / nrFFT;
   for (int i = 0; i < nrFFT; i++) {
      f[i] = fs*i*stp;
   }

   // plot the filtered data
   TGraph *graphFFT = new TGraph(nsamples, timeAxis, calibratedAxis);
   sprintf(title, "Channel %i, Event %i: Filtered data", chID, evtID);
   graphFFT->SetTitle(title);
   graphFFT->GetXaxis()->SetTitle("Time [#mus]");
   graphFFT->GetYaxis()->SetTitle("Amplitude [nA]");

   // plot he spectrum including the filter shape
   TGraph *graphSPEC = new TGraph(nrFFT/2, f, frequencySpectrum);
   TGraph *graphFILTER = new TGraph(nrFFT/2, f, filterShape);
   sprintf(title, "Channel %i, Event %i: Frequency spectrum", chID, evtID);
   graphSPEC->SetTitle(title);
   graphSPEC->GetXaxis()->SetTitle("frequency [Hz]");
   graphSPEC->GetYaxis()->SetTitle("Magnitude");

   // open output file
   TFile *fi = new TFile("exampleResult.root","RECREATE");

   // create a new canvas
   TCanvas *c1 = new TCanvas();
   c1->Divide(2,2);

   c1->cd(1);
   graphData->Draw("ALP");

   c1->cd(2);
   graphInt->Draw("ALP");

   c1->cd(3);
   graphFFT->Draw("ALP");

   c1->cd(4);
   graphSPEC->Draw("ALP");

   graphFILTER->SetLineColor(kRed);
   graphFILTER->SetLineStyle(7);
   graphFILTER->Draw("L");

   // write canvas to file
   c1->Write();

   cout << "Result written to exampleResult.root" << endl;

   fi->Close();

   return 1;
}
