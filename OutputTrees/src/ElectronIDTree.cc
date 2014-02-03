#include "OutputTrees/include/ElectronIDTree.hh"
#include "EgammaTools/include/ElectronIDAlgo.hh"

using namespace vecbos;
using namespace std;

ElectronIDTree::ElectronIDTree(const char *filename) {

  myFile = new TFile(filename,"RECREATE");
  myFile->mkdir("electrons");
  myTree = new TTree("T1","eleID tree");

  // electron basics
  myTree->Branch("Charge",          &myCharge,          "Charge/F");
  myTree->Branch("IsEB",            &myIsEB,            "IsEB/O");
  myTree->Branch("IsEE",            &myIsEE,            "IsEE/O");

  // ele ID vars
  myTree->Branch("EEleoPout",       &myEEleoPout,       "EEleoPout/F");
  myTree->Branch("ESeedoPout",      &myEseedoPout,      "ESeedoPout/F");
  myTree->Branch("EoP",             &myEoP,             "EoP/F");
  myTree->Branch("EoPin",           &myEseedoPin,       "EoPin/F");
  myTree->Branch("IoEmIoP",         &myIoEoIoP,         "IoEmIoP/F");
  myTree->Branch("HoE",             &myHoE,             "HoE/F");
  myTree->Branch("eledeta",         &myEleDeta,         "eledeta/F");
  myTree->Branch("deta",            &myDeta,            "deta/F");
  myTree->Branch("dphi",            &myDphi,            "dphi/F");
  myTree->Branch("detacalo",        &myDetaCalo,        "detacalo/F");
  myTree->Branch("dphicalo",        &myDphiCalo,        "dphicalo/F");
  myTree->Branch("fbrem",           &myFbrem,           "fbrem/F");
  myTree->Branch("nbrems",          &myNbrems,          "nbrems/F");
  myTree->Branch("missHits",        &myMissHits,        "missHits/F");
  myTree->Branch("dist",            &myDist,            "dist/F");
  myTree->Branch("dcot",            &myDcot,            "dcot/F");
  myTree->Branch("d0",              &myD0,              "d0/F");
  myTree->Branch("dz",              &myDZ,              "dz/F");
  myTree->Branch("ip3d",            &myIP3d,            "ip3d/F");
  myTree->Branch("ip3ds",           &myIP3dSig,         "ip3ds/F");
  myTree->Branch("kfhits",          &myKFHits,          "kfhits/F");
  myTree->Branch("kflayers",        &myKFLayers,        "kflayers/F");
  myTree->Branch("kfchi2",          &myKFChi2,          "kfchi2/F");
  myTree->Branch("gsfchi2",         &myGSFChi2,         "gsfchi2/F");

  // cluster shapes
  myTree->Branch("s9s25",           &mys9s25,           "s9s25/F");
  myTree->Branch("phiwidth",        &myPhiWidth,        "phiwidth/F");
  myTree->Branch("etawidth",        &myEtaWidth,        "etawidth/F");
  myTree->Branch("see",             &mySee,             "see/F");
  myTree->Branch("sep",             &mySep,             "sep/F");
  myTree->Branch("spp",             &mySpp,             "spp/F");
  myTree->Branch("NClusters",       &myNClusters,       "NClusters/F");
  myTree->Branch("e1x5e5x5",        &myE1x5E5x5,        "e1x5e5x5/F");
  // cluster positions/energies
  myTree->Branch("scEt",            &myScEt,            "scEt/F");
  myTree->Branch("scEta",           &myScEta,           "scEta/F");
  myTree->Branch("scPhi",           &myScPhi,           "scPhi/F");
  myTree->Branch("SCRawEnergy",     &mySCRawEnergy,     "SCRawEnergy/F"); 
  myTree->Branch("ecalenergy",      &myEcalEnergy,      "ecalenergy/F"); // this is dummy: not in vecbos
  myTree->Branch("esenergy",        &myEsenergy,        "esenergy/F");
  myTree->Branch("PreShowerOverRaw", &myPreShowerOverRaw, "PreShowerOverRaw/F");
  // seed basic cluster
  myTree->Branch("EtaSeed",         &myEtaSeed,         "EtaSeed/F");
  myTree->Branch("PhiSeed",         &myPhiSeed,         "PhiSeed/F");
  myTree->Branch("ESeed",           &myESeed,           "ESeed/F");
  myTree->Branch("IEtaSeed",         &myIEtaSeed,       "IEtaSeed/F");
  myTree->Branch("IPhiSeed",         &myIPhiSeed,       "IPhiSeed/F");
  // seed crystal
  myTree->Branch("EtaCrySeed",       &myEtaCrySeed,     "EtaCrySeed/F");
  myTree->Branch("PhiCrySeed",       &myPhiCrySeed,     "PhiCrySeed/F");
  myTree->Branch("IEtaCrySeed",      &myIEtaCrySeed,    "IEtaCrySeed/F");
  myTree->Branch("IPhiCrySeed",      &myIPhiCrySeed,    "IPhiCrySeed/F");

  // cluster shapes
  myTree->Branch("EMaxSeed",   &mySeedEMax,   "EMaxSeed/F"); 
  myTree->Branch("ETopSeed",   &mySeedETop,   "ETopSeed/F"); 
  myTree->Branch("EBottomSeed",&mySeedEBottom,"EBottomSeed/F"); 
  myTree->Branch("ELeftSeed",  &mySeedELeft,  "ELeftSeed/F"); 
  myTree->Branch("ERightSeed", &mySeedERight, "ERightSeed/F"); 
  myTree->Branch("E2ndSeed",   &mySeedE2nd,   "E2ndSeed/F"); 
  myTree->Branch("E2x5RightSeed",  &mySeedE2x5Right, "E2x5RightSeed/F"); 
  myTree->Branch("E2x5LeftSeed",   &mySeedE2x5Left,  "E2x5LeftSeed/F"); 
  myTree->Branch("E2x5TopSeed",&mySeedE2x5Top,"E2x5TopSeed/F"); 
  myTree->Branch("E2x5BottomSeed", &mySeedE2x5Bottom, "E2x5BottomSeed/F"); 
  myTree->Branch("E2x5MaxSeed",&mySeedE2x5Max,"E2x5MaxSeed/F"); 
  myTree->Branch("E1x5Seed",   &mySeedE1x5,   "E1x5Seed/F"); 
  myTree->Branch("E2x2Seed",   &mySeedE2x2,   "E2x2Seed/F"); 
  myTree->Branch("E3x3Seed",   &mySeedE3x3,   "E3x3Seed/F"); 
  myTree->Branch("E5x5Seed",   &mySeedE5x5,   "E5x5Seed/F"); 
  myTree->Branch("OneMinusSeedE1x5OverE5x5", &myOneMinusSeedE1x5OverE5x5, "OneMinusSeedE1x5OverE5x5/F");
  myTree->Branch("R9",              &myR9,              "R9/F");
  myTree->Branch("matchConv",       &myMatchConv,       "matchConv/O");
  myTree->Branch("ecaldriven",      &myEcalDriven,      "ecaldriven/O");
}

