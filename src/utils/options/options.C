#include "options.h"
#include "readme.h"
#include "usage.h"
#include "include/version.h"
#include <unistd.h>
//#include <fstream.h>
#include <fstream>
#include <iostream>

Options::Options()	
{
  sector = new SectorOptions* [MAX_SECTORS][INSTRUMENTS];
  snooze_dir_name = NULL;
  local_argv = NULL;
  local_argc = 0;
  defaults();
  oldTime = time(NULL) - 3600 ;
}

void Options::defaults()
{

  ImagerIcse           = 0;
  ImagerSectors        = 0;
  ImagerSpacelookStats = 0;
  Inav                 = 0;
  Progress_Counter     = 0; 
  LookForNewFiles      = 0; 
  Snav                 = 0;
  SounderIcse          = 0; 
  SounderSectors       = 0;
  Priority             = 0; // priority frame status ignored

  NavDir = NULL;
  OutDir = NULL;
  Log    = NULL;
  Script = NULL; 
  Debug = Debug0;

  local_argv = NULL;
  local_argc = 0;

}

int Options::sectors(int i ) { 
  int ret = ImagerSectors;
  if (i==sounder)ret =  SounderSectors; 
  if (i==ALL) ret = MAX(ImagerSectors, SounderSectors);
  return(ret);
}

void Options::printUsage(char * argv0) { 
  std::cout << "\nUsage:";
  std::cout << argv0 ; 
  std::cout << " [OPTIONS] <RawGvarDataFile> [MoreRawGvarDataFiles] \n"; 
  std::cout << USAGE;
  std::cout << " Version @ " << version() ; 
  exit(0);
}

int Options::PrintCommandLine(int & argc, char ** & argv) { 
  int arg=1; 

    //print argument list to cout for sanity check
    std::cout <<"Program parameters:\n";
    std::cout <<"      "<<argv[0]<<"\n      ";

    for (arg=1; arg<(argc-1); arg++) {
       if ( strncmp(argv[arg], "-j", 2) == 0)  std::cout <<"\n      "; 
       if ( strncmp(argv[arg], "-N", 2) == 0)  std::cout <<"\n      "; 
       if ( strncmp(argv[arg], "-X", 2) == 0)  std::cout <<"\n      "; 
       if ( strncmp(argv[arg], "-i", 2) == 0)  std::cout <<"\n      "; 
       if ( strncmp(argv[arg], "-c", 2) == 0)  std::cout <<"\n      "; 
       std::cout <<argv[arg]<<" ";
    }
    std::cout <<"\n      "<<argv[arg]<<"\n\n"<<std::flush;

  return(arg);
}

