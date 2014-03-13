#ifndef ElectronIDTree_H
#define ElectronIDTree_H

#include "DataFormats/include/Electron.hh"
#include "DataFormats/include/VertexFwd.hh"

#include <TFile.h>
#include <TTree.h>

namespace vecbos {

  class ElectronIDTree {
  public:
  
    ElectronIDTree(const char *filename);
    ~ElectronIDTree();

    //! add the electron attributes (see below)
    void addAttributesSignal();
    void addAttributesBackground();
    //! add the splitting categories (see below)
    void addCategories();
    void addMVAs();
    void addMomenta();
    void addElectronIdBits();
    void addDenominatorFakeBits();
    void addIsolations();
    //! add run,lumi, event number (for data)
    void addRunInfos();
    //! add some event global variables
    void addEventInfos();

    //! set vertex collection
    void setVertices(vecbos::VertexCollection vertices) { vertices_ = vertices; }
    //! fill the electron infos
    void fillElectronInfos(vecbos::Electron electron);

    //! fill the fake rate on QCD infos
    void fillFakeRateDenomBits(float leadJetPt, vecbos::Electron electron);

    //! fill the run,lumi, event number, mc match
    void fillRunInfos(int run, int lumi, int event, int npu[3], int nvtx, float rho, int mcmatch);   

    //! fill electron attributes + z mass for the tag and probe
    //! note: when both electrons from Z are probes, the same Z mass is repeated
    void fillAttributesSignal(float zmass, int zeeDec, float genenergy, float gentheta, float genphi);

    //! fill electron attributes + other quantities for background tag and probe
    void fillAttributesBackground(float dphi, float invmass, float met, float pth);
    
    //! fill event infos (for now just the PF MET)
    void fillEventInfos(float met) { myMET = met; }


    //! fill the event in the tree
    void store();
    //! close the TFile
    void save();


  private:

    //! fill the tree with electron id variables
    //! first set
    void fillVariables(int charge, float eleEoPout, float EoPout, float EoP, float HoE, float Deta, float Dphi, float s9s25, float s1s9, float See, float Spp, float fbrem, 
		       int nbrems, int nHits, float dcot, float dist, float phiwidth, float etawidth,
		       float IoEmIoP, float eledeta, float d0, float ip3d, float ip3ds, int kfhits, int kflayers, float kfchi2, float e1x5e5x5, int ecaldriven, bool matchConv,
		       bool iseb, bool isee);
    //! second set
    void fillVariables2(float detacalo, float dphicalo, float sep, float dz, float gsfchi2, float emaxovere, float etopovere, float ebottomovere, float eleftovere, float erightovere,
			float e2ndovere, float e2x5rightovere, float e2x5leftovere, float e2x5topevere, float e2x5bottomovere, 
			float e2x5maxovere, float e1x5overe, float e2x2overe, float e3x3overe, float e5x5overe, float r9, float nclu,
			float phi, float scenergy, float scrawenergy, float scesenergy, float eseedopin);

    //! fill ECAL cluster informations
    void fillClusterInfos(float scEt, float scEta, float scPhi, float EtaSeed, float PhiSeed, float ESeed, float IEtaSeed, float IPhiSeed, float EtaCrySeed, float PhiCrySeed, float IEtaCrySeed, float IPhiCrySeed);

    //! fill the tree with isolation variables
    void fillIsolations(float tkIso, float ecalIso, float hcalIso,
			float combPFisoDeltaBetaCorrected,
			float chaPFiso, float neuPFiso, float phoPFiso);

    //! fill the ID MVAs output
    void fillMVAs(float bdthww, float bdthzz);


    //! fill the electron ID bits
    enum cuts_wp { cuts_veto=0, cuts_loose, cuts_medium, custs_tight };
    enum mva_wp { mva_loose=0, mva_tihgt };

    //! fill the splitting categories of the PDFs
    void fillCategories(int iecal, int iptbin, int iclass, int nbr);
    //! fill the different estimation of the momentum (ECAL - GSF track - comb - comb calibrated)
    void fillMomenta(vecbos::Electron electron);

  private:

    float myEEleoPout, myEseedoPout, myEoP,myHoE,myDeta,myDphi,mys9s25,mys1s9,mySee,mySpp,myFbrem, myPhiWidth, myEtaWidth, myEseedoPin;
    float myIoEoIoP, myEleDeta, myD0, myIP3d, myIP3dSig, myKFChi2, myE1x5E5x5, myPreShowerOverRaw;
    float myNbrems, myKFHits, myKFLayers, myMissHits;
    bool myMatchConv, myEcalDriven;
    float myDetaCalo, myDphiCalo, mySep, myDZ, myGSFChi2;
    float mySeedEMax,mySeedETop,mySeedEBottom,mySeedELeft,mySeedERight,mySeedE2nd,mySeedE2x5Right,mySeedE2x5Left,mySeedE2x5Top,mySeedE2x5Bottom;
    float mySeedE2x5Max,mySeedE1x5,mySeedE2x2,mySeedE3x3,mySeedE5x5,myR9,myNClusters,myOneMinusSeedE1x5OverE5x5;
    float myDist, myDcot, myCharge;
    float myEta, myPhi, myPt, mySCEnergy, mySCRawEnergy, myEsenergy, myEcalEnergy;
    float myNpu[3];
    UInt_t myRun, myLS, myMCMatch;
    ULong64_t myEvent;
    bool myIsEB, myIsEE;
    float myScEt,myScEta,myScPhi,myEtaSeed,myPhiSeed,myESeed,myIEtaSeed,myIPhiSeed,myEtaCrySeed,myPhiCrySeed,myIEtaCrySeed,myIPhiCrySeed;
    bool myEventNumberParity;
    float myZmass, myZDec, myWeight;
    float myGeneratedEnergy,myGeneratedTheta,myGeneratedPhi;
    float myMET;

    int myCutBasedId[4], myCutBasedIdOnlyID[4], myCutBasedIdOnlyIso[4], myCutBasedIdOnlyConv[4];
    int myBDTId[2], myBDTIdOnlyID[2], myBDTIdOnlyIso[2], myBDTIdOnlyConv[2];
    bool myDenomFake;
    float myLeadJetPt;


    float myQCDDeltaphi;
    float myQCDInvmass;
    float myQCDMet;
    float myQCDPtHat;

    int myiecal;
    int myiptbin;
    int myiclass;
    int mynbrem;

    float myTrkIso04;
    float myEcalIso04;
    float myHcalIso04;
    float myPFCandCombinedIsoDeltaBetaCorrected;
    float myPFCandChargedIso, myPFCandNeutralIso, myPFCandPhotonIso;

    float myNVtx;
    float myRho;
    float myBdtTrg, myBdtNonTrg;

    //! [0,1,2,3,4] = calib comb, comb, ECAL, GSF track mean, GSF track mode
    float myElePt[5], myEleEta[5], myElePhi[5];

    TFile *myFile;
    TTree *myTree;

    //! primary vertices collection
    vecbos::VertexCollection vertices_;
    
  };

}

#endif

