#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TH2.h>
#include <TGraph.h>
#include <TF1.h>

#include "Data.h"

using namespace std;
using std::cout;
using std::endl;

TFile *fROOT;
TTree *Tree;

Run_t * run;

string fname;
string oname;
bool quiteMode;

int nEvents;
int nsamples;
int nsamplesTMP;
double * timeAxis;
double * data;
double * intTime;
int *chMapping;

double sigmaThreshold;
unsigned int binScalingFactor;
int chStartX;
int chStartY;
int chEndX;
int chEndY;
int gridCH;

double clusterChargeTreshold;
double largestClusterThreshold;
double minimumEventChargeThreshold;
double chi2Threshold;
double smallClusterThreshold;
double trackMatchThreshold;

double driftVelocity;
double stripDistance;
int rebinFactor;

TH2D *h21;
TH2D *h22;
TH2D *h23;
TH2D *h24;

double h2DBinSizeX;
double h2DBinSizeY1;
double h2DBinSizeY2;

int n_binsX;
int n_binsY;
int maxncells;

double finalCharge;
double trackLength;
double TrackdQdx;
double TrackX1;
double TrackX2;
double TrackY1;
double TrackY2;
double TrackZ1;
double TrackZ2;
double TrackTheta;
double TrackPhi;

void init();
int dQdx(int evtID);
int FindClusters(int *clX,int *clY, int *clID, double *clC, TH2D *h, int &nCluster, int &nFoundCells);
void GetMinMax(int *clM, int *clN, int *clID, int currentID, int size, int &slope, int &min, int &max) ;
int GetMin(int *data, int *clusterID, int currentCluster, int size);
int GetMax(int *data, int *clusterID, int currentCluster, int size);
int IDmax(double *data, int size);
