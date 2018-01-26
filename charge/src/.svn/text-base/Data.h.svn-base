#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <TObject.h>
#include <TArrayI.h>
#include <TObjArray.h>
#include <TTree.h>
#include <TBranch.h>
#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <TMath.h>
#include <TSpectrum.h>
#include <TVirtualFFT.h>
#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <complex>
#include <time.h>

//#define MAX_NUMBER_GROUPS (8)
//#define MAX_NUMBER_CHANNELS (64)
//#define NUMBER_CHANNELS_PER_GROUP (8)

//unsigned int MAX_NUMBER_GROUPS = 8;
//unsigned int MAX_NUMBER_CHANNELS = 64;
//unsigned int NUMBER_CHANNELS_PER_GROUP = 8;

using namespace std;
using std::cout;
using std::endl;

// --------------CLASS CHANNEL ----------------------------

class Channel_t : public TObject
{
private:
  TArrayI *data;
  int ID,DT,groupID;
  
public:
  Channel_t() : ID(0), DT(0) , groupID(0) { data = new TArrayI();}
  ~Channel_t(){ delete data;}
  void Init(int groupid, int id, int dt);
  void Data(int *datain) {data->Set(DT,datain);}
  
  int GetGroupID() const { return groupID; }
  int GetID() const { return ID; }
  int GetDT() const { return DT; }
  TArrayI *GetData() { return data; }
  
  ClassDef(Channel_t,1)
};

// --------------CLASS MIDASHEADER ----------------------------

class MidasHeader {
 private:
  unsigned int event_id;
  unsigned int trigger_mask;
  unsigned int serial_nb;
  unsigned int time_stamp;
  unsigned int data_size;
  //  const char *bank_list;

 public:
  MidasHeader() : event_id(0), trigger_mask(0), serial_nb(0), time_stamp(0), data_size(0) { }
  virtual ~MidasHeader() {}
  void Set(unsigned int ei, unsigned int tm, unsigned int serial, unsigned int ts, unsigned int ds) { event_id = ei; trigger_mask = tm; serial = serial_nb; time_stamp = ts; data_size = ds;}

  unsigned int GetEventId() const { return event_id; }
  unsigned int GetTriggerMask() const { return trigger_mask; }
  unsigned int GetSerialNb() const { return serial_nb; }
  unsigned int GetTimeStamp() const { return time_stamp; }
  unsigned int GetDataSize() const { return data_size; }

  ClassDef(MidasHeader,1)
};

// --------------CLASS CaenHEADER ----------------------------

class CaenHeader {
 private:
  unsigned int caenID;
  unsigned int eventSize;
  unsigned int boardID;
  unsigned int res;
  unsigned int pattern;
  unsigned int group_mask;
  unsigned int reserved;
  unsigned int event_count;
  unsigned int trigger_time_tag; 
 
 public:
  CaenHeader() : caenID(0), eventSize(0), boardID(0), res(0), pattern(0), group_mask(0), reserved(0), event_count(0), trigger_time_tag(0) {}
  virtual ~CaenHeader() {}
  void Set(  unsigned int cID, unsigned int ceS,unsigned int cbID, unsigned int cres,unsigned int cpat,unsigned int cg_m,unsigned int cr,unsigned int ce_c,unsigned int cttt) { caenID = cID; eventSize = ceS; boardID = cbID; res = cres; pattern = cpat; group_mask = cg_m; reserved = cr; event_count = ce_c; trigger_time_tag = cttt;}

  unsigned int GetCaenID() const { return caenID; }
  unsigned int GetEventSize() const { return eventSize; }
  unsigned int GetBoardID() const { return boardID; }
  unsigned int GetRes() const { return res; }
  unsigned int GetPattern() const { return pattern; }
  unsigned int GetGroupMask() const { return group_mask; }
  unsigned int GetReserved() const { return reserved; }
  unsigned int GetEventCount() const { return event_count; }
  unsigned int GetTriggerTimeTag() const { return trigger_time_tag; }

  ClassDef(CaenHeader,1)
};

// --------------CLASS USERHEADER ----------------------------
class UserHeader {
 private:
  int nb_gr;
  int nb_ch;
  int nb_dt; 
  //   int duration;

 public:
  //  UserHeader() : nb_gr(0), nb_ch(0), nb_dt(0), duration(0) {}
  UserHeader() : nb_gr(0), nb_ch(0), nb_dt(0) {}
  virtual ~UserHeader() {}
  //  void Set(int nb_group, int nbdt, int durat) { nb_gr = nb_group; nb_dt = nbdt; nb_ch = 8*nb_gr; duration = durat;}
  void Set(int nb_group, int nbdt) { nb_gr = nb_group; nb_dt = nbdt; nb_ch = 8*nb_gr;}
  
