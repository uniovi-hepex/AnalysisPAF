///////////////////////////////////////////////////////////////////////
//
//    FILE: DatasetManager.C
//   CLASS: DatasetManager
//  AUTHOR: I. Gonzalez Caballero
//    DATE: January, 2011
//
///////////////////////////////////////////////////////////////////////

#include "DatasetManager.h"

#include "TSystem.h"
#include "TObjArray.h"

#include <iostream>
#include <fstream>

#include <time.h> //For time types and functions

using namespace std;

///////////////////////////////////////////////
//
// A few constants
//
// + Time between redownloads in seconds
const time_t MAXREDOWNLOADTIME = 86400;
// + N. of times DatasetManager tries to find files
const unsigned int MAXTRIALS = 5; 

static const char* kGoogleURL[] = {
  "http://spreadsheets.google.com/feeds/worksheets/t2jDBQxd2cmplDeuQReRGlQ/public/values",
  "http://spreadsheets.google.com/feeds/worksheets/1b4qnWfZrimEGYc1z4dHl21-A9qyJgpqNUbhOlvCzjbE/public/values"
};


///////////////////////////////////////////////
//
// Static members
//
DatasetManager* DatasetManager::fInstance      = 0; //static initialization
TString         DatasetManager::fLocalBasePath = DatasetManager::GuessLocalBasePath(); //static 
TString         DatasetManager::fSkim          = ".";
TString         DatasetManager::fTab           = "";
unsigned int    DatasetManager::fLHCRun        = 2;
TString         DatasetManager::fLocalFolder   = "";




//////////////
// The method to retrieve the one and only instance of this object
//
DatasetManager* DatasetManager::GetInstance() {
  if (!fInstance)
    fInstance = new DatasetManager();

  return fInstance;
}


//////////////
// Constructors
//
// Empty constructor
DatasetManager::DatasetManager():
  fDataset(""),
  fBaseFileName(""),
  fEventsInTheSample(0),
  fCrossSection(0),
  fSumWeight(0)
{
  //Check if the tabs.dat is already there and new enough
  if (ShouldDownloadAvailableTabs()) {
    if (!DownloadAvailableTabs()) {
      cerr << "FATAL: Problems accessing the data" << endl;
      exit(-2);
    }
  }
}



//////////////
// Sets the folder/tab to use to search for the dataset. Downloads the
// information if needed.
//
bool DatasetManager::SetTab(const char* folder, bool redownload) { 
  //Replace spaces by _
  fTab = folder;
  fTab.ReplaceAll(" ", "_");

  
  //Clear files
  fFiles.clear();


  //Check if folder.dat exists and is new enough
  TString file("Datasets/");
  file+=fTab;
  file += ".dat";
  if (!redownload) {
    Long_t id = 0;
    Long_t size = 0;
    Long_t flag = 0;
    Long_t fmodtime = 0; //Mod time in seconds
    if (gSystem->GetPathInfo(file, 
			     &id, &size, &flag, &fmodtime) == 1) {
      cerr << "NOTE: Could not find " << file <<  endl
	   << "      We will retrieve it." << endl;
      redownload = true;
    }
    else if ((time(NULL) - fmodtime) > MAXREDOWNLOADTIME) {
      cerr << "NOTE: " << file << " is older than 24 hours" 
	   <<  endl
	   << "      We will redownload it." << endl;
      redownload = true;
    }
  }

  // Download tab only if needed
  if (redownload) {
    if (!DownloadTab(fTab)) {
      cerr << "ERROR: Could not download folder " << fTab << endl;
      fTab = "";
      return false;
    }
  }

  FindLocalFolder();

  return true;
}



