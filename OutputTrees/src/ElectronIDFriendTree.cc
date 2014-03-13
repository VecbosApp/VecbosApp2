// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TMath.h>

#include "EgammaTools/include/ElectronEffectiveArea.hh"
#include "Tools/include/LumiReweightingStandAlone.hh"

using namespace std;
using namespace reweight;
using namespace vecbos;

void makeFriend(const char* file, int ismc) {

  cout << "===> creating the friend tree for the file " << file << endl;

  // 2012 run-dependent MC
  LumiReWeighting LumiWeights_Runs190456_To_196531( "Tools/data/pileup/194533-194533-Summer12_DD3_runDep.true.root",
                                                    "Tools/data/pileup/190456-196531-22Jan_v1.69300.true.root",
                                                    "pileup","pileup");
  
  LumiReWeighting LumiWeights_Runs198022_To_203742( "Tools/data/pileup/200519-200519-Summer12_DD3_runDep.true.root",
                                                    "Tools/data/pileup/198022-203742-22Jan_v1.69300.true.root",
                                                    "pileup","pileup");
  
  LumiReWeighting LumiWeights_Runs203777_To_208686( "Tools/data/pileup/206859-206859-Summer12_DD3_runDep.true.root",
                                                    "Tools/data/pileup/203777-208686-22Jan_v1.69300.true.root",
                                                    "pileup","pileup");

  ElectronEffectiveArea EA(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04, ElectronEffectiveArea::kEleEAData2012);

  TFile *pF = TFile::Open(file);
  TTree *pT = (TTree*)pF->Get("electrons/T1");
  
  TString nF(file);
  nF.ReplaceAll(".root","_friend.root");
  TFile *fF = TFile::Open(nF,"recreate");

  UInt_t run;
  Float_t chaPFIso, neuPFIso, phoPFIso, rho, eta[4];
  Float_t trkIso,ecalIso,hcalIso;
  Float_t npu[3];
  pT->SetBranchAddress("run", &run);
  pT->SetBranchAddress("chaPFIso", &chaPFIso);
  pT->SetBranchAddress("neuPFIso", &neuPFIso);
  pT->SetBranchAddress("phoPFIso", &phoPFIso);
  pT->SetBranchAddress("trkIso04", &trkIso);
  pT->SetBranchAddress("ecalIso04", &ecalIso);
  pT->SetBranchAddress("hcalIso04", &hcalIso);
  pT->SetBranchAddress("rho", &rho);
  pT->SetBranchAddress("eta", eta);
  pT->SetBranchAddress("npu", npu);

  fF->mkdir("electrons");
  TTree *fT = new TTree("T1","tree with additional variables");
  Float_t combDetIso, combPfIsoEA;
  Float_t puW;
  fT->Branch("combDetIso",&combDetIso,"combDetIso/F");
  fT->Branch("combPFIsoEA",&combPfIsoEA,"combPFIsoEA/F");
  fT->Branch("puW", &puW, "puW/F");

  for(int i=0; i<pT->GetEntries(); i++) {
    if (i%100000 == 0) std::cout << ">>> Analyzing event # " << i << " / " << pT->GetEntries() << " entries" << std::endl;
     pT->GetEntry(i);

     float abseta=fabs(eta[1]);
     float eff_area_ganh = EA.GetElectronEffectiveArea(abseta);
     
     combPfIsoEA = chaPFIso + max<float>(0.,neuPFIso+phoPFIso - eff_area_ganh*rho);
     combDetIso = trkIso + ecalIso + hcalIso;

     // this is only needed in MC (rundep MC has 3 runs for 3 periods)
     if(ismc) {
       if(run == 194533) puW = LumiWeights_Runs190456_To_196531.weight(npu[1]);
       else if(run == 200519) puW = LumiWeights_Runs198022_To_203742.weight(npu[1]);
       else if(run == 206859) puW = LumiWeights_Runs203777_To_208686.weight(npu[1]);
       else cout << "ERROR! This shoould not happen. Not a run of the run-dependent MC" << endl;
       // remove some unphysical tails
       if(puW > 2) puW = 2.0;
     }
     else puW = 1.0;

     fT->Fill();
  }

  fF->cd("electrons");
  fT->Write();
  fF->Close();

  cout << "DONE. Friend tree is in file: " << nF.Data() << endl;

}

int main(int argc, char* argv[]) {

  char files1[5000], files2[5000], fileb1[5000], fileb2[5000], fileb3[5000], fileb4[5000], fileb5[5000]; 
  sprintf(files1,"data/electrons.root");
  sprintf(files2,"data/dyeerundep.root");
  sprintf(fileb1,"data/zll1fake.root");
  // sprintf(fileb2,"data/fakes.root");
  // sprintf(fileb3,"data/fakes-wlnu1e.root");

  cout << "\t===> DOING FRIEND TREES <===" << endl;

  makeFriend(files1,0);
  makeFriend(files2,1);
  makeFriend(fileb1,0);

}
