#include "Data.h"
ClassImp(MidasHeader)
ClassImp(CaenHeader)
ClassImp(UserHeader)
ClassImp(Channel_t)
ClassImp(OdbSettings)
ClassImp(Data_t)
ClassImp(Run_t)

unsigned int MAX_NUMBER_GROUPS = 8;
unsigned int MAX_NUMBER_CHANNELS = 64;
unsigned int NUMBER_CHANNELS_PER_GROUP = 8;

//  -------------- CLASS Data_T ----------------------------

Data_t::Data_t()
{
  chan = new TObjArray;
}

Data_t::~Data_t()
{
  delete chan;
}

//  -------------- CLASS OdbSettings ----------------------------

void OdbSettings::Set(double Omini_freq,
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
		      bool   Olocal_trig)
{
  mini_freq               = Omini_freq;
  mini_duration           = Omini_duration;
  global_range_min        = Oglobal_range_min;
  global_trig_thres       = Oglobal_trig_thres;
  post_trigger            = Opost_trigger;
  group_muliplicity       = Ogroup_muliplicity;
  ext_trig                = Oext_trig;
  local_trig              = Olocal_trig;

  for (int i=0;i<8;i++)
    {
      group_range_min[i]  = Ogroup_range_min[i];
      group_trig_thres[i] = Ogroup_trig_thres[i];
      group_enable[i]     = Ogroup_enable[i];
    }
  
  for (int i=0;i<64;i++)
    {
      calib_slope[i]      = Ocalib_slope[i];
      calib_offset[i]     = Ocalib_offset[i];
      channel_enable[i]   = Ochannel_enable[i];
    }
}

//  -------------- Channel_t ----------------------------

void Channel_t::Init(int groupid, int id, int dt)
{
  groupID = groupid;
  ID      = id;
  DT      = dt;
  data->Set(DT);
}

//  -------------- CLASS Run_T ----------------------------

Run_t::Run_t()
{
  // init. pointers ... 
  f = NULL;
  t = NULL;
  data = NULL;
  duration = (unsigned int)0;
  groupIDs = NULL;
  timeScale = NULL;
  rawData = NULL;
  scaledData = NULL;
  calibratedData = NULL;
  frequencySpectrum = NULL;
  butterworthFilter = NULL;
  serv = NULL;
  fUseCalibDB = false;
}

Run_t::Run_t(const char * fName)
{
  // init. pointers ...
  f = NULL;
  t = NULL;
  data = NULL;
  duration = (unsigned int)0;
  groupIDs = NULL;
  timeScale = NULL;
  rawData = NULL;
  scaledData = NULL;
  calibratedData = NULL;
  frequencySpectrum = NULL;
  butterworthFilter = NULL;
  serv = NULL;
  // call method to load data from file ... 
  OpenRun(fName);
}

Run_t::~Run_t()
{
  // if run still open close it ...
  if (f != NULL) {
    CloseRun();
  }
}

int Run_t::OpenRun(const char * fName)
{
  if (f != NULL) { // a run is still open, close it first
    CloseRun();
  }
  f = new TFile(fName);
  if (f == NULL) { // oups we are not going very far ...
    cout << "could not allocate memory and open the ROOT file" << endl;
    
    return 1;
  }
  if (f->IsZombie()) {
    cout << "could not open the ROOT file" << endl;
    delete f; // get ride of the file
    f = NULL; // reset to NULL
    
    return 1;
  }
  if (t != NULL) { // oups we may have problems
    cout << "WARNING: pointer to TTree object already pointing to something, better check it out!!!" << endl; // but we can keep on ...
  }
  t = (TTree *)(f->Get("t")); // get tree address ...
  if (data != NULL) { // oups more problems ...
    cout << "WARNING: pointer to Data_t object already pointing to something, better check it out!!!" << endl;
    // we can keep on after cleaning ....
    delete data;
    data = NULL;
  }
  data = new Data_t(); // allocate memory for buffer for the bulk of data
  if (data == NULL) { // oups ...
    cout << "could not allocate memory for the data buffer" << endl;
    t = NULL;
    delete f;
    f = NULL;
    
    return 1;
  }
  (t->GetBranch("data"))->SetAddress(&data); // installs the handle, we have to clean up when finished
  t->SetBranchAddress("duration", &duration); // sets the buffer for the duration leaf
  // let's load the first event which will give us access to the header
  if (!t->GetEntries()) { // no data available ... 
    cout << "no data available, ROOT file is empty" << endl;
    CloseRun();
    
    return 1;
  }
  t->GetEntry(); // loading data for the first event ... we have access to headers now ...
  currentEventID = (unsigned int)0; // set the current event ID
  midasTimeStamp = (data->GetMidasHeader())->GetTimeStamp(); // fetch the Midas time stamp
  caenTimeStamp = (data->GetCaenHeader())->GetTriggerTimeTag(); // fetch the CAEN time stamp
  // let's set the group and channel ID's
  if (groupIDs != NULL) { // we have some old crap ...
    cout << "WARNING: memory for group IDs still allocated in Run_t, better check this!" << endl;
    delete [] groupIDs;
    groupIDs = NULL;
  }
  unsigned int n_groups = GetNumberOfGroups(); // get the number of groups in this run ...
  groupIDs = new unsigned int[n_groups]; // allocate the memory for the table with the group ID's
  unsigned int j = (unsigned int)0; // index over the group ID's
  for (unsigned int i = (unsigned int)0; i < n_groups; i++) { // for every existing group find his ID
    while (!(((data->GetOdbSettings())->GetGrpEnable())[j])) { // increase the group ID index until one finds the group ID for the next enabled group
      j++;
    }
    groupIDs[i] = j; // allocate the group ID
    j++;
  }
  // let's allocate memory for the time scale
  if (timeScale != NULL) { // old crap ...
    cout << "WARNING: memory for the time scale is still allocated in Run_t, better check this!" << endl;
    delete [] timeScale;
    timeScale = NULL;
  }
  unsigned int n_samples = GetNumberOfSamples(); // get the number of samples in each event/channel
  timeScale = new double[n_samples]; // allocate the memory for the time scale, X-axis of event/channel data
  double interval = 1.0 / GetSamplingFrequency(); // time interval between samples in us
  double offset = -1.0 * (GetSamplingDuration() * GetPreTriggerFraction()); // time for pretrigger in us
  for (unsigned int i = (unsigned int)0; i < n_samples; i++) { // let's generate the time scale in [us]
    timeScale[i] = offset + interval * i;
  }
  // setting the data now ... 
  if (rawData != NULL) {
  }
  if (rawData != NULL) { // old crap ...
    cout << "WARNING: memory for the raw data is still allocated in Run_t, better check this!" << endl;
    delete [] rawData;
    rawData = NULL;
  }
  rawData = new int[n_samples]; // allocate the memory
  if (scaledData != NULL) { // old crap ...
    cout << "WARNING: memory for the scaled data is still allocated in Run_t, better check this!" << endl;
    delete [] scaledData;
    scaledData = NULL;
  }
  scaledData = new double[n_samples]; // allocate the memory
  if (calibratedData != NULL) { // old crap ...
    cout << "WARNING: memory for the calibrated data is still allocated in Run_t, better check this!" << endl;
    delete [] calibratedData;
    calibratedData = NULL;
  }
  calibratedData = new double[n_samples]; // allocate the memory
  SetCurrentData((unsigned int)0, (unsigned int)0); // load the buffers with data

  // number of FFT samples
  int nFFT = ceil(log(n_samples)/log(2)); // get next power of 2
  int nrFFT = pow(2.0, nFFT);

  // allocate memory for frequency spectrum and butterworth filter
  if (frequencySpectrum != NULL) { // old crap ...
    cout << "WARNING: memory for the frequency spectrum is still allocated in Run_t, better check this!" << endl;
    delete [] frequencySpectrum;
    frequencySpectrum = NULL;
  }
  frequencySpectrum = new double[nrFFT];

  if (butterworthFilter != NULL) { // old crap ...
    cout << "WARNING: memory for the butterworth filter is still allocated in Run_t, better check this!" << endl;
    delete [] butterworthFilter;
    butterworthFilter = NULL;
  }
  butterworthFilter = new double[nrFFT];

  // set current run name
  string fileName = string(fName);
  int cutAt1 = fileName.find_last_of("/");
  int cutAt2 = fileName.find(".");
  currentRunName = fileName.substr(cutAt1+1, cutAt2 - cutAt1 - 1);

  // we are happy!!!
  return 0;
}