//////////////
// Load Dataset (datasetname) information. Optionally a folder and/or a skim
// can be provided overridding the current ones. 
bool DatasetManager::LoadDataset(const char* datasetname) {
#ifdef DEBUGDS
  cout << "DEBUG: Loading dataset " << datasetname;
  cout << " ..." << endl;
#endif
#if (DEBUGDS >= 2)
  cout << "DEBUG: + fLocalBasePath = \"" << fLocalBasePath << "\"" << endl;
  cout << "DEBUG: + fSkim = \"" << fSkim << "\"" << endl;
  cout << "DEBUG: + fTab = \"" << fTab << "\"" << endl;
#endif

  //Remove information from previous dataset
  fFiles.clear();

  if (fTab == "") {
    cerr << "ERROR: Folder is not set in DatasetManager!" << endl;
    return false;
  }
  
  //Build file names
  TString file("Datasets/");
  file+=fTab;
  file += ".dat";
  
  ifstream is(file);
  TString tmpdataset;
  char tmpline[256];
  while (is) {
    is >> tmpdataset;
#if (DEBUGDS >= 2)
    cout << "DEBUG: + Found dataset = \"" << tmpdataset << "\"" << endl;
#endif
    // If the first word is not the sample we want or it starts with a # skip line
    if (tmpdataset[0]=='#' || tmpdataset != datasetname) {
      is.getline(tmpline, 255);
    }
    else {
#if (DEBUGDS >= 2)
      cout << "DEBUG:   - Reading..." << endl;
#endif
      fDataset = datasetname;
      is >> fEventsInTheSample;
      is >> fCrossSection;
      is >> fLocalFolder;
      is >> fBaseFileName;
      is >> fSumWeight;
      return true;
    }
  }

  return false;
}

///////////
// Returns a vector with the full path to the files in the dataset
const vector<TString>& DatasetManager::GetFiles() {
  //Check if the list of files has been built. Otherwise construct it
  unsigned int trials = 0;
  while (fFiles.size() == 0 && trials < MAXTRIALS) {
    //Check if file exists
    TString fullpath = fLocalBasePath + "/" + fLocalFolder;
    if (fLocalFolder[fLocalFolder.Sizeof()-2] != '/')
      fullpath += "/";
    if (fSkim != "." && fSkim != "")
      fullpath+= fSkim + "/";
    TString basefile = fBaseFileName;
#ifdef DEBUGDS
    cout << "DEBUG: basefile = " << basefile << endl;
    cout << "DEBUG: fBaseFileName = " << fBaseFileName << endl;
    cout << "DEBUG: fullpath = " << fullpath.Data() << endl;
#endif

    //Find files if the sample was split in several pieces
    // NOTE: The command gSystem->FindFile screws the variables passed.
    //       I need to set them again.
    basefile = fBaseFileName;
    if (fullpath[fullpath.Sizeof()-2] == '/')
      fullpath.Remove(fullpath.Sizeof()-2);
    TString command("ls ");
    command += 
      fullpath + "/" + basefile + " " +
      fullpath + "/" + basefile(0, basefile.Sizeof() - 6) + "_[0-9].root " +
      fullpath + "/" + basefile(0, basefile.Sizeof() - 6) + "_[0-9][0-9].root";
#ifdef DEBUGDS
    cout << "DEBUG: Executing command " << command << endl;
#else
    command += " 2> /dev/null";
#endif
    //We cannot use GetFromPipe because it is too verbose, so we implement
    //the full code
    //    TString result=gSystem->GetFromPipe(command);
    TString result;
    FILE *pipe = gSystem->OpenPipe(command, "r");
    if (!pipe) {
      cerr << "ERROR: in DatasetManager::GetFiles. Cannot run command \""
	   << command << "\"" << endl;
    }
    else {
      TString line;
      while (line.Gets(pipe)) {
	if (result != "")
	  result += "\n";
	result += line;
      }

      gSystem->ClosePipe(pipe);
    }
   
#ifdef DEBUGDS
    cout << "DEBUG: Result is " << result << endl;
#endif
    if (result != "" ) {
      TObjArray* filesfound = result.Tokenize(TString('\n'));
      if (!filesfound)
	cerr << "ERROR: Could not parse output while finding files" << endl;
      else {
	for (int i = 0; i < filesfound->GetEntries(); i++) {
	  fFiles.push_back(filesfound->At(i)->GetName());
	}
	filesfound->Clear();
	delete filesfound;
      }
    }

    // Increment the counter on the number of trials
    trials++;
    if (fFiles.size() == 0 && trials == 5)
      cerr << "ERROR: Could not find files in /" << fLocalBasePath
	   << "/" << fLocalFolder << "/" << fSkim << endl
	   << "       of the form " << fBaseFileName << endl;
  }


  return fFiles;
}





