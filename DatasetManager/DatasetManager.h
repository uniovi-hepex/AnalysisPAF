///////////////////////////////////////////////////////////////////////
//
//    FILE: DatasetManager.h
//   CLASS: DatasetManager
//  AUTHOR: I. Gonzalez Caballero
//    DATE: January, 2011
//
// CONTENT: This class helps getting the information of a MC dataset
//          and the MiniTree root files in our storages. The 
//          information is retrieved from the google doc page
//          maintained by Javier Fernandez
///////////////////////////////////////////////////////////////////////

#pragma once


// ROOT includes
#include "TString.h"

// STL includes
#include <vector>

// Uncomment the line below to get debug information.
// Set DEBUGDS to to for more debug information
// #define DEBUGDS 1

#ifdef DEBUG
#define DEBUGDS DEBUG
#endif


class DatasetManager {

 public:
  //Get the one and only instance of the DatasetManager
  static DatasetManager* GetInstance();


  ~DatasetManager() {}

  // Get methods
  // * Returns the local base path to the datasets (ex. /hadoop)
  static TString GetLocalBasePath() {return fLocalBasePath;}
  // * Returns the skim being used to search for the dataset files
  TString GetSkim() const {return fSkim;}
  // * Returns the folder/tab being used to search for the dataset
  TString GetTab() const {return fTab;}
  // * Return the name of the dataset currently loaded
  TString GetDatasetName() const {return fDataset;}

  // Set methods
  // * Sets the local base path to the datasets (ex. /hadoop)
  static void SetLocalBasePath(const char* path) {fLocalBasePath=path;}
  // * Sets the skim to use to search for the dataset files
  void SetSkim(const char* skim) { fSkim = skim; fFiles.clear();}
  // * Sets the folder/tab to use to search for the dataset. Downloads the
  //   information if needed. If redownload is set to false, the folder 
  //   information will be redownloaded
  bool SetTab(const char* folder, bool redownload = false);


  // Load Dataset (datasetname) information. Optionally a folder and/or a skim
  // can be provided overridding the current ones. 
  bool LoadDataset(const char* datasetname);


  // Returns information on the dataset just loaded
  // * Events in the dataset
  int GetEventsInTheSample() const {return fEventsInTheSample;}
  // * Dataset cross-section
  double GetCrossSection() const {return fCrossSection;}
  // * Dataset SumWeight
  double GetSumWeights() const {return fSumWeight;}
  // * Local folder path to tab/folder
  static TString GetLocalFolder() {return fLocalFolder;}
  // * Format of the files corresponding to this dataset
  TString GetBaseFileName() const {return fBaseFileName;}
  // * A vector of files (full path) in the dataset... It searches for them
  //   the first time it is called and caches the data.
  const std::vector<TString>& GetFiles();

  //Get the data files providing a relative path to it and file base name
  //It will check for files of the following formats:
  //* [localpath]/relativepath/filebasename_XX.root
  static std::vector<TString> GetRealDataFiles(const char* filebasename,
					       const char* relativepath = 0);

  //Dumps all the information
  void Dump() const;
  

  //Download the information on the folder/tabs available from Google Docs
  bool DownloadAvailableTabs();
  //Download the information on a given folder/tab from Google Docs
  bool DownloadTab(const char* folder);
  //This method writes the script that downloads and processes the information
  //on the available datasets from Google Docs
  TString WriteGetDatatabsScript();
  //Redownload all the files
  void RedownloadFiles();
  //This method writes the script that downloads and processes the data from
  //the Google Doc file
  TString WriteGetDatasetsScript();

  //Try to find the path to the local data
  //Returns an empty string if the guess failed
  static TString GuessLocalBasePath();

  static TString FindLocalFolder();

  
 protected:
  // Base Constructor. Hidden to avoid several instances.
  DatasetManager();
  // Check if the available set of tabs should be downloaded
  bool ShouldDownloadAvailableTabs() const;
  // Tries to guess local folder

 protected:
  // Static members
  static TString fLocalBasePath;    //Local base path guessed from hostname
  static DatasetManager* fInstance; //This object. For the singleton
  static TString fSkim;             //Fixes the skim used for the data.
  static TString fTab;              //Folder (i.e. tab) used to search for the samples
  static unsigned int fLHCRun;      //LHCRun (defaults to 2)
  static TString fLocalFolder;      //Folder associated to tab


  // Data members
  TString fDataset;
  TString fBaseFileName;
  int     fEventsInTheSample;
  double  fCrossSection;
  double  fSumWeight;
  std::vector<TString> fFiles;


};