int Run_t::CloseRun()
{
  if (data != NULL) { // clean up ...
    delete data;
    data = NULL;
  }
  if (t != NULL) {
    t = NULL;
  }
  if (f != NULL) {
    delete f;
    f = NULL;
  }
  duration = (unsigned int)0;
  if (groupIDs != NULL) {
    delete [] groupIDs;
    groupIDs = NULL;
  }
  if (timeScale != NULL) {
    delete [] timeScale;
    timeScale = NULL;
  }
  if (rawData != NULL) {
    delete [] rawData;
    rawData = NULL;
  }
  if (scaledData != NULL) {
    delete [] scaledData;
    scaledData = NULL;
  }
  if (calibratedData != NULL) {
    delete [] calibratedData;
    calibratedData = NULL;
  }

  if (frequencySpectrum != NULL) {
    delete [] frequencySpectrum;
    frequencySpectrum = NULL;
  }

  if (butterworthFilter != NULL) {
    delete [] butterworthFilter;
    butterworthFilter = NULL;
  }
  
  return 0;
}

bool Run_t::RunLoaded()
{
  if (f != NULL) {
    
    return true;
  }
  
  return false;
}

bool Run_t::RunNotLoaded()
{
  if (f == NULL) {
    
    return true;
  }
  
  return false;
}

unsigned int Run_t::GetEventID()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 0;
  }

  return (unsigned int)(data->GetMidasHeader()->GetEventId());
}

unsigned int Run_t::GetTimeStamp()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 0;
  }

  return (unsigned int)(data->GetMidasHeader()->GetTimeStamp());
}

unsigned int Run_t::GetEventSize()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 0;
  }

  return (unsigned int)(data->GetCaenHeader()->GetEventSize());
}

unsigned int Run_t::GetGroupMask()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 0;
  }

  return (unsigned int)(data->GetCaenHeader()->GetGroupMask());
}

double Run_t::GetRunDuration()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return -1.0;
  }
  // may add more sofisticated time computation latter, for now it reduces to ...
  
  return (double)duration;
}

unsigned int Run_t::GetNumberOfEvents()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return (unsigned int)0;
  }
  
  return ((unsigned int)(t->GetEntries()) - (unsigned int)1); // one entry count is for the run duration
}

unsigned int Run_t::GetNumberOfGroups()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return (unsigned int)0;
  }
  
  return (unsigned int)((data->GetUserHeader())->GetNbGroup());
}

unsigned int Run_t::GetGroupID(unsigned int serialGroupNumber)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return (unsigned int)0;
  }
  if (serialGroupNumber >= GetNumberOfGroups()) { // out of bounds ...
    cout << "ERROR: you requested " << serialGroupNumber << "-th group ID when there are only " << GetNumberOfGroups() << " groups available!?!" << endl;
    
    return (unsigned int)0;
  }
  
  return groupIDs[serialGroupNumber];
}

unsigned int Run_t::GetNumberOfChannels()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return (unsigned int)0;
  }
  
  return (unsigned int)((data->GetUserHeader())->GetNbChannel());
}

unsigned int Run_t::GetChannelID(unsigned int serialChannelNumber)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return (unsigned int)0;
  }
  if (serialChannelNumber >= GetNumberOfChannels()) { // out of bounds ...
    cout << "ERROR: you requested " << serialChannelNumber << "-th channel ID when there are only " << GetNumberOfChannels() << " channels available!?!" << endl;
    
    return (unsigned int)0;
  }
  
  return (GetGroupID(serialChannelNumber / GetNumberOfGroups()) * NUMBER_CHANNELS_PER_GROUP + (serialChannelNumber % GetNumberOfGroups()));
}

