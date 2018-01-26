#include "mid2root.h"

int main(int argc, char* argv[])
{
  evt=0;

  int i=0,size=0;
  bool RootStruct=0;
  ievt=0; // event ID
  CaenID_old = -1;

  if (argc!=2)
    {
      cout << "Usage:" << endl;
      cout << "mid2root filename.mid" << endl;
      return -1;
    }
  
  cout << " Convert Midas in Root (v052110)" << endl;

  string fname  = argv[1];

  int lgh = fname.length();
  int loc = fname.find( ".mid", 0 );
  int diff = lgh-loc;

  if( diff != 4 )
    {
      cout << endl << "  Wrong file format : must be *.mid   !!!" <<  endl << endl;
      return -1;
    }

  TMidasFile f;
  if (!f.Open(fname.c_str()))
    {
      cout << "File " << fname.c_str() << " doesn't exist !" << endl;
      return -1;
    }


  // OUTPUT FILE
  // file name
  int limit= fname.find(".mid");
  string fname2 = fname.substr(0,limit);
  fname2 += ".root";
  
  // tree name
  string treen = fname.substr(0,8);

  fout = new TFile(fname2.c_str(),"RECREATE");
  tree = new TTree("t",treen.c_str());
 
  cout << "Input file  : " << fname << endl;
  cout << "Output file : " << fname2 << endl;

  // loop on all the event
  while (1)
    {
      evt = new Data_t();

      TMidasEvent event;  
      if (!f.Read(&event))
	break;
      
      MidasHeader.event_id     = event.GetEventId();
      MidasHeader.trigger_mask = event.GetTriggerMask();
      MidasHeader.serial_nb    = event.GetSerialNumber();
      MidasHeader.data_size    = event.GetDataSize();
      //       MidasHeader.bank_list    = event.GetBankList();

      if ((MidasHeader.event_id & 0xFFFF) == 0x8000)
	{
	  MidasHeader.time_stamp   = event.GetTimeStamp();
	  duration = event.GetTimeStamp();
 	  ReadOdb(&event);
	}
      else if ((MidasHeader.event_id & 0xFFFF) == 0x8001)
	{ 
	  tree->Branch("duration",&duration);
	  MidasHeader.time_stamp   = event.GetTimeStamp();
	  duration =  event.GetTimeStamp() - duration;
	  tree->Fill();
	}
      else
        {
	  MidasHeader.time_stamp   = event.GetTimeStamp();
	  
	  event.SetBankList();
	  
	  void *pt = NULL;
	  size = event.LocateBank(NULL, "CAEN", &pt);
	  
	  if(!ReadCaen(pt,size,RootStruct))
	    return -1;

	  RootStruct=1;
// 	  break;
	} 
      ievt=i;
      i++;
    }
  f.Close();
  fout->Write();
  fout->Close();

  cout << "Nb of Event : " << nentries << endl;
  cout << "---> Done" << endl;

  return 0;
}