ElectronIDTree::~ElectronIDTree() {
  delete myFile;
}

void ElectronIDTree::addAttributesSignal() {
  myTree->Branch("mass",       &myZmass,       "mass/F");
  myTree->Branch("zdec",       &myZDec,        "zdec/F");
  myTree->Branch("etrue",      &myGeneratedEnergy, "etrue/F");
  myTree->Branch("thetatrue",  &myGeneratedTheta,  "thetatrue/F");
  myTree->Branch("phitrue",    &myGeneratedPhi,    "phitrue/F");
}

void ElectronIDTree::addElectronIdBits() {

  myTree->Branch("wp_cuts",      myCutBasedId,         "wp_cuts[4]/I");
  myTree->Branch("wp_cuts_id",   myCutBasedIdOnlyID,   "wp_cuts_id[4]/I");
  myTree->Branch("wp_cuts_iso",  myCutBasedIdOnlyIso,  "wp_cuts_iso[4]/I");
  myTree->Branch("wp_cuts_conv", myCutBasedIdOnlyConv, "wp_cuts_conv[4]/I");

  myTree->Branch("wp_mva",      myBDTId,         "wp_mva[4]/I");
  myTree->Branch("wp_mva_id",   myBDTIdOnlyID,   "wp_mva_id[4]/I");
  myTree->Branch("wp_mva_iso",  myBDTIdOnlyIso,  "wp_mva_iso[4]/I");
  myTree->Branch("wp_mva_conv", myBDTIdOnlyConv, "wp_mva_conv[4]/I");

}