unsigned int Run_t::GetNumberOfSamples()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return (unsigned int)0;
  }
  
  return (unsigned int)((data->GetUserHeader())->GetNbDt());
}

double Run_t::GetSamplingFrequency()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return -1.0;
  }
  
  return (data->GetOdbSettings())->GetMinFreq();
}

double Run_t::GetSamplingDuration()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return -1.0;
  }
  
  return (data->GetOdbSettings())->GetMiniDuration();
}

double Run_t::GetGlobalInputRange()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return -1.0;
  }
  
  return (data->GetOdbSettings())->GetGlobalRangeMin();
}

double Run_t::GetInputRange(unsigned int groupID)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return -1.0;
  }
  if (groupID >= MAX_NUMBER_GROUPS) { // oups buffer overflow ... 
    cout << "ERROR: maximum group ID for this version of the library is " << MAX_NUMBER_GROUPS << endl;
    
    return -1.0;
  }
  double inputRange = ((data->GetOdbSettings())->GetGrpRangeMin())[groupID];
  if (inputRange == 1.0) { // return the global value for this group
    
    return GetGlobalInputRange();
  }
  
  return inputRange;
}

double Run_t::GetGlobalTriggerThreshold()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return -1.0;
  }
  
  return (data->GetOdbSettings())->GetGlobalTrigThres();
}

double Run_t::GetTriggerThreshold(unsigned int groupID)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return -1.0;
  }
  if (groupID >= MAX_NUMBER_GROUPS) { // oups buffer overflow ... 
    cout << "ERROR: maximum group ID for this version of the library is " << MAX_NUMBER_GROUPS << endl;
    
    return -1.0;
  }
  double threshold = ((data->GetOdbSettings())->GetGrpTrigThres())[groupID];
  if (threshold == 0.0) { // return the global value for this group
    
    return GetGlobalTriggerThreshold();
  }
  
  return threshold;
}

double Run_t::GetCalibrationFactor(unsigned int channelID)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 1.0;
  }
  if (channelID >= MAX_NUMBER_CHANNELS) { // oups buffer overflow ... 
    cout << "ERROR: maximum channel ID for this version of the library is " << MAX_NUMBER_CHANNELS << endl;
    
    return 1.0;
  }
  
  return ((data->GetOdbSettings())->GetCalibSlope())[channelID];
}

double Run_t::GetCalibrationTerm(unsigned int channelID)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 0.0;
  }
  if (channelID >= MAX_NUMBER_CHANNELS) { // oups buffer overflow ... 
    cout << "ERROR: maximum channel ID for this version of the library is " << MAX_NUMBER_CHANNELS << endl;
    
    return 0.0;
  }
  
  return ((data->GetOdbSettings())->GetCalibOffset())[channelID];
}

double Run_t::GetPostTriggerFraction()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return -1.0;
  }
  
  return ((double)((data->GetOdbSettings())->GetPostTrigger()) / 100.0);
}

double Run_t::GetPreTriggerFraction()
{
  
  return (1.0 - GetPostTriggerFraction());
}

bool Run_t::GetExternalTriggerStatus()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return false;
  }
  
  return (data->GetOdbSettings())->GetExtTrig();
}

bool Run_t::GetLocalTriggerStatus()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return false;
  }
  
  return (data->GetOdbSettings())->GetLocalTrig();
}

unsigned int Run_t::GetGroupMultiplicity()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return (unsigned int)0;
  }
  
  return (data->GetOdbSettings())->GetGrpMultiplicity();
}

bool Run_t::GetGroupEnable(unsigned int groupID)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return false;
  }
  if (groupID >= MAX_NUMBER_GROUPS) { // oups buffer overflow ... 
    cout << "ERROR: maximum group ID for this version of the library is " << MAX_NUMBER_GROUPS << endl;
    
    return false;
  }
  
  return ((data->GetOdbSettings())->GetGrpEnable())[groupID];
}

bool Run_t::GetChannelEnable(unsigned int channelID)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return false;
  }
  if (channelID >= MAX_NUMBER_CHANNELS) { // oups buffer overflow ... 
    cout << "ERROR: maximum channel ID for this version of the library is " << MAX_NUMBER_CHANNELS << endl;
    
    return false;
  }

  return ((data->GetOdbSettings())->GetChannelEnable())[channelID];
}

double * Run_t::GetTimeScale()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return NULL;
  }
  
  return timeScale;
}

int * Run_t::GetRawData()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return NULL;
  }
  
  return rawData;
}

double * Run_t::GetScaledData()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return NULL;
  }
  
  return scaledData;
}

double * Run_t::GetCalibratedData()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return NULL;
  }
  
  return calibratedData;
}

double * Run_t::GetFrequencySpectrum()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return NULL;
  }
  
  return frequencySpectrum;
}

double * Run_t::GetFilterShape()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return NULL;
  }
  
  return butterworthFilter;
}

unsigned int Run_t::GetCurrentEventID()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
  }
  
  return currentEventID;
}

unsigned int Run_t::GetCurrentSerialChannelNumber()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
  }
  
  return currentSerialChannelNumber;
}

double Run_t::GetLowPrecisionEventTime()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 0.0;
  }
  
  return (double)(midasTimeStamp);
}

double Run_t::GetHighPrecisionEventRelativeTime()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 0.0;
  }
  
  return (double)(caenTimeStamp & (unsigned int)0x7FFFFFFF) / GetSamplingFrequency() / 1000000.0;
}