int ReadOdb(TMidasEvent *event)
{
  VirtualOdb* gOdb = NULL;
  XmlOdb* odb = new XmlOdb(event->GetData(),event->GetDataSize());
  gOdb = odb;
  
  int size=0;

  Settings.mini_freq         = gOdb->odbReadDouble("/Equipment/CAEN_V1740/Settings/Minimal frequency in MHz");
  Settings.mini_duration     = gOdb->odbReadDouble("/Equipment/CAEN_V1740/Settings/Minimal duration in us");
  Settings.global_range_min  = gOdb->odbReadDouble("/Equipment/CAEN_V1740/Settings/Global range minimum in mV");

  size = gOdb->odbReadArraySize("/Equipment/CAEN_V1740/Settings/Group range minimum in mV");
  for (int i=0;i<size;i++)
    Settings.group_range_min[i] = gOdb->odbReadDouble("/Equipment/CAEN_V1740/Settings/Group range minimum in mV",i);

  Settings.global_trig_thres = gOdb->odbReadDouble("/Equipment/CAEN_V1740/Settings/Global trigger threshold in mV");

  size = gOdb->odbReadArraySize("/Equipment/CAEN_V1740/Settings/Group trigger threshold in mV");
  for (int i=0;i<size;i++)
    Settings.group_trig_thres[i] = gOdb->odbReadDouble("/Equipment/CAEN_V1740/Settings/Group trigger threshold in mV",i);

  size = gOdb->odbReadArraySize("/Equipment/CAEN_V1740/Settings/Calibration slope in a.u.");
  for (int i=0;i<size;i++)
    Settings.calib_slope[i] = gOdb->odbReadDouble("/Equipment/CAEN_V1740/Settings/Calibration slope in a.u.",i);
  
  size = gOdb->odbReadArraySize("/Equipment/CAEN_V1740/Settings/Calibration offset in a.u.");
  for (int i=0;i<size;i++)
    Settings.calib_offset[i] = gOdb->odbReadDouble("/Equipment/CAEN_V1740/Settings/Calibration offset in a.u.",i);
  
  Settings.post_trigger      = gOdb->odbReadUint32("/Equipment/CAEN_V1740/Settings/Post trigger in %");
  Settings.group_muliplicity = gOdb->odbReadUint32("/Equipment/CAEN_V1740/Settings/Group multiplicity");
  
  size = gOdb->odbReadArraySize("/Equipment/CAEN_V1740/Settings/Group enable");
  for (int i=0;i<size;i++)
    Settings.group_enable[i] = gOdb->odbReadBool("/Equipment/CAEN_V1740/Settings/Group enable",i);

  size = gOdb->odbReadArraySize("/Equipment/CAEN_V1740/Settings/Channel enable");
  for (int i=0;i<size;i++)
      Settings.channel_enable[i] = gOdb->odbReadBool("/Equipment/CAEN_V1740/Settings/Channel enable",i);

  Settings.ext_trig          = gOdb->odbReadBool("/Equipment/CAEN_V1740/Settings/External trigger enable");
  Settings.local_trig        = gOdb->odbReadBool("/Equipment/CAEN_V1740/Settings/Local trigger enable");

  return 1;
}


