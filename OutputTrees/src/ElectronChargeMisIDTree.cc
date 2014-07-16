#include "OutputTrees/include/ElectronChargeMisIDTree.hh"
#include "EgammaTools/include/ElectronIDAlgo.hh"

using namespace vecbos;
using namespace std;

ElectronChargeMisIDTree::ElectronChargeMisIDTree(const char *filename) {

  myFile = new TFile(filename,"RECREATE");
  myFile->mkdir("electrons");
  myTree = new TTree("T1","eleID tree");

  // event
  myTree->Branch("run",      &myRun,     "run/i");
  myTree->Branch("lumi",     &myLS,      "lumi/i");
  myTree->Branch("event",    &myEvent,   "event/l");
  myTree->Branch("vertices", &myNVtx,    "vertices/F");
  myTree->Branch("rho",      &rho_,      "rho/F");
  myTree->Branch("npu",      myNpu,      "npu[3]/F");
  myTree->Branch("mass",     &myMass,    "mass/F");

  // electron basics
  myTree->Branch("Charge",          myCharge,          "Charge[2]/I");
  myTree->Branch("GsfCharge",       myGsfTrackCharge,  "GsfCharge[2]/I");
  myTree->Branch("CtfCharge",       myCtfTrackCharge,  "CtfCharge[2]/I");
  myTree->Branch("ScPixCharge",     myScPixCharge,     "ScPixCharge[2]/I");

  myTree->Branch("eta",     myEta,     "eta[2]/F");
  myTree->Branch("phi",     myPhi,     "phi[2]/F");
  myTree->Branch("pt",      myPt,      "pt[2]/F");

  // electron ID bits (tighter than the loose MVA used for selection)
  myTree->Branch("mvatight",  myMVATight,    "mvatight[2]/I");

  // deltaR with gen electrons
  myTree->Branch("gendeltar", myGenDeltaR,   "gendeltar[2]/F");

}

void ElectronChargeMisIDTree::fillRunInfos(float mass, int run, int lumi, int event, int npu[3], int nvtx) {
  myMass = mass;
  myRun = run;
  myLS = lumi;
  myEvent = event;
  for(int i=0;i<3;i++) myNpu[i]=float(npu[i]);
  myNVtx=nvtx;
}

void ElectronChargeMisIDTree::fillElectronInfos(vecbos::ElectronCollectionPtr electrons) {
  int storedEles=0;
  for(electronptr_iterator eleptr=electrons.begin(); eleptr < electrons.end() && storedEles<3; ++eleptr) {
    Electron *ele = *eleptr; 
    myEta[storedEles] = ele->eta();
    myPhi[storedEles] = ele->phi();
    myPt[storedEles] = ele->pt();
    myCharge[storedEles] = ele->charge();
    myGsfTrackCharge[storedEles] = ele->gsfTrack().charge();
    myCtfTrackCharge[storedEles] = ele->closestTrack().charge();
    myScPixCharge[storedEles] = ele->scPixCharge();

    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    myMVATight[storedEles]  = algo.pass_mva("mva", "tight");
    ++storedEles;
  }
}

void ElectronChargeMisIDTree::fillGenMatch(float deltar1, float deltar2) {
  myGenDeltaR[0] = deltar1;
  myGenDeltaR[1] = deltar2;
}

void ElectronChargeMisIDTree::store() {

  myTree->Fill();
}

void ElectronChargeMisIDTree::save() {

  myFile->cd("electrons");
  myTree->Write();
  myFile->Close();
}