void ElectronIDTree::addDenominatorFakeBits() {
  myTree->Branch("DenomFake",          &myDenomFake,             "DenomFake/O");
  myTree->Branch("leadJetPt",          &myLeadJetPt,             "leadJetPt/F");
}

void ElectronIDTree::addRunInfos() {
  myTree->Branch("run",     &myRun,     "run/i");
  myTree->Branch("lumi",    &myLS,      "lumi/i");
  myTree->Branch("event",   &myEvent,   "event/l");
  myTree->Branch("EventNumberParity",  &myEventNumberParity,  "EventNumberParity/O");
  myTree->Branch("vertices", &myNVtx,   "vertices/F");
  myTree->Branch("rho",      &myRho,    "rho/F");
  myTree->Branch("npu",      myNpu,     "npu[3]/F");
  myTree->Branch("mcmatch",  &myMCMatch,"mcmatch/O");
}

void ElectronIDTree::addEventInfos() {
  myTree->Branch("met",  &myMET,   "met/F");
}

void ElectronIDTree::addAttributesBackground() {

  myTree->Branch("qcdDeltaphi",    &myQCDDeltaphi,    "qcdDeltaphi/F");
  myTree->Branch("qcdInvmass",     &myQCDInvmass,     "qcdInvmass/F");
  myTree->Branch("qcdMet",         &myQCDMet,         "qcdMet/F");
  myTree->Branch("qcdPtHat",       &myQCDPtHat,       "qcdPtHat/F");
}

void ElectronIDTree::addCategories() {

  myTree->Branch("iecal",      &myiecal,      "iecal/I");
  myTree->Branch("iptbin",     &myiptbin,     "iptbin/I");
  myTree->Branch("iclass",     &myiclass,     "iclass/I");
  myTree->Branch("nbrem",      &mynbrem,      "nbrem/I");
}

void ElectronIDTree::addIsolations() {
  myTree->Branch("trkIso04",  &myTrkIso04,    "trkIso04/F");
  myTree->Branch("ecalIso04", &myEcalIso04,   "ecalIso04/F");
  myTree->Branch("hcalIso04", &myHcalIso04,   "hcalIso04/F");
  myTree->Branch("combPFIsoDBeta", &myPFCandCombinedIsoDeltaBetaCorrected, "combPFIsoDBeta/F");
  myTree->Branch("chaPFIso",     &myPFCandChargedIso,     "chPFIso/F");
  myTree->Branch("neuPFIso",     &myPFCandNeutralIso,     "neuPFIso/F");
  myTree->Branch("phoPFIso",     &myPFCandPhotonIso,      "phoPFIso/F");
}

void ElectronIDTree::addMVAs() {
  myTree->Branch("bdttrg",     &myBdtTrg,    "bdthww/F");
  myTree->Branch("bdtnontrg",  &myBdtNonTrg, "bdtnontrg/F");
}

void ElectronIDTree::addMomenta() {
  myTree->Branch("pt",  &myElePt,  "pt[4]/F");
  myTree->Branch("eta", &myEleEta, "eta[4]/F");
  myTree->Branch("phi", &myElePhi, "phi[4]/F");
}

void ElectronIDTree::store() {

  myTree->Fill();
}

void ElectronIDTree::save() {

  myFile->cd("electrons");
  myTree->Write();
  myFile->Close();
}