///////////
// Dumps all the information
void DatasetManager::Dump() const {
  cout << "Dataset Name: " << fDataset << endl;
  cout << "N. Events: " << fEventsInTheSample << endl;
  cout << "Cross Section: " << fCrossSection << endl;
  cout << "Base File Name: " << fBaseFileName << endl;
  cout << "SumWeight:" << fSumWeight << endl;
  cout << "Files:" << endl;
  for (unsigned int i = 0; i < fFiles.size(); i++)
    cout << " + " << fFiles[i] << endl;
}





///////////
//Download the information on the folder/tabs available from Google Docs
bool DatasetManager::DownloadAvailableTabs() {
  //Write script
  TString scriptname = WriteGetDatatabsScript();

  //Execute script
  int ret = gSystem->Exec(scriptname);

  //Delete script
  gSystem->Unlink(scriptname);
  
  TString tabsdat("tabs.dat");
  if (!gSystem->FindFile("Datasets", tabsdat)) {
    cerr << "ERROR: Could not find the available folders from the Google Docs spreadsheet!" << endl;
    ret = -1;
  }

  return (!ret);
}

///////////
// Redownload all the files
bool DatasetManager::DownloadTab(const char* folder) {
  //Write script
#ifdef DEBUGDS  
  cout << "DEBUG: Writing..." << endl;
#endif
  TString scriptname = WriteGetDatasetsScript();

  //Execute script
#ifdef DEBUGDS  
  cout << "DEBUG: Executing " << scriptname << " " << folder << "..." << endl;
#endif
  int ret = gSystem->Exec(scriptname + " " + folder);

  //Delete script
  gSystem->Unlink(scriptname);

  TString dsdat(folder);
  dsdat+=".dat";
  if (!gSystem->FindFile("Datasets", dsdat)) {
    cerr << "ERROR: Could not create Datasets/" << folder << ".dat!" << endl;
    ret = -1;
  }

  //cout << "DEB: ret=" << ret << endl;
  return (!ret);
}