int Run_t::SetCurrentData(unsigned int eventID, unsigned int serialChannelNumber)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 1;
  }
  if (eventID >= GetNumberOfEvents()) { // out of bounds ... 
    cout << "ERROR: you requested " << eventID << "-th event when there are only " << GetNumberOfEvents() << " events available in this run!?!" << endl;
    
    return 1;
  }
  if (serialChannelNumber >= GetNumberOfChannels()) { // out of bounds ...
    cout << "ERROR: you requested " << serialChannelNumber << "-th channel ID when there are only " << GetNumberOfChannels() << " channels available!?!" << endl;
    
    return 1;
  }
  if (currentEventID != eventID) { // if we are not looking for the some event as loaded
    currentEventID = eventID; // set the new ID and ...
    t->GetEntry(currentEventID); // push the new data in the buffer
    midasTimeStamp = (data->GetMidasHeader())->GetTimeStamp(); // fetch the Midas time stamp
    caenTimeStamp = (data->GetCaenHeader())->GetTriggerTimeTag(); // fetch the CAEN time stamp
  }
  currentSerialChannelNumber = serialChannelNumber; // set the serial channel number
  int n_samples = (int)(GetNumberOfSamples());
  double scaledOffset = GetInputRange(GetGroupID(currentSerialChannelNumber / NUMBER_CHANNELS_PER_GROUP));
  double calibratedFactor = GetCalibrationFactor(GetChannelID(currentSerialChannelNumber));
  double calibratedTerm = GetCalibrationTerm(GetChannelID(currentSerialChannelNumber));
  
  TSQLResult *res;
  TSQLRow *row;
  
  for (int i = 0; i < n_samples; i++) {
    rawData[i] = (((Channel_t *)((data->GetChannel())->At(currentSerialChannelNumber)))->GetData())->GetAt(i); // fetch raw sample
    scaledData[i] = (double)(rawData[i]) / 4096.0 * 2000.0 + scaledOffset; // compute scaled sample
    double val = (double)rawData[i];
    if (fUseCalibDB && serialChannelNumber < 63) {
      if (!serv) {
        std::cout << "No connection to the database. Using calibration parameters from MIDAS." << std::endl;
        calibratedData[i] = ((double)(rawData[i]) * calibratedFactor + calibratedTerm) * 1000.0;
      }
      else {
        calibratedData[i] = (pow(val,4)*CalibValues[serialChannelNumber][4] + pow(val,3)*CalibValues[serialChannelNumber][3] + pow(val,2)*CalibValues[serialChannelNumber][2] + val*CalibValues[serialChannelNumber][1] + CalibValues[serialChannelNumber][0]) * 1000.0;
      }
    }
    else {calibratedData[i] = ((double)(rawData[i]) * calibratedFactor + calibratedTerm) * 1000.0;} // compute calibrated sample in [nA] (calibration in [uA])
  }
  
  // all done ...
  return 0;
}

void Run_t::GetMinMaxRawData(int * min, int * max, bool init)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return;
  }
  
  int lMin, lMax;
  if (init) {
    lMin = * min;
    lMax = * max;
  }
  else {
    lMin = 4096;
    lMax = 0;
  }

  if (GetChannelEnable(currentSerialChannelNumber)) {
    unsigned int n_samples = GetNumberOfSamples();
    for (unsigned int i = (unsigned int)0; i < n_samples; i++) {
      if (rawData[i] < lMin) {
        lMin = rawData[i];
      }
      if (rawData[i] > lMax) {
        lMax = rawData[i];
      }
    }
  }
  * min = lMin;
  * max = lMax;
  
  return;
}

void Run_t::GetMinMaxScaledData(double * min, double * max, bool init)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return;
  }

  double lMin, lMax;
  if (init) {
    lMin = * min;
    lMax = * max;
  }
  else {
    lMin = 1000.0;
    lMax = -1000.0;
  }

  if (GetChannelEnable(currentSerialChannelNumber)) {
    unsigned int n_samples = GetNumberOfSamples();
    for (unsigned int i = (unsigned int)0; i < n_samples; i++) {
      if (scaledData[i] < lMin) {
        lMin = scaledData[i];
      }
      if (scaledData[i] > lMax) {
        lMax = scaledData[i];
      }
    }
  }
  * min = lMin;
  * max = lMax;
  
  return;
}

void Run_t::GetMinMaxCalibratedData(double * min, double * max, bool init)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return;
  }

  double lMin, lMax;
  if (init) {
    lMin = * min;
    lMax = * max;
  }
  else {
    lMin = 1000000000.0;
    lMax = -1000000000.0;
  }

  if (GetChannelEnable(currentSerialChannelNumber)) {
    unsigned int n_samples = GetNumberOfSamples();
    for (unsigned int i = (unsigned int)0; i < n_samples; i++) {
      if (calibratedData[i] < lMin) {
        lMin = calibratedData[i];
      }
      if (calibratedData[i] > lMax) {
        lMax = calibratedData[i];
      }
    }
  }
  * min = lMin;
  * max = lMax;
  
  return;
}