void ElectronIDTree::fillElectronInfos(Electron electron) {
  fillVariables(electron.charge(),
		electron.eEleClusterOverPout(),
		electron.eSeedClusterOverPout(),
		electron.eSuperClusterOverP(),
		electron.hcalOverEcal(),
		electron.deltaEtaSuperClusterTrackAtVtx(),
		electron.deltaPhiSuperClusterTrackAtVtx(),
		electron.superCluster().e3x3() / electron.superCluster().e5x5(),
		electron.superCluster().eMax() / electron.superCluster().e3x3(),
		electron.sigmaIetaIeta(),
		electron.sigmaIphiIphi(),
		electron.fbrem(),
		electron.numberOfBrems(),
		electron.gsfTrack().trackerExpectedHitsInner(),
		electron.convDcot(),
		electron.convDist(),
		electron.superCluster().phiWidth(),
		electron.superCluster().etaWidth(),
		1./electron.superCluster().energy()-1./electron.gsfTrack().p(),
		electron.deltaEtaEleClusterTrackAtCalo(),
		electron.gsfTrack().d0(),
		electron.gsfTrack().ip3D(),
		electron.gsfTrack().sip3D(),
		electron.closestTrack().numberOfValidHits(),
		electron.closestTrack().getHitPattern().trackerLayersWithMeasurement,
		electron.closestTrack().normalizedChi2(),
		electron.superCluster().e1x5()/electron.superCluster().e5x5(),
		electron.ecalDrivenSeed(),
		electron.hasMatchedConv(),
		electron.isEB(),
		electron.isEE() );

  fillVariables2(electron.deltaEtaSeedClusterTrackAtCalo(),
		 electron.deltaPhiSeedClusterTrackAtCalo(),
		 electron.superCluster().sigmaIetaIphi(),
		 electron.gsfTrack().dz(),
		 electron.gsfTrack().normalizedChi2(),
		 electron.superCluster().eMax() / electron.superCluster().energy(),
		 electron.superCluster().eTop() / electron.superCluster().energy(),
		 electron.superCluster().eBottom() / electron.superCluster().energy(),
		 electron.superCluster().eLeft() / electron.superCluster().energy(),
		 electron.superCluster().eRight() / electron.superCluster().energy(),
		 electron.superCluster().e2nd() / electron.superCluster().energy(),
		 electron.superCluster().e2x5Right() / electron.superCluster().energy(),
		 electron.superCluster().e2x5Left() / electron.superCluster().energy(),
		 electron.superCluster().e2x5Top() / electron.superCluster().energy(),
		 electron.superCluster().e2x5Bottom() / electron.superCluster().energy(),
		 electron.superCluster().e2x5Max() / electron.superCluster().energy(),
		 electron.superCluster().e1x5() / electron.superCluster().energy(),
		 electron.superCluster().e2x2() / electron.superCluster().energy(),
		 electron.superCluster().e3x3() / electron.superCluster().energy(),
		 electron.superCluster().e5x5() / electron.superCluster().energy(),
		 electron.r9(),
		 electron.superCluster().clustersSize(),
		 electron.phi(),
		 electron.superCluster().energy(),
		 electron.superCluster().rawEnergy(),
		 electron.superCluster().preshowerEnergy(),
		 electron.eSeedClusterOverP() );

  fillIsolations(electron.dr04TkSumPt(),
		 electron.dr04EcalRecHitSumEt(),
		 electron.dr04HcalTowerSumEt(),
		 electron.dr04pfIsolationDeltaBetaCorr(),
		 electron.dr04pfChargedSumPt(),
		 electron.dr04pfPhotonSumEt(),
		 electron.dr04pfNeutralHadronSumEt() );

  fillMVAs(electron.mvaTriggering(),
	   electron.mvaNonTriggering() );

  ElectronIDAlgo algo(myRho,vertices_);
  algo.setElectron(electron);

  vector<string> cuts_wps;
  cuts_wps.push_back("veto");
  cuts_wps.push_back("loose");
  cuts_wps.push_back("medium");
  cuts_wps.push_back("tight");
  for(int wp=0; wp<4; ++wp) {
    myCutBasedId[wp]         = algo.pass_cuts         (cuts_wps[wp]);
    myCutBasedIdOnlyID[wp]   = algo.pass_cuts_id      (cuts_wps[wp]);
    myCutBasedIdOnlyIso[wp]  = algo.pass_cuts_iso     (cuts_wps[wp]);
    myCutBasedIdOnlyConv[wp] = algo.pass_cuts_convrej (cuts_wps[wp]);
  }

  vector<string> mva_wps;
  mva_wps.push_back("loose");
  mva_wps.push_back("tight");
  for(int wp=0; wp<2; ++wp) {
    myBDTId[wp]         = algo.pass_mva         ("mva", mva_wps[wp]);
    myBDTIdOnlyID[wp]   = algo.pass_mva_id      ("mva", mva_wps[wp]);
    myBDTIdOnlyIso[wp]  = algo.pass_mva_iso     (mva_wps[wp]);
    myBDTIdOnlyConv[wp] = algo.pass_mva_convrej (mva_wps[wp]);
  }
  
  fillMomenta(electron);

}

