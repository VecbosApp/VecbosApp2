#ifndef ElectronChargeMisIdTree_H
#define ElectronChargeMisIdTree_H

#include "DataFormats/include/Electron.hh"
#include "DataFormats/include/ElectronFwd.hh"
#include "DataFormats/include/VertexFwd.hh"

#include <TFile.h>
#include <TTree.h>

namespace vecbos {

  class ElectronChargeMisIDTree {
  public:

    ElectronChargeMisIDTree(const char *filename);
    ~ElectronChargeMisIDTree();

    //! set vertex collection and rho to run the ele ID
    void setVerticesAndRho(vecbos::VertexCollection vertices, float rho) { vertices_ = vertices; rho_ = rho; }
    
    //! fill the run,lumi, event number, mc match
    void fillRunInfos(float mass, int run, int lumi, int event, int npu[3], int nvtx);   

    //! fill the electron infos
    void fillElectronInfos(vecbos::ElectronCollectionPtr electron);
    
    //! fill the generator match
    void fillGenMatch(float deltar1, float deltar2);

    //! fill the event in the tree
    void store();
    //! close the TFile
    void save();

  private:

    //! primary vertices collection and rho
    vecbos::VertexCollection vertices_;
    float rho_;

    // event variables
    float myNpu[3];
    UInt_t myRun, myLS;
    ULong64_t myEvent;
    float myMass, myNVtx;
    
    // electrons variables
    float myEta[2], myPhi[2], myPt[2];
    int myCharge[2], myGsfTrackCharge[2], myCtfTrackCharge[2], myScPixCharge[2];
    float myGenDeltaR[2];
    int myMVATight[2];

    TFile *myFile;
    TTree *myTree;

  };

}

#endif