///////////
// This method writes the script that downloads and processes the information
// on the available datasets from Google Docs
TString DatasetManager::WriteGetDatatabsScript() {
  TString googleURL = kGoogleURL[fLHCRun-1];



  //Build file name with User ID in the /tmp directory
  TString scriptname="/tmp/";
  scriptname+=gSystem->GetUid();
  scriptname+="-gettabs.sh";

  //Open file
  ofstream os(scriptname);



  os << "#!/bin/bash" << endl;
  os << "#Some configuration values" << endl;
  os << "tabsurl=\"" << googleURL << "\"" << endl;
  os << "tabsxml=\"/tmp/values.xml\"" << endl;
  os << "tmpfiles=\"$tabsxml\"" << endl;
  os << "" << endl;
  os << "#Remove temp files if the script is stoped by some reason" << endl;
  os << "trap \"rm -f $tmpfiles; exit\" INT TERM EXIT" << endl;
  os << "" << endl;

  os << "#Download the information on the available tabs" << endl;
  os << "curl \"$tabsurl\" 2> /dev/null | xmllint --format - > $tabsxml" << endl;
  os << "" << endl;
  os << "#Build directories if they don't exit" << endl;
  os << "pathtodatfiles=\"./Datasets\"" << endl;
  os << "if [ ! -d \"$pathtodatfiles\" ]; then" << endl;
  os << "    echo \">> Creating $pathtodatfiles to store the information of the datasets\"" << endl;
  os << "    mkdir $pathtodatfiles" << endl;
  os << "fi" << endl;
  os << "" << endl;
  os << "#Output file" << endl;
  os << "outputfile=\"$pathtodatfiles/tabs.dat\"" << endl;
  os << "" << endl;
  os << "#Compute the number of tabs" << endl;
  os << "#echo DEB:tabsxml=$tabsxml" << endl;  
  os << "#ls -l $tabsxml" << endl;
  os << "ntabs=`grep \"<entry>\" $tabsxml  | wc -l`" << endl;
  os << "#echo \"DEB: NTABS = $ntabs\"" << endl;
  os << "" << endl;
  os << "#Get the titles and the links in some array" << endl;
  os << "tabtitles=( `grep \"<entry>\" -A 4 $tabsxml| grep title | cut -d \\> -f 2| cut -d \\< -f 1 | sed 's\\ \\_\\g'` )" << endl;
  os << "tablinks=( `grep \"<entry>\" -A 7 $tabsxml| grep \"<link \" | grep cells | cut -d \\\" -f 6` )" << endl;
  os << "" << endl;
  os << "#Store output in $outputfile" << endl;
  os << "rm -f $outputfile" << endl;
  os << "touch $outputfile" << endl;
  os << "for ((i=0;i<$ntabs;i++)); do" << endl;
  os << "   #echo -e \"DEB: [$i] ${tabtitles[${i}]}\\t${tablinks[${i}]}\"" << endl;
  os << "   echo -e \" ${tabtitles[${i}]} ${tablinks[${i}]}\" >> $outputfile" << endl;
  os << "done" << endl;


  //Close file
  os.close();

  //Set apropiate permisions
  gSystem->Chmod(scriptname, 448/*700 in decimal = rwx------*/);

  //Return file name
  return scriptname;

}



