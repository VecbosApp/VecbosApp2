// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

//#include "Analysis/include/Application.hh"

//#if Application == 1
#include "Analysis/include/DYToEESelection.hh"
//#endif

using namespace std;
using namespace vecbos;

int main(int argc, char* argv[]) {

  char inputFileName[500];
  char outputFileName[500];
  char dataset[150];
  if ( argc < 2 ){
    std::cout << "missing argument: insert at least inputFile with list of root files" << std::endl; 
    std::cout << "VecbosApp inputFile [outputFile] [1=MC,0=data] [dataset]" << std::endl;
    return 1;
  }
  strcpy(inputFileName,argv[1]);
  if (argc < 3 ) strcpy(outputFileName,argv[1]);
  else strcpy(outputFileName,argv[2]);
  int isMC=1;
  if(argc==5) {
    isMC=atoi(argv[3]);
    strcpy(dataset,argv[4]);
  }
  
  // -------------------------
  // Loading the file
  TChain *theChain = new TChain("ntp1");
  char Buffer[500];
  char MyRootFile[2000];
  std::cout << "input: " << inputFileName << std::endl;
  ifstream *inputFile = new ifstream(inputFileName);
  char tmpFileName[256];
  vector<string> filesToRemove;
  while( !(inputFile->eof()) ){
    inputFile->getline(Buffer,500);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer)))
      {
        sscanf(Buffer,"%s",MyRootFile);
        // theChain->Add("root://castorcms/"+TString(MyRootFile)); 
        // theChain->Add("rfio:"+TString(MyRootFile));
        theChain->Add(TString(MyRootFile));
        std::cout << "chaining " << MyRootFile << std::endl;
      }
  }
  inputFile->close();
  delete inputFile;

  bool ismc=true;

  DYToEESelection selection(theChain);
  selection.maxMC(20);
  selection.BeginJob(isMC);
  selection.setJson("JSON/data/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt");
  selection.Loop();
  selection.EndJob();

}