int Run_t::GetNoiseSubtractedIntegratedSignal(double *totalCharge, unsigned int binScalingFactor, double thresholdInSigma, unsigned int gridCH)
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return 0;
  }

  unsigned int n_samples = GetNumberOfSamples();

  // if channel is not enabled return NULL array
  if(!GetChannelEnable(currentSerialChannelNumber)) {
     for (int i = 0; i < n_samples; i++) {calibratedData[i] = 0.0;}

     return 0;
  }

  n_samples--; // there is one less for the charge "samples"
  // get the duration of a bin
  double dt = 1.0 / GetSamplingFrequency(); // sampling frequency in MHz -> dt in [us]
  // allocate memory ... and get calibrated data handle
  double * intData = new double[n_samples];
  for (unsigned int i = 0; i < n_samples; i++) {
    intData[i] = (calibratedData[i] + calibratedData[i + 1]) / 2.0 * dt; // charge "samples" are in [fC]
    if (currentSerialChannelNumber == gridCH) {intData[i] *= -1;} // invert grid signal to proceed with same algorithm as for negative signals
  }

  // get min, max of the calibrated data
  double min, max;
  GetMinMaxCalibratedData(&min, &max, false);
  max *= dt;
  min *= dt;

  max = max + (max - min)/20; // increase maximum of histogram range in order to be able to fit signals with large signal/noise ratio

  // determine number of bins in the histogram
  if (binScalingFactor == (unsigned int)0) {
    binScalingFactor = (unsigned int)25; // set default value if no value was handled
    cout << "calling the function Run_t::GetNoiseSubtractedIntegratedSignal with parameter binScalingFactor equal zero does not make any sense, value changed to default" << endl;
  }
  unsigned int n_bins = n_samples / binScalingFactor;

  // build the histogram ...
  TH1I * h1 = new TH1I("charge", "histogram of the charge", n_bins, min, max);
  h1->FillN(n_samples, intData, NULL); // fill the histogram with the charge "samples"

  // create TSpectrum object to find peaks
  TSpectrum * spec = new TSpectrum();
  spec->Search(h1, 2, "goff"); // search the peaks
  unsigned int n_peaks = spec->GetNPeaks();

  Float_t * peakPositions = spec->GetPositionX(); // get handle on the positions of the peaks

  double maxPeak = 0.0;
  double meanPeak = (max-min)/2.0;
  double currentHistValue;
  double binSize = (max-min)/n_bins;
  for (unsigned int i = 0; i < n_peaks; i++) { // find largest peak and set it as mean value of the gaussian
     currentHistValue = h1->GetBinContent(int((peakPositions[i]-min)/binSize));
     if (currentHistValue >= maxPeak) {
        maxPeak = currentHistValue;
        meanPeak = peakPositions[i];
     }
  }

  double histoMinSignalArea;
  double histoMaxSignalArea;

  if (n_peaks == 0) {histoMinSignalArea = min; histoMaxSignalArea = max;} // set whole histogram range if no peaks were found
  else {if ((max - meanPeak) > (max - min)/2) {histoMinSignalArea = min; histoMaxSignalArea = 2*meanPeak - min;} // the peak is at left boundary
        else {histoMinSignalArea = 2*meanPeak - max; histoMaxSignalArea = max;} // the peak is at right boundary
  }
  if (histoMinSignalArea < min) {histoMinSignalArea = min;} // set min as minimum fit range if out of bounds
  if (histoMaxSignalArea > max) {histoMaxSignalArea = max;} // set max as maximum fit range if out of bounds
  
  // fit the peak with a gaussian
  TF1 * fitFunction = new TF1("fitFunction", "gaus", histoMinSignalArea, histoMaxSignalArea);
  fitFunction->SetParameter(1, meanPeak);
  h1->Fit("fitFunction", "Rqn");
  double meanNoise = fitFunction->GetParameter(1); // get mean noise level
  double thresholdNoise = meanNoise - thresholdInSigma*fitFunction->GetParameter(2); // get signal threshold

  // extract data without noise
  for (unsigned int i = 0; i < n_samples; i++) {
    if (intData[i] < thresholdNoise) {
       if (currentSerialChannelNumber == gridCH) {calibratedData[i] = -1 * (intData[i] - meanNoise);} // undo grid signal inversion
       else {calibratedData[i] = intData[i] - meanNoise;}
       * totalCharge += calibratedData[i]; // total charge for current channel
    }
    else {calibratedData[i] = 0;}
  }

  // set last sample of calibrated data equal 0
  calibratedData[n_samples] = 0;

  // clean up memory
  delete [] intData;
  delete fitFunction;
  delete spec;
  delete h1;

  // done ...
  return 0;
}

double Run_t::SubtractBaseline()
{
  if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;

    return 0;
  }

  unsigned int n_samples = GetNumberOfSamples();

  // get min, max of the calibrated data
  double min, max;
  GetMinMaxCalibratedData(&min, &max, false);

  max = max + (max - min)/20; // increase maximum of histogram range in order to be able to fit signals with large signal/noise ratio

  // determine number of bins in the histogram
  unsigned int binScalingFactor = (unsigned int)20; // set default value
  unsigned int n_bins = n_samples / binScalingFactor;

  // build the histogram ...
  TH1I * h1 = new TH1I("calibrated data", "histogram of the calibrated data", n_bins, min, max);
  h1->FillN(n_samples, calibratedData, NULL); // fill the histogram with the charge "samples"

  // create TSpectrum object to find peaks
  TSpectrum * spec = new TSpectrum();
  spec->Search(h1, 2, "goff"); // search the peaks
  unsigned int n_peaks = spec->GetNPeaks();

  Float_t * peakPositions = spec->GetPositionX(); // get handle on the positions of the peaks

  double maxPeak = 0.0;
  double meanPeak = (max-min)/2.0;
  double currentHistValue;
  double binSize = (max-min)/n_bins;
  for (unsigned int i = 0; i < n_peaks; i++) { // find largest peak and set it as mean value of the gaussian
     currentHistValue = h1->GetBinContent(int((peakPositions[i]-min)/binSize));
     if (currentHistValue >= maxPeak) {
        maxPeak = currentHistValue;
        meanPeak = peakPositions[i];
     }
  }

  double histoMinSignalArea;
  double histoMaxSignalArea;

  if (n_peaks == 0) {histoMinSignalArea = min; histoMaxSignalArea = max;} // set whole histogram range if no peaks were found
  else {if ((max - meanPeak) > (max - min)/2) {histoMinSignalArea = min; histoMaxSignalArea = 2*meanPeak - min;} // the peak is at left boundary
        else {histoMinSignalArea = 2*meanPeak - max; histoMaxSignalArea = max;} // the peak is at right boundary
  }
  if (histoMinSignalArea < min) {histoMinSignalArea = min;} // set min as minimum fit range if out of bounds
  if (histoMaxSignalArea > max) {histoMaxSignalArea = max;} // set max as maximum fit range if out of bounds
  
  // fit the peak with a gaussian
  TF1 * fitFunction = new TF1("fitFunction", "gaus", histoMinSignalArea, histoMaxSignalArea);
  fitFunction->SetParameter(1, meanPeak);
  h1->Fit("fitFunction", "Rqn");

  double baseline = fitFunction->GetParameter(1);

  // update data
  for (int i = 0; i < n_samples; i++) {
     calibratedData[i] = calibratedData[i] - baseline;
  }

  return baseline;
}