int Options::ParseCommandLine(int & argc, char ** & argv) { 

  int c; 
  extern char *optarg; 
  extern int optind; 
  int channel = ALL; 

  if  (argc < 2) printUsage(argv[0]);
  

  // copy argv[0] to local_argv[0]
  local_argc = 0;
  local_argv = new char * [MAX_ARGS]; 
  local_argv[local_argc] = new char [strlen(argv[0])+1]; 
  strcpy(local_argv[local_argc], argv[0]); 
  local_argc++;
  

  // if command line parameters are in a file, the next 2 parameters must be "-F  filename", so
  //  read in parameters from file to local_argv, local_argc
  int startarg = 1;
  if (! strcmp(argv[1],"-F") ){ 
    std::cout << "Reading arguments from file " << argv[2] << "\n";
    std::ifstream inFile(argv[2]);
    char tmp[FILENAME_SIZE];
    inFile >> tmp;
    while(inFile){
      if(strcmp(tmp, "")){
	local_argv[local_argc] = new char [strlen(tmp)+1];
	strcpy(local_argv[local_argc], tmp);
	local_argc++;
      }
      inFile >> tmp;
    }
    startarg = 3; 
  }

  // copy remaining stuff from argv to local_argv
  for ( int indx = startarg; indx<argc /*argv[indx]*/; indx++) {
     local_argv[local_argc] = new char [FILENAME_SIZE];
     strcpy(local_argv[local_argc], argv[indx]);
     local_argc++;
  }
  local_argv[local_argc] = NULL;


  // now use getopt function to parse all options - command line plus from options file
  SectorOptions *currentSector = NULL;
  //  while((c=getopt(local_argc, local_argv,
  //		  "hf:w:u:i:x:y:c:b:g:G:A:N:e:j:O:s:C:T:M:m:p:lS:KD:X:?:H:F:r:R:PL:Q:"
  //		  )) != EOF)
  // L and Q don't use optional values, so take that out for now

  while((c=getopt(local_argc, local_argv,
		  "hf:w:u:i:x:y:c:b:g:G:A:N:e:j:O:s:C:T:M:m:p:lS:KD:X:?:H:F:r:R:PLQ"
		  )) != EOF)
    switch(c){
      


    case 'i': // INSTRUMENT
      if(!strcasecmp(optarg, "imager"))
	{ sector[ImagerSectors][imager] = new SectorOptions(ICHANNELS);
	  currentSector = sector[ImagerSectors][imager];
	  ImagerSectors++;
        }
      else if(! strcasecmp(optarg, "sounder"))
	{ sector[SounderSectors][sounder] = new SectorOptions(SCHANNELS);
	  currentSector = sector[SounderSectors][sounder];
	  SounderSectors++; 
         }
      else if(!strcasecmp(optarg, "imagerIcse"))
	ImagerIcse = 1; 
      else if(! strcasecmp(optarg, "sounderIcse"))
	SounderIcse = 1; 
      else if(!strcasecmp(optarg, "imagerSpacelookStats"))
	ImagerSpacelookStats = 1; 
      else if (!strcasecmp(optarg, "Snav"))
	Snav = 1; 
      else { 
	std::cout << argv[0] << ": Unknown Instrument " << optarg;
	std::cout <<"\n\tSupported instruments: none, imager, iIcse,";
	std::cout <<"\n\tiSpacelookStats, sounder, sIcse, sNav\n";
	exit(0);
      }
      break; 



    case 'f':{ // FILETYPE
      FILE_TYPE fType ;
      if (!strcasecmp(optarg, "bin"))
	fType = binFile;
      else if(!strcasecmp(optarg, "jpeg"))
	fType = jpegFile;
      else if(!strcasecmp(optarg, "tifftile"))
	fType =	tiffTile;
      else if(!strcasecmp(optarg, "tiffstrip"))
	fType = tiffStrip;
      else if(!strcasecmp(optarg, "tiff"))
	fType = tiffLine; 
      else if(!strcasecmp(optarg, "mcidas"))
	fType = mcidasFile; 
      else if(!strcasecmp(optarg, "none"))
	fType = noFile; 
      else { 
	std::cout << argv[0] << ": Unknown file format " << optarg;
	std::cout<<"\n\tSupported formats: none, bin, jpeg, tiff,";
	std::cout << " tiffTile, tiffScan, hdf \n" ; 
	exit(0); 
      }
      currentSector->fileType() = fType;
      if(fType == mcidasFile)
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->gain(c) = 1.0/32.0;
    }
      break;
      


    case 's':{ // START line and pixel
      int xstart = atoi(optarg);
      int ystart = atoi(strchr(optarg,',')+1);

      if(channel==ALL) 
      {
	for (int c = 0; c < currentSector->channels(); c++)
        {
	  currentSector->xstart(c) = xstart;
	  currentSector->ystart(c) = ystart;
        }
      }
      else
      {
	currentSector->xstart(channel) = xstart;
	currentSector->ystart(channel) = ystart;
      }
    }
      break;



    case 'e':{ // END line and pixel
      int xend= atoi(optarg);
      int yend = atoi(strchr(optarg,',')+1);
      
      if(channel==ALL) 
      {
	for (int c = 0; c < currentSector->channels(); c++)
        {
	  currentSector->xend(c) = xend;
	  currentSector->yend(c) = yend;
        }
      }
      else
      {
	currentSector->xend(channel) = xend;
	currentSector->yend(channel) = yend;
      }
    }
      break;



    case 'r':{ // center line and pixel
      int xCenter = atoi(optarg);
      optarg = strchr(optarg,',')+1;
      int yCenter = atoi(optarg);
      optarg = strchr(optarg,',')+1;
      int xsize   = atoi(optarg);
      optarg = strchr(optarg,',')+1;
      int ysize   = atoi(optarg);
      
      if(channel==ALL) 
      {
	for (int c = 0; c < currentSector->channels(); c++)
        {
	  currentSector->xstart(c) = xCenter - xsize/2;
	  currentSector->ystart(c) = yCenter - ysize/2;
	  currentSector->xend(c) = currentSector->xstart(c)+xsize-1;
	  currentSector->yend(c) = currentSector->ystart(c)+ysize-1;
        }
      }
      else
      {
	currentSector->xstart(channel) = xCenter - xsize/2;
	currentSector->ystart(channel) = yCenter - ysize/2;
	currentSector->xend(channel) = currentSector->xstart(channel)+xsize-1;
	currentSector->yend(channel) = currentSector->ystart(channel)+ysize-1;
      }
      break;
    }
      


    case 'R':{ // center lon and lat
      double xCenter = atof(optarg);
      optarg = strchr(optarg,',')+1;
      double yCenter = atof(optarg);
      optarg = strchr(optarg,',')+1;
      int xsize   = atoi(optarg);
      optarg = strchr(optarg,',')+1;
      int ysize   = atoi(optarg);
      
      currentSector->cLongitude() = xCenter ;
      currentSector->cLatitude() = yCenter  ;
      if(channel==ALL) 
      {
	for (int c = 0; c < currentSector->channels(); c++)
        {
          currentSector->xsize(c)  = xsize;
          currentSector->ysize(c) =  ysize;
        }
      }
      else
      {
        currentSector->xsize(channel)  = xsize;
        currentSector->ysize(channel) =  ysize;
      }
      break;
    }



   case 'L': // align and shift start line
     if (channel==ALL)
     {
       for (int c=0; c<currentSector->channels(); c++)
       {
         currentSector->alignline(c) = 1;
         //currentSector->alignlinevalue(c) = atoi(optarg);
       }
     }
     else
     {
       currentSector->alignline(channel) = 1;
       //currentSector->alignlinevalue(channel) = atoi(optarg);
     }
     break;



   case 'Q': // align and shift start pixel
     if (channel==ALL)
     {
       for (int c=0; c<currentSector->channels(); c++)
       {
         currentSector->alignpixel(c) = 1;
         //currentSector->alignpixelvalue(c) = atoi(optarg);
       }
     }
     else
     {
       currentSector->alignpixel(channel) = 1;
       //currentSector->alignpixelvalue(channel) = atoi(optarg);
     }
     break;



    case 'T':{ // TIMESTAMP
      int timeStamp;
      if (!strcasecmp(optarg, "fromfilename"))
	timeStamp = 0; 
      else if (! strcasecmp(optarg, "framestart"))
	timeStamp = 1; 
      else if (! strcasecmp(optarg, "longframestart"))
	timeStamp = 3; 
      else if ( ! strcasecmp(optarg, "region")) 
	timeStamp = 2; 
      else { 
	std::cout << argv[0] << " Unsupported timestamp " << optarg ;
	std::cout << " Supported timestamps: fromfilename, framestart \n";
	exit(0);
      }
      currentSector->timeStamp() = timeStamp;
    }
      break;
      


    case 'C':{ // COMPRESS
      int compress;
      if(! strcasecmp(optarg, "ccittrle"))
	compress = COMPRESSION_CCITTRLE; 
      else if(! strcasecmp(optarg, "ciitfax3"))
	compress = COMPRESSION_CCITTFAX3; 
      else if (! strcasecmp(optarg, "lzw"))
	compress = COMPRESSION_LZW; 
      else if (! strcasecmp(optarg, "jpeg"))
	compress = COMPRESSION_JPEG;
      else if (! strcasecmp(optarg, "next"))
	compress = COMPRESSION_NEXT;
      else if (! strcasecmp(optarg, "ccittrlew"))
	compress = COMPRESSION_CCITTRLEW;
      else if (! strcasecmp(optarg, "packbits"))
	compress = COMPRESSION_PACKBITS;
      else if (! strcasecmp(optarg, "thunderscan"))
	compress = COMPRESSION_THUNDERSCAN;
      else if (! strcasecmp(optarg, "pixarfilm"))
	compress = COMPRESSION_PIXARFILM;
      else if (isdigit(optarg[0]) )
	compress = atoi(optarg);
      else { 
	std::cout << argv[0] << ": Unknown compression type " << optarg; 
	std::cout <<"\n\tSupported types: ccittrle, ccittfax3, lzw ";
	std::cout << "\n\t next, ccittrlew, packbits, thunderscan, pixarfilm";
	exit(0);
      }
      
      currentSector->compress() = compress;

    }
      break;



    case 'c': // CHANNEL
      if (isdigit(optarg[0])) {
	channel = atoi(optarg) -1 ;
      } else channel = ALL;  
      break;
      


    case 'w':{ // DATATYPE
      WORD_TYPE dType;
      if (!strcasecmp(optarg, "char"))
	dType = Char; 
      else if (!strcasecmp(optarg, "uchar")) 
	dType = Uchar; 
      else if (!strcasecmp(optarg, "short"))
	dType = Short; 
      else if (!strcasecmp(optarg, "ushort"))
	dType = Ushort; 
      else if (!strcasecmp(optarg, "int"))
	dType = Int; 
      else if (!strcasecmp(optarg, "uint"))
	dType = Uint; 
      else if (!strcasecmp(optarg, "float"))
	dType = Float; 
      else if (!strcasecmp(optarg, "double"))
	dType = Double; 
      else if (!strcasecmp(optarg, "none"))
	dType = Undef; 
      else { 
	std::cout << argv[0] << ": Unknown word type " << optarg;
	std::cout<<"\n\tSupported types: uchar, char, short, ushort,";
	std::cout <<" \t int, uint, float, double \n";
	exit(0); } 
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->wordType(c) = dType;
      else
	currentSector->wordType(channel) = dType;
    }
      break;



    case 'u':{ // UNITS
      RadiometricUnits units;
      if (! strcasecmp(optarg, "counts"))
	units = Counts; 
      else if (! strcasecmp(optarg, "tstar")) 
	units = Tstar; 
      else if (! strcasecmp(optarg, "Tscene")) 
	units = Tscene; 
      else if (! strcasecmp(optarg, "radiance"))
	units = Radiance; 
      else if (! strcasecmp(optarg, "albedo"))
	units = Albedo; 
      else if (! strcasecmp(optarg, "albedo2"))
	{ units = Albedo2; ImagerSpacelookStats = 1; }
      else if (! strcasecmp(optarg, "modeA"))
	units = ModeA;
      else { 
	std::cout << argv[0] << ": Unknown units " << optarg; 
	std::cout <<"\n\tSupported units: counts, radiance " ;
	std::cout <<"\n\ttstar, tscene, modeA (for IR channels) ";
	std::cout <<"\n\talbedo, albedo2    (for Vis channels) "; 
	exit(0);
      }
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->units(c) = units;
      else
	currentSector->units(channel) = units;
    }
      break;



    case 'x':{  // XSCALE
      double xscale = atof(optarg);
      if (xscale < 1) xscale = -1.0/xscale; 
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->xscale(c) = xscale;
      else
	currentSector->xscale(channel) = xscale;
    }
      break;   
      


    case 'y':{ // YSCALE
      double yscale = atof(optarg);
      if (yscale < 1) yscale = -1.0/yscale; 
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->yscale(c) = yscale;
      else
	currentSector->yscale(channel) = yscale;
    }
      break;



    case 'g':{ // GAIN
      double gain = atof(optarg);
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->gain(c) = gain;
      else
	currentSector->gain(channel) = gain;
    }
      break;



    case 'b':{ // BIAS
      double bias = atof(optarg);
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->bias(c) = bias;
      else
	currentSector->bias(channel) = bias;
    }
      break;



    case 'G':{ // Gamma
      double gamma = atof(optarg);
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->gamma(c) = gamma;
      else
	currentSector->gamma(channel) = gamma;
    }
      break;



    case 'A':{ // Area
      float Area  = atof(optarg);
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->area(c) = Area;
      else
	currentSector->area(channel) = Area;
    }
      break;



    case 'N':{ // navigation
      Inav = 1; 
      NavDir = optarg; 
    }
      break;



    case 'm':{  // MAPVALUE
      double mval = atof(optarg);
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->mapVal(c) = mval;
      else
	currentSector->mapVal(channel) = mval;
    }
      break;



    case 'M':{ // MAPSET
      GRIDSET map;
      if (! strcasecmp(optarg, "none"))
	map = grid0; 
      else if (! strcasecmp(optarg, "grid1")) 
	map = grid1; 
      else if (! strcasecmp(optarg, "grid2")) 
	map = grid2; 
      else if (! strcasecmp(optarg, "grid12")) 
	map = grid12; 
      else{ 
	std::cout << argv[0] << ": Unknown map type " << optarg; 
	std::cout <<"\n\tSupported types: none, grid1, grid2, grid12 ";
	exit(0);
      }
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->map(c) = map;
      else
	currentSector->map(channel) = map;
    }
      break;
      


    case 'p':{ // PATH
      char * dir = optarg;
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++)
	  currentSector->setDirectory(c,dir);
      else
	currentSector->setDirectory(channel,dir);
    }
      break;



    case 'l': // LINK
      currentSector->linkLatest() = 1;
      break;



    case 'H':{
      int h0 = atoi(optarg);
      int h1 = atoi(strchr(optarg,',')+1);      
      if(channel==ALL) 
	for (int c = 0; c < currentSector->channels(); c++){
	  currentSector->gmtHoursStart(c) = h0;
	  currentSector->gmtHoursStop(c) = h1;	  
	}
      else{
	currentSector->gmtHoursStart(channel) = h0;
	currentSector->gmtHoursStop(channel) = h1;	  
      }
    }
      break;      
      


    case 'S': // SNOOZE
      lookForNewFiles() = 1; 

      if(!snooze_dir_name) { 
	snooze_dir_name = new char * [32];
	for ( int i = 0; i < 32; i++) snooze_dir_name[i] = NULL;
      }

      int i1;
      for(i1 =0; snooze_dir_name[i1]; i1++);
      snooze_dir_name[i1] = new char [strlen(optarg)+1];
      strcpy(snooze_dir_name[i1], optarg);

      break;



    case 'K': // PROGRESS_COUNTER
      progress_counter() = 0; 
      break;



    case 'D': // DEBUG
      if (! strcasecmp(optarg, "none"))
	debug() = Debug0;
      else if (! strcasecmp(optarg, "header")) 
	debug() = Dheader; 
      else if (! strcasecmp(optarg, "linedoc")) 
	debug() = Dlinedoc; 
      else if (! strcasecmp(optarg, "frame")) 
	debug() = Dframe; 
      else if (! strcasecmp(optarg, "dump_all")) 
	debug() = Ddump_all; 
      else if (! strcasecmp(optarg, "block0time")) 
	debug() = Dblock0time; 
      else if (! strcasecmp(optarg, "sad")) 
	debug() = Dsadid; 
      else if (! strcasecmp(optarg, "crc"))
	debug() = Dcrc; 
      else if (! strcasecmp(optarg, "parity"))
	debug() = Dparity; 
      else{ 
	std::cout << argv[0] << ": Unknown debug type " << optarg ; 
	std::cout <<"\n\tSupported types: none,header";
	std::cout <<"\n\t linedoc,block0time,sad,crc,parity ";
	exit(0);
      }
      break;
      


    case 'h': // HELP
      std::cout << README; 
      exit(0);
      break;
      

    case 'X':
      Script = optarg; 
      break;


    case 'j':
      Log = optarg; 
      break;


    case 'O':
      OutDir = optarg; 
      break;


    case 'F':
      break;


    case 'P': // turn on priority frame processing
      Priority = 1;
      break;
      

    default:
      printUsage(argv[0]); 
      break;
    }

  std::cout.flush();


  // gvar filenames will be last thing on command line,
  // convert them to full path specification
  if( argc > 3 ){
    for(int ndx =optind ;ndx < local_argc; ndx++)
      getLinkName(local_argv[ndx]);
  }
  
  argc = local_argc;
  argv = local_argv;
  return(optind);
}