  int GetNbGroup() const { return nb_gr; }
  int GetNbChannel() const { return nb_ch; }
  int GetNbDt() const { return nb_dt; }
  //   int GetDuration() const { return duration; }

  ClassDef(UserHeader,1)
};

// --------------CLASS ODB SETTINGS ----------------------------

class OdbSettings {
 private:
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

 public:
  OdbSettings() {}
  virtual ~OdbSettings() {}
  void Set(double Omini_freq,
	   double Omini_duration,
	   double Oglobal_range_min,
	   double Ogroup_range_min[8],
	   double Oglobal_trig_thres,
	   double Ogroup_trig_thres[8],
	   double Ocalib_slope[64],
	   double Ocalib_offset[64],
	   unsigned int Opost_trigger,
	   unsigned int Ogroup_muliplicity,
	   bool   Ogroup_enable[8],
	   bool   Ochannel_enable[64],
	   bool   Oext_trig,
	   bool   Olocal_trig);

  double GetMinFreq() const { return mini_freq; }
  double GetMiniDuration() const { return mini_duration; }
  double GetGlobalRangeMin() const { return global_range_min; }
  double *GetGrpRangeMin() { return group_range_min; }
  double GetGlobalTrigThres() const { return global_trig_thres; }
  double *GetGrpTrigThres() { return group_trig_thres; }
  double *GetCalibSlope() { return calib_slope; }
  double *GetCalibOffset() { return calib_offset; }
  unsigned int GetPostTrigger() const { return post_trigger; }
  unsigned int GetGrpMultiplicity() const { return group_muliplicity; }
  bool *GetGrpEnable() { return group_enable; }
  bool *GetChannelEnable() { return channel_enable; }
  bool GetExtTrig() const { return ext_trig; }
  bool GetLocalTrig() const { return local_trig; }

  ClassDef(OdbSettings,1)
};

// --------------CLASS Data_t ----------------------------

class Data_t : public TObject
{
 private:
  MidasHeader MidHdr;
  CaenHeader  CaenHdr;
  UserHeader  UserHdr;
  OdbSettings OdbSet;
  TObjArray   *chan;

 public:
  Data_t();
  ~Data_t();
  void SetMidasHeader(unsigned int ei, unsigned int tm, unsigned int serial, unsigned int ts, unsigned int ds) { MidHdr.Set(ei,tm,serial,ts,ds); }
  MidasHeader *GetMidasHeader() { return &MidHdr; }
  
  void SetCaenHeader(unsigned int cID, unsigned int ceS,unsigned int cbID, unsigned int cres,unsigned int cpat,unsigned int cg_m,unsigned int cr,unsigned int ce_c,unsigned int cttt) { CaenHdr.Set(cID, ceS,cbID, cres,cpat,cg_m,cr,ce_c,cttt); }
  CaenHeader *GetCaenHeader() { return &CaenHdr; }
  
  // void SetUserHeader(int nbg, int nbdt, int dur) { UserHdr.Set(nbg,nbdt,dur); }
  void SetUserHeader(int nbg, int nbdt) { UserHdr.Set(nbg,nbdt); }
  UserHeader *GetUserHeader() { return &UserHdr; }
  
  void SetOdbSettings(double Omini_freq,
		      double Omini_duration,
		      double Oglobal_range_min,
		      double Ogroup_range_min[8],
		      double Oglobal_trig_thres,
		      double Ogroup_trig_thres[8],
		      double Ocalib_slope[64],
		      double Ocalib_offset[64],
		      unsigned int Opost_trigger,
		      unsigned int Ogroup_muliplicity,
		      bool   Ogroup_enable[8],
		      bool   Ochannel_enable[64],
		      bool   Oext_trig,
		      bool   Olocal_trig) {
    OdbSet.Set(Omini_freq,
	       Omini_duration,
	       Oglobal_range_min,
	       Ogroup_range_min,
	       Oglobal_trig_thres,
	       Ogroup_trig_thres,
	       Ocalib_slope,
	       Ocalib_offset,
	       Opost_trigger,
	       Ogroup_muliplicity,
	       Ogroup_enable,
	       Ochannel_enable,
	       Oext_trig,
	       Olocal_trig); }
  OdbSettings *GetOdbSettings() { return &OdbSet;}
  
  void SetChannel(Channel_t *channel) { chan->Add(channel); }
  TObjArray *GetChannel(){ return chan;}

  ClassDef(Data_t,1)
};

//  --------------CLASS Run_T ----------------------------