int ReadCaen(void *pt,const int size,bool RootStruct)
{
  // Subroutine to read Caen header of the current event
  unsigned int * ptr = (unsigned int *)pt;
  nb_group=0;
 
  // Midas Header
  evt->SetMidasHeader(MidasHeader.event_id,MidasHeader.trigger_mask,MidasHeader.serial_nb,MidasHeader.time_stamp,MidasHeader.data_size); 
  //   cout << "Read Event Header ..." << endl;
  
  /* Header 1 */
  CaenHeader.caenID            = ( (ptr[0]) & (unsigned int)0xF0000000 ) >> 28;
  CaenHeader.eventSize         = ( (ptr[0]) & (unsigned int)0x0FFFFFFF );
  
  /* Header 2 */
  CaenHeader.boardID           = ( (ptr[1]) & (unsigned int)0xF8000000 ) >> 27;
  CaenHeader.res               = ( (ptr[1]) & (unsigned int)0x07000000 ) >> 24;
  CaenHeader.pattern           = ( (ptr[1]) & (unsigned int)0x00FFFF00 ) >> 8;
  CaenHeader.group_mask        = ( (ptr[1]) & (unsigned int)0x000000FF );
  
  /* Header 3 */
  CaenHeader.reserved          = ( (ptr[2]) & (unsigned int)0xFF000000 ) >> 24;
  CaenHeader.event_count       = ( (ptr[2]) & (unsigned int)0x00FFFFFF );
  
  /* Header 4 */
  CaenHeader.trigger_time_tag  = (ptr[3]);
  
  evt->SetCaenHeader(CaenHeader.caenID,CaenHeader.eventSize,CaenHeader.boardID,CaenHeader.res,CaenHeader.pattern,CaenHeader.group_mask,CaenHeader.reserved,CaenHeader.event_count,CaenHeader.trigger_time_tag);
  //   cout << "done" << endl;

  if (display)
    {
      printf("caenID            :   in deca %u  ;  in hex 0x%X  ;  ", CaenHeader.caenID, CaenHeader.caenID);
      printf("in binary ");int2bin(CaenHeader.caenID);
      printf("eventSize         :   in deca %u  ;  in hex 0x%X  ;  ", CaenHeader.eventSize, CaenHeader.eventSize);
      printf("in binary ");int2bin(CaenHeader.eventSize);
      printf("boardID           :   in deca %u  ;  in hex 0x%X  ; ", CaenHeader.boardID, CaenHeader.boardID); 
      printf("in binary ");int2bin(CaenHeader.boardID);
      printf("res               :   in deca %u  ;  in hex 0x%X  ; ", CaenHeader.res, CaenHeader.res); 
      printf("in binary ");int2bin(CaenHeader.res);
      printf("pattern           :   in deca %u  ;  in hex 0x%X  ; ", CaenHeader.pattern, CaenHeader.pattern);
      printf("in binary ");int2bin(CaenHeader.pattern);
      printf("group_mask        :   in deca %u  ;  in hex 0x%X  ; ", CaenHeader.group_mask, CaenHeader.group_mask);
      printf("in binary ");int2bin(CaenHeader.group_mask);
      printf("reserved          :   in deca %u  ;  in hex 0x%X  ; ", CaenHeader.reserved, CaenHeader.reserved); 
      printf("in binary ");int2bin(CaenHeader.reserved);
      printf("event_count       :   in deca %u  ;  in hex 0x%X  ; ", CaenHeader.event_count, CaenHeader.event_count); 
      printf("in binary ");int2bin(CaenHeader.event_count);
      printf("trigger_time_tag  :   in deca %u  ;  in hex 0x%X  ; ", CaenHeader.trigger_time_tag, CaenHeader.trigger_time_tag); 
      printf("in binary ");int2bin(CaenHeader.trigger_time_tag);
    }
  
  CaenID_old = CaenHeader.caenID;
  

  // READ  group_mask
  int b,j=0,i=0;
  unsigned int group_mask_tmp=CaenHeader.group_mask; 
  group_used = (int*) malloc(sizeof(1));
  

  while(group_mask_tmp != 0)
    {
      b = group_mask_tmp % 2;
      
      if (b)
	{
 	  group_used = (int*) realloc(group_used,(nb_group+1)*sizeof(int));
	  group_used[nb_group]=i;
	  nb_group++;
	}
      group_mask_tmp = group_mask_tmp / 2;
      i++;
    }
  
  UserHeader.nb_gr    = nb_group;
  UserHeader.nb_ch    = 8*nb_group;
  UserHeader.nb_dt    = CaenHeader.eventSize / (3*nb_group);
 
  evt->SetUserHeader(UserHeader.nb_gr,UserHeader.nb_dt);

  evt->SetOdbSettings(Settings.mini_freq,Settings.mini_duration,
		      Settings.global_range_min,
		      Settings.group_range_min,
		      Settings.global_trig_thres,
		      Settings.group_trig_thres,
		      Settings.calib_slope,
		      Settings.calib_offset,
		      Settings.post_trigger,
		      Settings.group_muliplicity,
		      Settings.group_enable,
		      Settings.channel_enable,
		      Settings.ext_trig,
		      Settings.local_trig);

  if (display)
    {
      printf("Nb of used group : %i   ( Group : ",nb_group);  
      for (i=0;i<nb_group;i++)
	printf(" %i ",group_used[i]);
      printf(")\n");
      
      printf("Nb of channel : %i\n",UserHeader.nb_ch);
      printf("Channel : ");
      
      for(i=0;i<nb_group;i++)
	{
	  for(j=0;j<8;j++)
	    {
	      printf(" %i",8*group_used[i]+j);
	    }
	}
      printf("\n");
      
      
      printf("Nb of dT : %i\n",UserHeader.nb_dt);
      //  printf("\n\n\n");
    }
  
  if (!RootStruct)
    CreateRootStruct();
  
  if(!ReadCaenEvent(ptr, size))
    return -1;
  
  return 0;
}