char* Options::nextFile(int argc, char **argv) { 
  
  /*extern char *optarg; */
  extern int optind; 
  
  static char fname[FILENAME_SIZE];
  struct stat Stat; 

  if(script()) 
  {
    int err;
    std::cout << "\n Executing " << script() << "\n" << std::flush; 
    err = system(script());
    if (err==-1)
      perror("Failure: ");
  }

  if(optind < argc){ 
    if (stat(argv[optind], &Stat) == -1)  {
      std::cerr << argv[0] << " error:Could not open file " 
	<< argv[optind] <<"\n"; 
      optind++;
      return(nextFile(argc, argv));
    }
    oldTime = Stat.st_ctime;
    strcpy(fname,argv[optind]);
    optind++;
  }
  else if (lookForNewFiles()){
    int flag;
    while( (flag = FindNewerFile(oldTime,snooze_dir_name,fname))==0)  sleep(60)   ;
    if (flag==2) // error condition
      return NULL;
  }
  else return((NULL));
  
  return(fname );
}


int Options::FindNewerFile(time_t & TimeOld, 
			   char **dirName, 
			   char *fname){ 
  struct stat Stat; 
  DIR* dirp; 
  char buf[sizeof(struct dirent) + FILENAME_SIZE];
  struct dirent * direntp = (struct dirent*) buf;

  char tmpnam[FILENAME_SIZE];

  time_t timdiff = 0x7fffffff ; 
  int dircount = 0, dirfail = 0; 
  time_t oTime = TimeOld;

  if (!dirName) return(2);

  while(dirName[dircount])
  { 
    if ((dirp = opendir(dirName[dircount])) == NULL)
    {
      std::cout << " Could not open " <<  dirName[dircount] << std::endl;
      dirfail++;
      // return(0);
    }
    else
    {
      while((direntp = readdir(dirp)) != NULL){ 
        if (direntp->d_name[0] != '.'){
	  strcpy(tmpnam,dirName[dircount]);      
	  strcat(tmpnam,"/");
	  strcat(tmpnam,direntp->d_name);
	  stat(tmpnam, &Stat);
	  if(Stat.st_ctime > oTime && Stat.st_ctime-oTime < timdiff){
	    strcpy(fname, tmpnam);
	    timdiff = Stat.st_ctime - oTime;
	    TimeOld = Stat.st_ctime;
	  }
	
        }
      }
      closedir(dirp);
    }
    dircount++;
  }
  if (dircount==dirfail) // none of the directories is readable
    return(2); // potential file system problem, exit and start again
  
  return(timdiff != 0x7fffffff);
}


char * Options::script()
{ return ( Script); } 

char * Options::log()
{ return ( Log); } 

char * Options::outdir()
{ return ( OutDir); } 

char * Options::navdir()
{ return ( NavDir); } 


void Options::getLinkName(char * fname){
  char linkName[FILENAME_SIZE];
  realpath(fname, linkName);
  strcpy(fname, linkName);
}