///////////
// Writes the scripts that retrieves the dataset information for a given 
// folder/tab
TString DatasetManager::WriteGetDatasetsScript() {

  //Settings depending on LHC Run
  char neventscol   = 'D';
  char xsectioncol  = 'Q';
  char minitreecol  = 'O';
  char sumWeightcol = 'R';
  if (fLHCRun == 2) {
    neventscol  = 'C';
    xsectioncol = 'H';
    minitreecol = 'F';
  }

  //Build file name with User ID in the /tmp directory
  TString scriptname="/tmp/";
  scriptname+=gSystem->GetUid();
  scriptname+="-getdatasets.sh";

  //Open file
  ofstream os(scriptname);
  
  //Write script

  os << "#!/bin/bash" << endl;
  os << "#Some configuration values" << endl;
  os << "xmlfile=/tmp/$UID-datasets.xml" << endl;
  os << "formatedfile=/tmp/$UID-datasets.html" << endl;
  //os << "tmpfiles=\"$xmlfile $formatedfile\"" << endl;
  os << "tmpfiles=\"$xmlfile\"" << endl;
  os << "" << endl;
  os << "#Remove temp files if the script is stoped by some reason" << endl;
  os << "trap \"rm -f $tmpfiles; exit\" INT TERM EXIT" << endl;
  os << " " << endl;
  os << "#Check arguments" << endl;
  os << "if [ -z $1 ]; then" << endl;
  os << "    echo \"ERROR: $0 needs a parameter specifiying the dataset\"" << endl;
  os << "    exit 100" << endl;
  os << "fi" << endl;
  os << "dataset=`echo $1 | sed 's\\ \\_\\g'`" << endl;
  os << "#echo DEB:$dataset" << endl;
  os << " " << endl;
  os << "#Build directories if they don't exit" << endl;
  os << "pathtodatfiles=\"./Datasets\"" << endl;
  os << "if [ ! -d \"$pathtodatfiles\" ]; then" << endl;
  os << "    echo \"ERROR: Could not find directory $pathtodatfiles. Exiting!\"" << endl;
  os << "    exit 1" << endl;
  os << "fi" << endl;
  os << "" << endl;
  os << "echo \">> Reading tab for dataset $dataset...\"" << endl;
  os << "datasetsdb=\"$pathtodatfiles/tabs.dat\"" << endl;
  os << "if [ ! -f \"$datasetsdb\" ]; then" << endl;
  os << "    echo \"ERROR: Could not find file \\\"$datasetsdb\\\". Exiting!\"" << endl;
  os << "    exit 2" << endl;
  os << "fi" << endl;
  os << "googlexmlfeed=`grep \" $dataset \" $datasetsdb | cut -d\\  -f 3`" << endl;
  os << "if [ \"$googlexmlfeed\" == \"\" ]; then" << endl;
  os << "    echo \"ERROR: Could not find an entry for $dataset in $datasetsdb\"" << endl;
  os << "    exit 99" << endl;
  os << "fi" << endl;
  os << "#echo \"   + Getting feed from $googlexmlfeed\"" << endl;
  os << "curl \"$googlexmlfeed\" > $xmlfile 2> /dev/null" << endl;
  os << "xmllint --format $xmlfile > $formatedfile" << endl;
  os << "" << endl;
  os << "#Find last cell" << endl;
  os << "lastcell=`grep -e \"</title>\"  $formatedfile | tail -1 | cut -d \\> -f 2 | cut -d \\< -f 1`" << endl;
  os << "#Get letter of last cell and ASCII code for the letter" << endl;
  os << "lastcelllet=`echo ${lastcell:0:1}`" << endl;
  os << "lastcellnum=`echo ${lastcell:1}`" << endl;
  os << "lastcellascii=`printf \"%d\\n\" \"'$lastcelllet\"`" << endl;
  os << "#echo \"   DEB: Last Cell = $lastcelllet ($lastcellascii)\"" << endl;
  os << "" << endl;
  os << "#Get the letters from A to lastcell" << endl;
  os << "letters=`awk 'BEGIN{for(i=65;i<=85;i++) printf(\"%c \",i);}'`" << endl;
  os << "" << endl;
  os << "" << endl;
  os << "#Main folder" << endl;
  os << "mainfolder=`grep -A 1 \"O1</title>\" $formatedfile | grep -v title| cut -d \\> -f 2 | cut -d \\< -f 1|cut -d \\  -f 1`" << endl;
  os << "skim1folder=`grep -A 1 \"R1</title>\" $formatedfile | grep -v title | cut -d \\> -f 2| cut -d \\< -f 1| sed 's/[F,f]older//g' | sed 's/ //g'`" << endl;
  os << "skim2folder=`grep -A 1 \"T1</title>\" $formatedfile | grep -v title | cut -d \\> -f 2| cut -d \\< -f 1| sed 's/[F,f]older//g' | sed 's/ //g'`" << endl;
  os << "" << endl;
  os << "#echo \"   DEB: Main folder = $mainfolder\"" << endl;
  os << "#echo \"   DEB: Skim 1 folder = $skim1folder\"" << endl;
  os << "#echo \"   DEB: Skim 2 folder = $skim2folder\"" << endl;
  os << "" << endl;
  os << "outputfile=$pathtodatfiles/$dataset.dat" << endl;
  os << "echo \"   + Writing info to $outputfile...\"" << endl;
  os << "if [ -f $outputfile ]; then" << endl;
  os << "    echo \"     - $outputfile already exists. Moving it to $outputfile.bak\"" << endl;
  os << "    mv $outputfile $outputfile.bak" << endl;
  os << "fi" << endl;
  os << "touch $outputfile" << endl;
  os << "echo -e \"#Dataset\\tN_Events\\tCross Section\\tMain Folder\\tBase File\\tSumWeight\" >> $outputfile" << endl;
  os << "" << endl;
  os << "#Iterate over rows (first 2 rows are titles)" << endl;
  os << "echo \"   + Iterating over $lastcellnum rows...\"" << endl;
  os << "for i in `seq 3 $lastcellnum`; do" << endl;
  os << "    #echo -n \"$i \"" << endl;
  os << "    Ai=`grep -A 1 \"A$i</title>\" $formatedfile | grep -v title| cut -d \\> -f 2 | cut -d \\< -f 1`" << endl;
  os << "    " << endl;
  os << "    #echo \"Dataset name = $Ai\"" << endl;
  os << "    " << endl;
  os << "    # Get general information of the sample" << endl;
  os << "    rootfilebase=`grep -A 1 \"" << minitreecol << "$i</title>\" $formatedfile | grep -v title| cut -d \\> -f 2 | cut -d \\< -f 1`" << endl;
  os << "    nevents=`grep -A 1 \"" << neventscol << "$i</title>\" $formatedfile | grep -v title| cut -d \\> -f 2 | cut -d \\< -f 1 | sed 's/,/./g'`" << endl;
  os << "    xsection=`grep -A 1 \"" << xsectioncol << "$i</title>\" $formatedfile | grep -v title| cut -d \\> -f 2 | cut -d \\< -f 1 | sed 's/,/./g'`" << endl;
  os << "    sumWeight=`grep -A 1 \"" << sumWeightcol << "$i</title>\" $formatedfile | grep -v title| cut -d \\> -f 2 | cut -d \\< -f 1 | sed 's/,/./g'`" << endl;
  os << "    #echo \"     - DEB: File = $rootfilebase\" " << endl;
  os << "    #echo \"     - DEB: NEvt = \\'$nevents\\'\" " << endl;
  os << "    #echo \"     - DEB: Xsec = \\'$xsection\\'\" " << endl;
  os << "    #echo \"     - DEB: SumW = $sumWeight\" " << endl;
  os << "    " << endl;
  os << "    #Only printout if number of events and cross section are numbers" << endl;
  os << "    numberformat='^[0-9]+([.][0-9]+)?$'" << endl;
  os << "    #[[ $nevents =~ $numberformat ]] && [[ $xsection =~ $numberformat ]] && echo -e \"Escribiendo que es gerundio\"" << endl;
 os << "    [[ $nevents =~ $numberformat ]] && [[ $xsection =~ $numberformat ]] && echo -e \"$Ai\\t$nevents\\t$xsection\\t$mainfolder\\t$rootfilebase\t$sumWeight\" >> $outputfile" << endl;
  os << "done #rows" << endl;
  os << "#echo \"\"" << endl;
  os << "exit 0" << endl;




  //Close file
  os.close();

  //Set apropiate permisions
  gSystem->Chmod(scriptname, 448/*700 in decimal = rwx------*/);

  //Return file name
  return scriptname;

}