void ElectronIDTree::fillVariables(int charge, float eleEoPout, float EseedoPout, float EoP, float HoE, float Deta, float Dphi, float s9s25, float s1s9, float See, float Spp, float fbrem, 
                                 int nbrems, int nHits, float dcot, float dist, float phiwidth, float etawidth,
                                 float IoEmIoP, float eledeta, float d0, float ip3d, float ip3ds, int kfhits, int kflayers, float kfchi2, float e1x5e5x5, int ecaldriven, bool matchConv, 
                                 bool iseb, bool isee) {
  myCharge=charge;
  myEEleoPout=eleEoPout;
  myEseedoPout=EseedoPout;
  myEoP=EoP;
  myHoE=HoE;
  myDeta=Deta;
  myDphi=Dphi;
  mys9s25=s9s25;
  mys1s9=s1s9;
  mySee=See;
  mySpp=Spp;
  myFbrem=fbrem;
  myNbrems=float(nbrems);
  myMissHits=float(nHits);
  myDist=dist;
  myDcot=dcot;
  myPhiWidth=phiwidth;
  myEtaWidth=etawidth;
  myIoEoIoP=IoEmIoP;
  myEleDeta=eledeta;
  myD0=d0;
  myIP3d=ip3d;
  myIP3dSig=ip3ds;
  myKFHits=float(kfhits);
  myKFLayers=float(kflayers);
  myKFChi2=kfchi2;
  myE1x5E5x5=e1x5e5x5;
  myEcalDriven=(ecaldriven==1) ? true : false;
  myMatchConv=matchConv;
  myIsEB=iseb;
  myIsEE=isee;
}

void ElectronIDTree::fillVariables2(float detacalo, float dphicalo, float sep, float dz, float gsfchi2, float emax, float etop, float ebottom, float eleft, float eright,
                                  float e2nd, float e2x5right, float e2x5left, float e2x5top, float e2x5bottom, 
                                  float e2x5max, float e1x5, float e2x2, float e3x3, float e5x5, float r9, float nclu,
                                  float phi, float scenergy, float scrawenergy, float scesenergy, float eseedopin) {
  myDetaCalo=detacalo;
  myDphiCalo=dphicalo;
  mySep=sep;
  myDZ=dz;
  myGSFChi2=gsfchi2;
  mySeedEMax=emax;
  mySeedETop=etop;
  mySeedEBottom=ebottom;
  mySeedELeft=eleft;
  mySeedERight=eright;
  mySeedE2nd=e2nd;
  mySeedE2x5Right=e2x5right;
  mySeedE2x5Left=e2x5left;
  mySeedE2x5Top=e2x5top;
  mySeedE2x5Bottom=e2x5bottom;
  mySeedE2x5Max=e2x5max;
  mySeedE1x5=e1x5;
  mySeedE2x2=e2x2;
  mySeedE3x3=e3x3;
  mySeedE5x5=e5x5;
  myOneMinusSeedE1x5OverE5x5=1-e1x5/e5x5;
  myR9=r9;
  myNClusters=nclu;
  myPhi=phi;
  mySCEnergy=scenergy;
  mySCRawEnergy=scrawenergy;
  myEcalEnergy=-999.;
  myEsenergy=scesenergy;
  myEseedoPin=eseedopin;
  myPreShowerOverRaw=scesenergy/scrawenergy;
}