int ReadCaenEvent(void *pt, const int size)
{
  // Subroutine to get the data of the current event

//   cout << "Read data ..." << endl;

  unsigned int * ptr = (unsigned int *)pt;
  int n_samples = UserHeader.nb_dt * UserHeader.nb_ch;
  int n_samples_per_gr = n_samples / UserHeader.nb_gr;
  unsigned int j=0,k=0,s_id=0,ch_id=0;
  bool err = false;

  for (int i=0;i<n_samples;i++)
    {
      s_id = ((i % n_samples_per_gr) / 3) / 8 * 3 + ((i % n_samples_per_gr) % 3);
      ch_id = ((i % n_samples_per_gr) / 3) % 8 + (i / n_samples_per_gr) * 8;
      
      if (j+4 > size) {cout << "Warning: index exceeds array boundary --> omitting this event: " << ievt << endl; err = true; break;}

      switch(k)
 	{
	case 0:
 	  channel[ch_id][s_id] = ptr[j+4] & (unsigned int)0x00000FFF;
 	  k++;
	  break;
	  
	case 1:
	  channel[ch_id][s_id] = (ptr[j+4] & (unsigned int)0x00FFF000) >> 12;
 	  k++;
	  break;
	  
	case 2:
 	  channel[ch_id][s_id] = (( ptr[j+4] & (unsigned int)0xFF000000 ) >> 24) | ((( ptr[j+1+4] & (unsigned int)0x0000000F )) << 8);
 	  k=k+2;
	  j++;
	  break;
	  
	case 4:
	  channel[ch_id][s_id] = (ptr[j+4] & (unsigned int)0x0000FFF0) >> 4;
	  k++;
	  break;
	  
	case 5:
 	  channel[ch_id][s_id] = (ptr[j+4] & (unsigned int)0x0FFF0000) >> 16;
	  k++;
	  break;
	  
	case 6:
 	  channel[ch_id][s_id] = ((ptr[j+4] & (unsigned int)0xF0000000 ) >> 28) | (((ptr[j+1+4] & (unsigned int)0x000000FF )) << 4);
 	  k=k+2;
 	  j++;
	  break;
	  
	case 8:
 	  channel[ch_id][s_id] = (ptr[j+4] & (unsigned int)0x000FFF00) >> 8;
	  k++;
	  break;
	  
	case 9:
	  channel[ch_id][s_id] = (ptr[j+4] & (unsigned int)0xFFF00000) >> 20;
 	  j++;
	  k=0;
	  break;
	  
	default:
	  cout << "bordel de merde" << endl;
	  return -1;
	  break;
	}
    }
  
  if (err) {return 0;}

  Channel_t a[UserHeader.nb_ch];

  int r_ch=0;k=0;

  for (int j=0;j<UserHeader.nb_ch;j++)
    {
      k=(int)j/8;
      r_ch = j + 8*(group_used[k]-k);
      a[j].Init(group_used[k],r_ch,UserHeader.nb_dt);
      a[j].Data(channel[j]);
      evt->SetChannel(&a[j]);
    } 
  tree->Fill();
  nentries = tree->GetEntries();

  return 0;
}




void int2bin(unsigned int a)
{
  // subroutine to convert an integer into binary
  int i,b,j,bin[32];
  
  i=0;j=0;
  
  while(a != 0)
    {
      b=a%2;
      a=a/2; 
      bin[i++]=b;
      j=i;
    }

  for(i=j-1;i>=0;i--)
    printf("%d",bin[i]);
  printf("\n");

  return; 
}


int CreateRootStruct()
{
//   cout << "Create ROOT output file structure" << endl;
  channel = 0;
  channel = new int * [UserHeader.nb_ch];
  std::fill_n(channel,UserHeader.nb_ch, static_cast<int*>( 0 ) );

  for (int dim=0;dim<UserHeader.nb_ch;dim++)
    channel[dim] = new int[UserHeader.nb_dt];
  
  tree->Branch("data",&evt);

  return 0;
}