void DatasetManager::RedownloadFiles() {
  //Just download the available folders.
  //This will trigger the redownload of old folder.dat files when requested
  DownloadAvailableTabs();
  if (fTab != "") {
    DownloadTab(fTab.Data());
  }
};



TString DatasetManager::GuessLocalBasePath() {
    TString host = gSystem->HostName();
    if (host.Contains("uniovi.es"))
      return TString("/pool/ciencias");
    else if (host.Contains("ifca.es"))
      return TString("/gpfs/csic_projects/cms/data");
    else if (host.Contains("cern.ch"))
      return TString("/pool/data1/MiniTrees/");
    else {
      cerr << "ERROR: Could not guess base path from host name " << host << endl;
      return TString("");
    }
}



//Get the data files providing a relative path to it and file base name
//It will check for files of the following formats:
//* [localpath]/relativepath/filebasename_XX.root
vector<TString> DatasetManager::GetRealDataFiles(const char* filebasename,
						 const char* relativepath) {
#ifdef DEBUGDS
  cout << "DEBUG: Local Folder = " << fLocalFolder << endl;
  //cout << "DEBUG: Relative Path = " << relativepath << endl;
#endif

  vector<TString> theFiles;
  TString basefile(filebasename);
  TString fullpath;

  if (relativepath) {
    fullpath = Form("%s/%s", fLocalBasePath.Data(), relativepath);
  }
  else {
    fullpath = Form("%s/%s", fLocalBasePath.Data(), fLocalFolder.Data());
  }
#ifdef DEBUGDS
  cout << "DEBUG: full path = " << fullpath << endl;
#endif


  TString command("ls ");
  command += 
    fullpath + "/" + basefile + ".root " +
    fullpath + "/" + basefile + "_[0-9].root " +
    fullpath + "/" + basefile + "_[0-9][0-9].root" +
    fullpath + "/Tree_" + basefile + ".root " +
    fullpath + "/Tree_" + basefile + "_[0-9].root " +
    fullpath + "/Tree_" + basefile + "_[0-9][0-9].root";
#ifdef DEBUGDS
  cout << "DEBUG: Executing command " << command << endl;
#else
  command += " 2> /dev/null";
#endif

  //We cannot use GetFromPipe because it is too verbose, so we implement
  //the full code
  //    TString result=gSystem->GetFromPipe(command);
  TString result;
  FILE *pipe = gSystem->OpenPipe(command, "r");
  if (!pipe) {
    cerr << "ERROR: in DatasetManager::GetFiles. Cannot run command \""
	 << command << "\"" << endl;
  }
  else {
    TString line;
    while (line.Gets(pipe)) {
      if (result != "")
	result += "\n";
      result += line;
    }
    
    gSystem->ClosePipe(pipe);
  }
  
#ifdef DEBUGDS
  cout << "DEBUG: Result is " << result << endl;
#endif
  if (result != "" ) {
    TObjArray* filesfound = result.Tokenize(TString('\n'));
    if (!filesfound)
      cerr << "ERROR: Could not parse output while finding files" << endl;
    else {
      for (int i = 0; i < filesfound->GetEntries(); i++) {
	
	theFiles.push_back(filesfound->At(i)->GetName());
      }
      filesfound->Clear();
      delete filesfound;
    }
  }

  if (theFiles.size() == 0)
    cerr << "ERROR: Could not find data!" << endl;
  
  return theFiles;
}


