#include "ElectronFakeRateEstimation.hh"

#include <iostream>
#include <sstream>
#include <string>
#include "TFile.h"
#include "TTree.h"

using namespace std;
using namespace macros;

int main(int argc, char* argv[]) {

  char inname[500], outname[500];

  if( argc < 3) {
    cout << "missing argument: fakerate <inputfilebase> <basename>" << endl;
    return 1;
  }
  strcpy(inname,argv[1]);
  strcpy(outname,argv[2]);

  stringstream infilename, friendfile;
  infilename << "data/" << string(inname) << ".root";
  friendfile << "data/" << string(inname) << "_friend.root";

  cout << "run the FR for triggering electrons on " << infilename.str() << "..." << endl;
  cout << "   friend file = " << friendfile.str() << "..." << endl;
  TFile *file = TFile::Open(infilename.str().c_str());
  TTree *tree = (TTree*)file->Get("electrons/T1");
  
  ElectronFakeRateEstimation analyzer(tree);
  std::string outfileBias(outname);
  analyzer.SetOutputFileName(outfileBias);
  analyzer.calculateFakeRates();
  cout << "DONE triggering electrons." << endl;
  file->Close();

  return 0;
}
