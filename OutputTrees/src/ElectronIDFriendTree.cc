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

  // 2012
  LumiReWeighting LumiWeights( "Tools/data/pileup/puObservedProfile_Summer1253x_Data8TeV.root",
                               "Tools/data/pileup/puObservedProfile_Summer1253x_Data8TeV.root",
                               "generated_pu","target_pu");


  ElectronEffectiveArea EA(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04, ElectronEffectiveArea::kEleEAData2012);

  TFile *pF = TFile::Open(file);
  TTree *pT = (TTree*)pF->Get("electrons/T1");
  
  TString nF(file);
  nF.ReplaceAll(".root","_friend.root");
  TFile *fF = TFile::Open(nF,"recreate");

  Float_t chaPFIso, neuPFIso, phoPFIso, rho, eta[4];
  Float_t trkIso,ecalIso,hcalIso;
  Float_t npu[3];
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

     // this is only needed in MC
     if(ismc) puW = LumiWeights.weight(npu[1]);
     else puW = 1.0;

     fT->Fill();
  }

  fF->cd("electrons");
  fT->Write();
  fF->Close();

  cout << "DONE. Friend tree is in file: " << nF.Data() << endl;

}

int main(int argc, char* argv[]) {

  char files1[500], files2[500], fileb1[500], fileb2[500], fileb3[500], fileb4[500], fileb5[500]; 
  sprintf(files1,"data/electrons.root");
  sprintf(files2,"data/electrons_zeemc.root");
  sprintf(fileb1,"data/fakes-zll1e.root");
  // sprintf(fileb2,"data/fakes.root");
  // sprintf(fileb3,"data/fakes-wlnu1e.root");

  cout << "\t===> DOING FRIEND TREES <===" << endl;

  makeFriend(files1,0);
  makeFriend(files2,1);
  makeFriend(fileb1,0);

}