// Check if the available set of tabs should be downloaded
bool DatasetManager::ShouldDownloadAvailableTabs() const {
  //Check if the tabs.dat is already there and new enough
  Long_t id = 0;
  Long_t size = 0;
  Long_t flag = 0;
  Long_t modtime = 0; //Mod time in seconds
  if (gSystem->GetPathInfo("Datasets/tabs.dat", 
			   &id, &size, &flag, &modtime) == 1) {
    cout << "NOTE: Could not find Datasets/tabs.dat" <<  endl
	 << "      We will retrieve it." << endl;
    return true;
  }
  else if ((time(NULL) - modtime) > MAXREDOWNLOADTIME) {
    cout << "NOTE: Datasets/tabs.dat is older than 24 hours" 
	 <<  endl
	 << "      We will redownload it." << endl;
    return true;
  }
  return false;
}


TString DatasetManager::FindLocalFolder() {
  //Build file names
  TString file("Datasets/");
  file+=fTab;
  file += ".dat";
  
  ifstream is(file);
  TString tmpdataset;
  char tmpline[256];
  while (is) {
    is >> tmpdataset;
    if (tmpdataset[0]=='#') {
      is.getline(tmpline, 255);
    }
    else {
      int idummy;
      double ddummy;
      is >> idummy >> ddummy >> fLocalFolder;
      //cout << "DEBUG: Found local folder" << endl;
      //cout << fLocalFolder << endl;
      return fLocalFolder;
    }    
  }

  return TString("");
}