void ElectronIDTree::fillClusterInfos(float scEt, float scEta, float scPhi, float EtaSeed, float PhiSeed, float ESeed, float IEtaSeed, float IPhiSeed, 
				      float EtaCrySeed, float PhiCrySeed, float IEtaCrySeed, float IPhiCrySeed) {
  myScEt=scEt;
  myScEta=scEta;
  myScPhi=scPhi;
  myEtaSeed=EtaSeed;
  myPhiSeed=PhiSeed;
  myESeed=ESeed;
  myIEtaSeed=IEtaSeed;
  myIPhiSeed=IPhiSeed;
  myEtaCrySeed=EtaCrySeed;
  myPhiCrySeed=PhiCrySeed;
  myIEtaCrySeed=IEtaCrySeed;
  myIPhiCrySeed=IPhiCrySeed;
}

void ElectronIDTree::fillIsolations(float trkIso, float ecalIso, float hcalIso,
				    float combPFisoDeltaBetaCorrected,
				    float chaPFiso, float neuPFiso, float phoPFiso) {
  myTrkIso04=trkIso;
  myEcalIso04=ecalIso;
  myHcalIso04=hcalIso;
  myPFCandCombinedIsoDeltaBetaCorrected=combPFisoDeltaBetaCorrected;
  myPFCandChargedIso=chaPFiso;
  myPFCandNeutralIso=neuPFiso;
  myPFCandPhotonIso=phoPFiso;
}

void ElectronIDTree::fillAttributesSignal(float zmass, int zdec, float genenergy, float gentheta, float genphi) {
  myZmass=zmass;
  myZDec=float(zdec);
  myGeneratedEnergy=genenergy;
  myGeneratedTheta=gentheta;
  myGeneratedPhi=genphi;
}

void ElectronIDTree::fillAttributesBackground(float deltaphi, float invmass, float met, float pth) {
  myQCDDeltaphi=deltaphi;
  myQCDInvmass=invmass;
  myQCDMet=met;
  myQCDPtHat=pth;
}

void ElectronIDTree::fillCategories(int iecal, int iptbin, int iclass, int nbr) {
  myiecal=iecal;
  myiptbin=iptbin;
  myiclass=iclass;
  mynbrem=nbr;
}

void ElectronIDTree::fillMVAs(float bdthww, float bdthzz) {
  myBdtTrg=bdthww;
  myBdtNonTrg=bdthzz;
}

void ElectronIDTree::fillMomenta(Electron electron) {
  /// calibrated electron momentum
  myElePt[0]  = electron.calibratedP4().Pt();
  myEleEta[0] = electron.calibratedP4().Eta();
  myElePhi[0] = electron.calibratedP4().Phi();
  /// electron momentum
  myElePt[1]  = electron.pt();
  myEleEta[1] = electron.eta();
  myElePhi[1] = electron.phi();
  /// SuperCluster momentum (direction from the electron)
  myElePt[2]  = electron.superCluster().et();
  myEleEta[2] = electron.eta();
  myElePhi[2] = electron.phi();
  /// GSF track momentum
  myElePt[3]  = electron.gsfTrack().pt();
  myEleEta[3] = electron.gsfTrack().eta();
  myElePhi[3] = electron.gsfTrack().phi();
}

void ElectronIDTree::fillFakeRateDenomBits(float leadJetPt, Electron electron) {
  ElectronIDAlgo algo(myRho,vertices_);
  algo.setElectron(electron);
  myDenomFake = algo.pass_hlt();
  myLeadJetPt = leadJetPt;
}

void ElectronIDTree::fillRunInfos(int run, int lumi, int event, int npu[3], int nvtx, float rho, int mcmatch) {
  myRun = run;
  myLS = lumi;
  myEvent = event;
  myEventNumberParity = (event%2==0) ? true : false;
  myWeight=1.0;
  for(int i=0;i<3;i++) myNpu[i]=float(npu[i]);
  myNVtx=nvtx;
  myRho=rho;
  myMCMatch = (mcmatch==1) ? true : false;
}