double * Run_t::GetFFTData(double *data, int FilterOpt, double f1, double f2, int n)
{
  double pi = 3.141592653589793;
  std::complex<double> j = std::complex<double>(0.0,1.0);

  // get number of samples
  unsigned int n_samples = GetNumberOfSamples();

  // get sampling frequency
  double fs = GetSamplingFrequency();
  fs = fs*pow(10.0,6); // sampling frequency is in MHz

  // get next power of 2
  int nFFT = ceil(log(n_samples)/log(2));

  // number of FFT samples
  int nrFFT = pow(2.0, nFFT);

  // define input data
  double *fftDataINre = new double[nrFFT];
  double *fftDataINim = new double[nrFFT];

  // fill up with zeros
  for (int i = 0; i < nrFFT; i++) {
     if (i < n_samples) {
        fftDataINre[i] = data[i];
        fftDataINim[i] = 0;
     }
     else {
        fftDataINre[i] = 0;
        fftDataINim[i] = 0;
     }
  }

  // new FFT object for complex forward transformation
  TVirtualFFT *fftc2cFW = TVirtualFFT::FFT(1, &nrFFT, "C2CFORWARD");

  // set input data of forward transformation
  fftc2cFW->SetPointsComplex(fftDataINre, fftDataINim);

  // execute forward transformation
  fftc2cFW->Transform();

  // get FFT data
  double *fftDataOUTre = new double[nrFFT];
  double *fftDataOUTim = new double[nrFFT];

  fftc2cFW->GetPointsComplex(fftDataOUTre, fftDataOUTim);

  // frequency spectrum and axis
  double *f = new double[nrFFT];
  std::complex<double> *c = new std::complex<double>[nrFFT];
  
  double stp = 1. / nrFFT;
  for (int i = 0; i < nrFFT; i++) {
     c[i] = std::complex<double>(fftDataOUTre[i],fftDataOUTim[i]) / std::complex<double>(nrFFT,0.0);
     f[i] = fs*i*stp;
  }

  // generate transfer function
  double w1 = 2*pi*f1;
  double w2 = 2*pi*f2;
  double *a = new double[n-1]; // Butterworth coefficients
  a = butter(n);

  std::complex<double> *H = new std::complex<double>[nrFFT];
  std::complex<double> *G = new std::complex<double>[nrFFT];
  double *fftDataMODre = new double[nrFFT];
  double *fftDataMODim = new double[nrFFT];

  // point i=0, i.e. f=0 Hz
  //H[0] = std::complex<double>(1.0,0.0);
  H[0] = std::complex<double>(0.0,0.0);
  G[0] = H[0] * c[0];
  fftDataMODre[0] = real(G[0]);
  fftDataMODim[0] = imag(G[0]);
  butterworthFilter[0] = abs(H[0]);
  frequencySpectrum[0] = abs(G[0]);

  // all other points
  for (int i = 1; i < nrFFT; i++){
     std::complex<double> B(0,0);
     std::complex<double> s;
     switch(FilterOpt) {
        case 0:
           s = sqrt(w1*w2) / (w2-w1) * (j*std::complex<double>(2.0,0.0)*pi*f[i] / sqrt(w1*w2) + sqrt(w1*w2) / (j*std::complex<double>(2.0,0.0)*pi*f[i]));
           break;
        case 1:
           s = 1.0 / (sqrt(w1*w2) / (w2-w1) * (j*std::complex<double>(2.0,0.0)*pi*f[i] / sqrt(w1*w2) + sqrt(w1*w2) / (j*std::complex<double>(2.0,0.0)*pi*f[i])));
           break;
     }
     B = B + std::complex<double>(1.0,0.0);
     for (int k = 0; k < n-1; k++) {
        B = B + a[k]*pow(s,k+1);
     }
     B = B + pow(s,n);
     H[i] = 1.0 / B;
     G[i] = H[i] * c[i];
     fftDataMODre[i] = real(G[i]);
     fftDataMODim[i] = imag(G[i]);
     butterworthFilter[i] = abs(H[i]);
     frequencySpectrum[i] = abs(G[i]);
  }

  // new FFT object for complex backward transformation
  TVirtualFFT *fftc2cBW = TVirtualFFT::FFT(1, &nrFFT, "C2CBACKWARD");

  // set input data of backward transfomration
  fftc2cBW->SetPointsComplex(fftDataMODre, fftDataMODim);

  // execute backward transformation
  fftc2cBW->Transform();

  // get inverse FFT data
  double *INVfftDatare = new double[nrFFT];
  double *INVfftDataim = new double[nrFFT];

  fftc2cBW->GetPointsComplex(INVfftDatare, INVfftDataim);

  std::complex<double> *ci = new std::complex<double>[n_samples];
  double *iFFTdata = new double[n_samples];
  for (int i = 0; i < n_samples; i++) {
     ci[i] = std::complex<double>(INVfftDatare[i],INVfftDataim[i]);
     iFFTdata[i] = real(ci[i]);
  }

  // clean up
  delete [] fftDataINre;
  delete [] fftDataINim;
  delete [] fftDataOUTre;
  delete [] fftDataOUTim;
  delete [] fftDataMODre;
  delete [] fftDataMODim;
  delete [] INVfftDatare;
  delete [] INVfftDataim;
  delete [] H;
  delete [] G;
  delete [] c;
  delete [] ci;
  delete [] f;
  delete fftc2cFW;
  delete fftc2cBW;

  return iFFTdata;
}

