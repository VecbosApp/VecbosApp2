#ifndef ElectronChargeMisIdTree_H
#define ElectronChargeMisIdTree_H

#include "DataFormats/include/Electron.hh"
#include "DataFormats/include/ElectronFwd.hh"

#include <TFile.h>
#include <TTree.h>

namespace vecbos {

  class ElectronChargeMisIDTree {
  public:

    ElectronChargeMisIDTree(const char *filename);
    ~ElectronChargeMisIDTree();
    
    //! fill the run,lumi, event number, mc match
    void fillRunInfos(float mass, int run, int lumi, int event, int npu[3], int nvtx, float rho);   

    //! fill the electron infos
    void fillElectronInfos(vecbos::ElectronCollectionPtr electron);
    
    //! fill the event in the tree
    void store();
    //! close the TFile
    void save();

  private:

    // event variables
    float myNpu[3];
    UInt_t myRun, myLS;
    ULong64_t myEvent;
    float myMass, myNVtx, myRho;
    
    // electrons variables
    float myEta[2], myPhi[2], myPt[2];
    int myCharge[2], myGsfTrackCharge[2], myCtfTrackCharge[2], myScPixCharge[2];

    TFile *myFile;
    TTree *myTree;

  };

}

#endif