class Run_t : public TObject
{
  private:
    TFile * f; // allocated
    TTree * t; // working pointer
    Data_t * data; // allocated
    unsigned int duration; // leaf buffer
    unsigned int * groupIDs; // buffer for group ID's
    double * timeScale; // buffer for the time scale, i.e. the X-axis of the event/channel data
    unsigned int currentEventID; // holds the event ID of the current event
    unsigned int currentSerialChannelNumber; // holds the current serial channel number 
    int * rawData; // buffer for the Y-scale in raw ADC # for one channel of one event
    double * scaledData; // buffer for the Y-scale in [mV] for one channel of one event
    double * calibratedData; // buffer for the Y-scale in [nA] for one channel of one event
    double * frequencySpectrum; // buffer for the frequency spectrum for one channel of one event
    double * butterworthFilter; // buffer for the filter shape for one channel of one event
    unsigned int midasTimeStamp; // event time stamp in seconds (UNIX time)
    unsigned int caenTimeStamp; // hardware high resolution time stamp in number of clocks (31 bits + 1 bit overflow)
    TSQLServer *serv; // pointer to mySQL database
    string currentRunName; // current run name
    bool fUseCalibDB;
    double CalibValues[64][5];
  public:
    Run_t(); // init. internal data structures
    Run_t(const char * fName); // init. internal data structures and calls OpenRun() method
    ~Run_t(); // clears internal data structures
    int OpenRun(const char * fName); // opens ROOT file
    int CloseRun(); // closes ROOT file
    bool RunLoaded(); // returns true if data available, i.e. run is open
    bool RunNotLoaded(); // returns true is data is NOT available, i.e. run is NOT open
    unsigned int GetEventID(); // returns event ID from Midas header
    unsigned int GetTimeStamp(); // returns time stamp from Midas header
    unsigned int GetEventSize(); // returns event size from Caen header
    unsigned int GetGroupMask(); // returns group mask from Caen header
    double GetRunDuration(); // returns run duration is seconds
    unsigned int GetNumberOfEvents(); // returns the total number of events in open run
    unsigned int GetNumberOfGroups(); // returns the number of groups (a group contains 8 DAQ channels that are sampled simultaniously)
    unsigned int GetGroupID(unsigned int serialGroupNumber); // returns the group ID (a fix number tagging the group) when provided with a group serial number in the range zero to "number of groups - 1"
    // some DAQ groups can be absent from certain runs, for example one run contains 5 groups out of maximum 8, then the serial group number runs from 0 to 4 and the previous function must return their absolute group ID which allows the user to identify the proper groups used, for example [0, 1, 5, 6, 7]
    // the DAQ configuration is ordered as a function of the group ID's and preferably should not change from run to run to allow the user to compare data acquired in various runs
    unsigned int GetNumberOfChannels(); // returns the number of channels
    unsigned int GetChannelID(unsigned int serialChannelNumber); // returns the channel ID (a fix number tagging each channel) when provided with a channel serial number, please see previous comments related to group ID's and serial number
    unsigned int GetNumberOfSamples(); // returns the number of samples per channel/event, this number is fixed for each run and identical from event to event and channel to channel due to the way teh DAQ works
    double GetSamplingFrequency(); // returns the sampling frequency in MHz, this number is fixed for each run obviously
    double GetSamplingDuration(); // returns the duration in us of each channel/event, this number is fixed for each run and identical for all channels and events
    double GetGlobalInputRange(); // returns the global input range, this parameter allows to scale the raw data to mV, it is valid only when the group's input range is set to (double)1 therefore flaging the user choice to use the default global parameter, this number represents the minimum in mV of the input range, the maximum value can be calculate by adding 2000 mV
    double GetInputRange(unsigned int groupID); // returns the input range for a specific group ID (NOT group serial number) as those parameters are set in the database ordered by their absolute group ID, if this value is (double)1 the value of the global input range must be considered for the specific group
    double GetGlobalTriggerThreshold(); // returns the global trigger threshold, this parameter is the trigger threshold in mV and applies to all groups that do not have a custom trigger threshold which is flag by a value of (double)0
    double GetTriggerThreshold(unsigned int groupID); // returns the trigger threshold for the specific group ID, if the returned value is (double)0 the global trigger threshold must be considered
    // both input range and trigger threshold parameters can NOT be set channel wise!!!
    double GetCalibrationFactor(unsigned int channelID); // returns the calibration factor (in y = mx + b, the m parameter, in [nA/ADC#]) for the specific channel ID (once again the absolute channel ID is used here and NOT the serial channel number because data is ordered like this is the database and it allows to compare data from various runs)
    double GetCalibrationTerm(unsigned int channelID); // returns the calibration term (in y = mx + b, the b parameter, in [nA])
    double GetPostTriggerFraction(); // returns a number between 0 and 1 indicating the fraction of the event/channel duration after the trigger
    double GetPreTriggerFraction(); // returns a number between 0 and 1 indicating the fraction of the event/channel duration before the trigger
    bool GetExternalTriggerStatus(); // returns yes if the external trigger was enabled, no otherwise
    bool GetLocalTriggerStatus(); // returns yes if the local trigger was enabled, no otherwise
    unsigned int GetGroupMultiplicity(); // returns the number of groups required to get a trigger before a global trigger (inducing data recording) is set
    bool GetGroupEnable(unsigned int groupID); // returns yes if the specific group (absolute group ID) has participated in the trigger when local trigger is enabled and if data from the group has been acquired, no otherwise
    bool GetChannelEnable(unsigned int channelID); // returns yes if the specific channel (absolute channel ID) has participated in the trigger when local trigger is enabled, no otherwise
    double * GetTimeScale(); // returns a pointer to a buffer holding the time scale valid for all events/channels, buffer is allocated and cleared by this class, units are in [us]
    int * GetRawData(); // returns a pointer to a buffer holding the raw data for the set event and channel
    double * GetScaledData(); // returns a pointer to a buffer holding the scaled data in [mV]
    double * GetCalibratedData(); // returns a pointer to a buffer holding the calibrated data in [nA]
    double * GetFrequencySpectrum(); // returns a pointer to a buffer holding the frequency spectrum after calling ApplyFilter()
    double * GetFilterShape(); // returns a pointer to a buffer holding the Butterworth filter shape after calling ApplyFilter()
    unsigned int GetCurrentEventID(); // returns the current event ID
    unsigned int GetCurrentSerialChannelNumber(); // returns the serial channel number
    double GetLowPrecisionEventTime(); // return the event time measure by the host computer (UNIX time) in seconds
    double GetHighPrecisionEventRelativeTime(); // returns the relative time as measured by the CAEN hardware in seconds (this time comes from a hardware counter that loops and so it is useful to compared events closer in time than about 34 seconds; the resolution is 1 / sampling frequency in its units are [us]; total duration is 2^31 clocks at the sampling frequency, the most significative bit tags the overflow and the DAQ does not reset the counter during the run)
    int SetCurrentData(unsigned int eventID, unsigned int serialChannelNumber); // set the data from the eventID and serialChannelNumber which is between zero and the value returned by GetNumberOfChannels(), returns an error code or zero if succesful
    void GetMinMaxRawData(int * min, int * max, bool init = true); // computes the min and max of raw data, if the flag init is true internal registers are initialized with the value of the calling parameters
    void GetMinMaxScaledData(double * min, double * max, bool init = true); // computes the min and max of scaled data, if the flag init is true the internal registers are initialized to the value of the calling parameters
    void GetMinMaxCalibratedData(double * min, double * max, bool init = true); // computes the min and max of calibrated data, if the flag init is true the internal registers are initialized to the value of the calling parameters
    int GetNoiseSubtractedIntegratedSignal(double *totalCharge, unsigned int binScalingFactor = (unsigned int)25, double thresholdInSigma = 5.0, unsigned int gridCH = 62); //returns the charge as a function of time and the total charge on the current channel. The second parameter is a factor that divides the total number of samples to obtain the number of bins for the histogram. The third parameter defines the threshold for the signal area in terms of number of sigmas of the noise distribution. The fourth parameter specifies the grid channel in order to distinguish the grid signal which is positive from the strip signals which are negative.
    double SubtractBaseline(); // subtracts the baseline from the calibrated data and returns the baseline
    double * GetFFTData(double *data, int Filteropt, double f1, double f2, int n); // returns input data after applying a frequency filter. The second parameter is the option to either use bandpass (FilterOpt=0) or bandstop (FilterOpt=1) filter. The third and fourth parameter are the cutoff frequencies in Hz. The last parameter is the order of the Bufferworth filter.
    int ApplyFilter(int FilterOpt, double f1, double f2, int n); // applies selected filter on current channel and event. The current set data are overwritten by the filtered data. Also returns modified frequency spectrum and the filter shape.
    int GetSpectrum(); // returns frequency spectrum of current channel and event.
    double * butter(int n); // returns the coefficients to build an n-th order Butterworth filter.
    int * GetChannelMapping(int *mappingID, int DchMap = 1); // returns channel mapping for current run and mapping ID (date of mapping)
    double GetDriftVelocity(int gasID); // returns drift velocity for selected gas calculated from properties in database. The parameter is the gas ID: 0 = P10, 1 = Xe(98)CF4(2).
    void UseCalibDB(bool use = false); // set whether calibration values are read from the database
    void GetCalibrationValues(int chID); // plots the calibration values of the selected channel

    ClassDef(Run_t,1)
};