int Run_t::ApplyFilter(int FilterOpt, double f1, double f2, int n)
{
  // convert int to double in rawData
  unsigned int n_samples = GetNumberOfSamples();
  double * rawDataTMP = new double[n_samples];
  for (unsigned int i = 0; i < n_samples; i++){
     rawDataTMP[i] = rawData[i];
  }

  // do the filtering
  double * filteredRawData = GetFFTData(rawDataTMP, FilterOpt, f1, f2, n);
  double * filteredScaledData = GetFFTData(scaledData, FilterOpt, f1, f2, n);
  double * filteredCalibratedData = GetFFTData(calibratedData, FilterOpt, f1, f2, n);

  // update data
  for (unsigned int i = 0; i < n_samples; i++){
     rawData[i] = int(filteredRawData[i]);
     scaledData[i] =  filteredScaledData[i];
     calibratedData[i] = filteredCalibratedData[i];
  }

  // clean up
  delete [] rawDataTMP;
  delete [] filteredRawData;
  delete [] filteredScaledData;
  delete [] filteredCalibratedData;

  // done ...
  return 0;
}

int Run_t::GetSpectrum()
{
  double * data = GetCalibratedData();

  // get number of samples
  unsigned int n_samples = GetNumberOfSamples();

  // get sampling frequency
  double fs = GetSamplingFrequency();
  fs = fs*pow(10.0,6); // sampling frequency is in MHz

  // get next power of 2
  int nFFT = ceil(log(n_samples)/log(2));

  // number of FFT samples
  int nrFFT = pow(2.0, nFFT);

  // define input data
  double *fftDataINre = new double[nrFFT];
  double *fftDataINim = new double[nrFFT];

  // fill up with zeros
  for (int i = 0; i < nrFFT; i++) {
     if (i < n_samples) {
        fftDataINre[i] = data[i];
        fftDataINim[i] = 0;
     }
     else {
        fftDataINre[i] = 0;
        fftDataINim[i] = 0;
     }
  }

  // new FFT object for complex forward transformation
  TVirtualFFT *fftc2cFW = TVirtualFFT::FFT(1, &nrFFT, "C2CFORWARD");

  // set input data of forward transformation
  fftc2cFW->SetPointsComplex(fftDataINre, fftDataINim);

  // execute forward transformation
  fftc2cFW->Transform();

  // get FFT data
  double *fftDataOUTre = new double[nrFFT];
  double *fftDataOUTim = new double[nrFFT];

  fftc2cFW->GetPointsComplex(fftDataOUTre, fftDataOUTim);

  // frequency spectrum
  std::complex<double> *c = new std::complex<double>[nrFFT];

  for (int i = 0; i < nrFFT; i++) {
     c[i] = std::complex<double>(fftDataOUTre[i],fftDataOUTim[i]) / std::complex<double>(nrFFT,0.0);
     frequencySpectrum[i] = abs(c[i]);
  }

  // clean up
  //delete data;
  delete [] fftDataINre;
  delete [] fftDataINim;
  delete [] fftDataOUTre;
  delete [] fftDataOUTim;
  delete [] c;
  delete fftc2cFW;

  return 0;
}

double * Run_t::butter(int n)
{
  // coefficient matrix
  double a[14][14] = {{1.414214,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.},
                    {2.,2.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.},
                    {2.613126,3.414214,2.613126,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.},
                    {3.236068,5.236068,5.236068,3.236068,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.},
                    {3.863703,7.464102,9.141620,7.464102,3.863703,0.,0.,0.,0.,0.,0.,0.,0.,0.},
                    {4.493959,10.097835,14.591794,14.591794,10.097835,4.493959,0.,0.,0.,0.,0.,0.,0.,0.},
                    {5.125831,13.137071,21.846151,25.688356,21.846151,13.137071,5.125831,0.,0.,0.,0.,0.,0.,0.},
                    {5.758770,16.581719,31.163437,41.986386,41.986386,31.163437,16.581719,5.758770,0.,0.,0.,0.,0.,0.},
                    {6.392453,20.431729,42.802061,64.882396,74.233429,64.882396,42.802061,20.431729,6.392453,0.,0.,0.,0.,0.},
                    {7.0267,24.6871,57.0203,95.9370,123.2435,123.2435,95.9370,57.0203,24.6871,7.0267,0.,0.,0.,0.},
                    {7.6613,29.3477,74.0762,136.8750,194.7187,218.4687,194.7187,136.8750,74.0762,29.3477,7.6613,0.,0.,0.},
                    {8.2962,34.4137,94.2280,189.5854,295.5113,366.7511,366.7511,295.5113,189.5854,94.2280,34.4137,8.2962,0.,0.},
                    {8.9314,39.8850,117.7337,256.1214,433.7284,589.0206,651.2664,589.0206,433.7284,256.1214,117.7337,39.8850,8.9314,0.},
                    {9.6,45.8,144.9,338.7,618.8,911.8,1102.4,1102.4,911.8,618.8,338.7,144.9,45.8,9.6}};

  // generate coefficient array for n-th order butterworth filter
  double *coeff = new double[n-1];
  for (int i = 0; i < n-1; i++) {
     coeff[i] = a[n-2][i];
  }

  return coeff;
}

int * Run_t::GetChannelMapping(int *mappingID, int DchMap)
{
   if (RunNotLoaded()) {
    cout << "no data available, maybe you forgot to open a run?" << endl;
    
    return NULL;
  }

  // get date of current loaded run
  char rDate[80];
  int DateOfRun;

  // run date
  time_t rd = time_t(GetLowPrecisionEventTime());
  // convert unix time
  struct tm* gmt = gmtime(&rd);
  strftime(rDate,80,"%Y%m%d",gmt);
  DateOfRun = atoi(rDate);

  // connect to SQL server to retrieve channel mapping
  if (!serv) {serv = TSQLServer::Connect("mysql://lheppc83.unibe.ch:3306", "exodaq", "exosql");}

  int *chMapping = new int[62];
  char sqlQuery[80];
  TSQLResult *res;
  TSQLRow *row;
  int nRows = 0;
  int rowDate = 0;
 
  // find channel mapping for current run
  if (serv) {
     serv->SelectDataBase("exo");

     if (DchMap == 1 ) {sprintf(sqlQuery, "select date from chMapping order by date desc");}
     else if (DchMap == 2 ) {sprintf(sqlQuery, "select date from EXOChMap_Coll order by date desc");}
          
     res = serv->Query(sqlQuery);
     row = res->Next();
     nRows = res->GetRowCount();
     rowDate = atoi(row->GetField(0));
     int k = 1;
     while (DateOfRun < rowDate && k < nRows) {
        row = res->Next();
        rowDate = atoi(row->GetField(0));
        k++;
     }

     if (DchMap ==1 ) {sprintf(sqlQuery, "select * from chMapping where date = '%i'",rowDate);}
     else if (DchMap ==2 ) {sprintf(sqlQuery, "select * from EXOChMap_Coll where date = '%i'",rowDate);}
     
     res = serv->Query(sqlQuery);
     row = res->Next();

     // assign VME channel to micromegas channel
     for (int i = 0; i < 62; i++) {
        chMapping[i] = atoi(row->GetField(i+1));
     }
  }
  else {
     cout << "Could not connect to SQL server! Using default channel mapping." << endl;

     // assign VME channel to micromegas channel (default)
     for (int i = 0; i < 62; i++) {
        chMapping[i] = i+1;
     }
  }

  * mappingID = rowDate;

  return chMapping;
}

