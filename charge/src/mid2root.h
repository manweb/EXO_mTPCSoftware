#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TObject.h>
#include <TROOT.h>
#include <TMidasOnline.h>
#include <TMidasFile.h>
#include <TMidasEvent.h>
#include <XmlOdb.h>
#include "Data.h"

using namespace std;
using std::cout;
using std::endl;


bool display=0;

int ReadCaen(void *pt,const int size,bool RootStruct);
int ReadCaenEvent(void *pt, const int size);
void int2bin(unsigned int a);
int CreateRootStruct();
int ReadOdb(TMidasEvent *event);

TFile *fout;
TTree *tree;
Data_t *evt;

int ievt, nentries;
int **channel;
int *group_used, nb_group;
unsigned int duration;
int CaenID_old;

struct MidasHeader_t 
{
  unsigned int event_id;
  unsigned int trigger_mask;
  unsigned int serial_nb;
  unsigned int time_stamp;
  unsigned int data_size;
  //  const char *bank_list;
};
MidasHeader_t MidasHeader;


struct CaenHeader_t 
{
  unsigned int caenID;
  unsigned int eventSize;
  unsigned int boardID;
  unsigned int res;
  unsigned int pattern;
  unsigned int group_mask;
  unsigned int reserved;
  unsigned int event_count;
  unsigned int trigger_time_tag;
};
CaenHeader_t CaenHeader;

struct UserHeader_t
{
  int nb_gr;
  int nb_ch;
  int nb_dt;
  int duration;
};
UserHeader_t UserHeader;


struct Settings_t
{
  double mini_freq;
  double mini_duration;
  double global_range_min;
  double group_range_min[8];
  double global_trig_thres;
  double group_trig_thres[8];
  double calib_slope[64];
  double calib_offset[64];
  unsigned int post_trigger;
  unsigned int group_muliplicity;
  bool   group_enable[8];
  bool   channel_enable[64];
  bool   ext_trig;
  bool   local_trig;  
};
Settings_t Settings;