double Run_t::GetDriftVelocity(int gasID)
{
  if (RunNotLoaded()) {
    cout << "Can not set drift velocity, there is no run loaded!" << endl;

    return 0;
  }

  const char * runID = currentRunName.c_str();
  char * installDir = getenv("READMIDASSYS");
  char * dvFile;
  if (gasID == 0) {dvFile = "driftVelocityP10.txt";}
  else if (gasID == 1) {dvFile = "driftVelocityXe98CF42.txt";}
  else if (gasID == 2) {dvFile = "driftVelocityLCF4.txt";}
  else {if (gasID == 3) {dvFile = "driftVelocityLXe.txt";}
        else {cout << "gasID must be 0(P10), 1(Xe98CF42), 2(LCF4) or 3(LXe) You have chosen " << gasID << endl; return 0;}
  }

  char fname[100];
  sprintf(fname,"%s/data/%s", installDir, dvFile);

  // connect to SQL server to retrieve values in database
  if (!serv) {serv = TSQLServer::Connect("mysql://lheppc83.unibe.ch:3306", "exodaq", "exosql");}

  char sqlQuery[80];
  TSQLResult *res;
  TSQLRow *row;
  double pressure = 0.0;
  double grid = 0.0;
  double firstRing = 0.0;
  double driftVelocity = 0.0;

  if (serv) {
     serv->SelectDataBase("exo");
     sprintf(sqlQuery, "select * from runlist where runNumber = '%s'",runID);
     res = serv->Query(sqlQuery);
     row = res->Next();
     if (row->GetField(4)) {pressure = atof(row->GetField(4));}
     if (row->GetField(5)) {grid = atof(row->GetField(5));}
     if (row->GetField(7)) {firstRing = atof(row->GetField(7));}

     // calculate drift velocity
     if (pressure > 0.0 && TMath::Abs(grid) > 0.0 && TMath::Abs(firstRing) > 0.0) {
        double E = (TMath::Abs(firstRing) - TMath::Abs(grid)) / pressure;

        // read file with drift velocity for current pressure and gas
        ifstream driftVelocityFile;
        driftVelocityFile.open(fname, ios::in);

        // get drift velocities in file
        string currentLine;
        vector<double> EValues;
        vector<double> dvValues;
        while (driftVelocityFile >> currentLine) {
           int cutAt = currentLine.find_first_of(";");
           EValues.push_back(atof(currentLine.substr(0, cutAt).c_str()));
           dvValues.push_back(atof(currentLine.substr(cutAt + 1).c_str()));
        }

        // number of entries in file
        int nvalues = EValues.size();

        // find position of current E field in vector
        int i = 0;
        int k = nvalues - 1;
        while (i < nvalues && EValues.at(i) < E) {i++;}
        while (k >= 0 && EValues.at(k) > E) {k--;}
        if (k != i) {i--; k++;}

        if (i < nvalues && i >= 0 && k < nvalues && k >= 0) {driftVelocity = (dvValues.at(k) + dvValues.at(i)) / 2;}
        else {
           cout << "Failed to calculate dritf velocity. Check database entry for this run. Using default value." << endl;
           driftVelocity = 4.7;
        }
     }
     else {
        cout << "Failed to calculate dritf velocity. Check database entry for this run. Using default value." << endl;
        driftVelocity = 4.7;
     }
  }
  else {
     cout << "Could not connect to SQL server! Using default drift velocity." << endl;
     driftVelocity = 4.7;
  }

  return driftVelocity;
}

void Run_t::UseCalibDB(bool use)
{
  TSQLResult *res;
  TSQLRow *row;
  
  if (use) {
    if (!serv) {serv = TSQLServer::Connect("mysql://lheppc83.unibe.ch:3306", "exodaq", "exosql");}
    serv->SelectDataBase("exo");
    
    for (int i = 0; i < 63; i++) {
      res = serv->Query(Form("select * from calibration where channel=%i",i));
      row = res->Next();
      
      CalibValues[i][0] = atof(row->GetField(2));
      CalibValues[i][1] = atof(row->GetField(3));
      CalibValues[i][2] = atof(row->GetField(4));
      CalibValues[i][3] = atof(row->GetField(5));
      CalibValues[i][4] = atof(row->GetField(6));
    }
    fUseCalibDB = true;
  }
  else {fUseCalibDB = false;}
  
  return;
}

void Run_t::GetCalibrationValues(int chID)
{
  TSQLResult *res;
  TSQLRow *row;
  
  if (!serv) {
    std::cout << "No connection to the database. Using calibration parameters from MIDAS." << std::endl;
  }
  else {
    serv->SelectDataBase("exo");
    res = serv->Query(Form("select * from calibration where channel=%i",chID));
    row = res->Next();
    std::cout << "CH = " << chID << "\t p0 = " << row->GetField(2) << "\t p1 = " << row->GetField(3) << "\t p2 = " << row->GetField(4) << "\t p3 = " << row->GetField(5) << "\t p4 = " << row->GetField(6) << std::endl;
  }
}
