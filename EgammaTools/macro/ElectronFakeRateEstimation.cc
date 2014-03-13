#include <iostream>
#include <stdio.h>
#include <math.h>
#include "TH1F.h"

#include "ElectronFakeRateEstimation.hh"
#include "../../Math/include/EfficiencyEvaluator.hh"

using namespace macros;
using namespace std;

void ElectronFakeRateEstimation::calculateFakeRates() {

  std::vector<TString> EgammaEleID;
  // this is for triggering electrons
  EgammaEleID.push_back("wp_cuts_veto"); // veto
  EgammaEleID.push_back("wp_cuts_loose"); // loose 
  EgammaEleID.push_back("wp_cuts_medium"); // medium
  EgammaEleID.push_back("wp_cuts_tight"); // tight
  EgammaEleID.push_back("wp_mva_loose"); // loose (hzz)
  EgammaEleID.push_back("wp_mva_tight"); // tight (hww)

  // -----------------------------------------------------------------------
  // study vs eta

  Float_t LowerEta[5];
  LowerEta[0]=0.0;
  LowerEta[1]=1.0;
  LowerEta[2]=1.479;  
  LowerEta[3]=2.0;
  LowerEta[4]=2.5;
  TH1F *RecoEta         = new TH1F( "RecoEta",          "reconstructed #eta", 4, LowerEta);
  TH1F *RecoEtaHighPt   = new TH1F( "RecoEtaHighPt",    "reconstructed #eta", 4, LowerEta);
  TH1F *RecoEtaLowPt    = new TH1F( "RecoEtaLowPt",     "reconstructed #eta", 4, LowerEta);
  
  std::vector<TH1F*> EleEta;
  for (int i=0;i<(int)EgammaEleID.size();++i) {
    TH1F* aHisto = new TH1F( EgammaEleID[i]+"_Eta",   "selection#eta", 4, LowerEta);
    EleEta.push_back(aHisto);
  }

  // eta, high pT
  std::vector<TH1F*> EleEtaHighPt;
  for (int i=0;i<(int)EgammaEleID.size();++i) {
    TH1F* aHisto = new TH1F( EgammaEleID[i]+"_EtaHighPt",   "selection#eta", 4, LowerEta);   
    EleEtaHighPt.push_back(aHisto);
  }

  // eta, low pT
  std::vector<TH1F*> EleEtaLowPt;
  for (int i=0;i<(int)EgammaEleID.size();++i) {
    TH1F* aHisto = new TH1F( EgammaEleID[i]+"_EtaLowPt",   "selection#eta", 4, LowerEta);  
    EleEtaLowPt.push_back(aHisto);
  }

  // -----------------------------------------------------------------------
  // study vs pT
  Float_t LowerPt[11] = {0.0,5.0,7.0,10.0,14.0,20.0,25.0,30.0,40.0,50.0,80.0};
  TH1F *RecoPtBarrel   = new TH1F( "RecoPtBarrel",    "reconstructed p_{T} (GeV)", 10, LowerPt);
  TH1F *RecoPtEndcap   = new TH1F( "RecoPtEndcap",    "reconstructed p_{T} (GeV)", 10, LowerPt);

  std::vector<TH1F*> ElePtBarrel;
  for (int i=0;i<(int)EgammaEleID.size();++i) {
    TH1F* aHisto = new TH1F( EgammaEleID[i]+"_PtBarrel", "selection#eta",   10, LowerPt );
    ElePtBarrel.push_back(aHisto);
  }

  std::vector<TH1F*> ElePtEndcap;
  for (int i=0;i<(int)EgammaEleID.size();++i) {
    TH1F* aHisto = new TH1F( EgammaEleID[i]+"_PtEndcap", "selection#eta",   10, LowerPt );
    ElePtEndcap.push_back(aHisto);
  }

  // -----------------------------------------------------------------------
  // study vs PU
  Float_t LowerPU[11];
  LowerPU[0] = 1;
  LowerPU[1] = 6;
  LowerPU[2] = 10;
  LowerPU[3] = 13;
  LowerPU[4] = 15;
  LowerPU[5] = 17;
  LowerPU[6] = 19;
  LowerPU[7] = 21;
  LowerPU[8] = 26;
  LowerPU[9] = 36;
  LowerPU[10] = 50;

  TH1F *RecoPUBarrel   = new TH1F( "RecoPUBarrel",   "reconstructed nPU", 10, LowerPU);    // this is done to subsplit 
  TH1F *RecoPUEndcap   = new TH1F( "RecoPUEndcap",   "reconstructed nPU", 10, LowerPU);

  // barrel
  std::vector<TH1F*> ElePUBarrel;
  for (int i=0;i<(int)EgammaEleID.size();++i) {
    TH1F* aHisto = new TH1F( EgammaEleID[i]+"_PUBarrel", "selection#eta",   10, LowerPU );
    ElePUBarrel.push_back(aHisto);
  }

  // endcap
  std::vector<TH1F*> ElePUEndcap;
  for (int i=0;i<(int)EgammaEleID.size();++i) {
    TH1F* aHisto = new TH1F( EgammaEleID[i]+"_PUEndcap", "selection#eta",   10, LowerPU );
    ElePUEndcap.push_back(aHisto);
  }

  // loop on events
  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  std::cout << "Number of entries = " << nentries << std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%100000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;

    // to remove WZ events
    if(met > 25) continue;

    // fill the denominator: take only the highest pT denominator candidate
    float etaFake = fabs(eta[1]);
    float etFake  = pt[1];
    bool isInEB   = fabs(eta[1])<1.479;
    bool isInEE   = !isInEB;

    // filling
    if (isInEB) {
      RecoPtBarrel -> Fill(etFake);  //, theWeight);
      RecoPUBarrel -> Fill(vertices);  //, theWeight);
    }
    if (isInEE) {
      RecoPtEndcap -> Fill(etFake);  //, theWeight);
      RecoPUEndcap -> Fill(vertices);  //, theWeight);
    }

    // fill the numerator(s)
    for(int iwpcuts=0; iwpcuts<4; iwpcuts++) {
      if(wp_cuts_id[iwpcuts] && wp_cuts_iso[iwpcuts] && wp_cuts_conv[iwpcuts]) {
	if (isInEB) { 
	  ElePtBarrel[iwpcuts] ->Fill(etFake);
	  ElePUBarrel[iwpcuts] ->Fill(vertices);
	} else {
	  ElePtEndcap[iwpcuts] ->Fill(etFake);
	  ElePUEndcap[iwpcuts] ->Fill(vertices);
	}
      }
    }
    for(int iwpmva=0; iwpmva<2; iwpmva++) {
      if(wp_mva_id[iwpmva] && wp_mva_iso[iwpmva] && wp_mva_conv[iwpmva]) {
	if (isInEB) { 
	  ElePtBarrel[iwpmva+4] ->Fill(etFake);
	  ElePUBarrel[iwpmva+4] ->Fill(vertices);
	} else {
	  ElePtEndcap[iwpmva+4] ->Fill(etFake);
	  ElePUEndcap[iwpmva+4] ->Fill(vertices);
	}
      }
    }

  } // loop over events


  // saving efficiency histos
  // === as a function of eta ===
  char filename[500];
  sprintf(filename,"%s-EleMisidEta.root",outname_.c_str());
  EfficiencyEvaluator ElectronEffEta(filename);
  ElectronEffEta.AddNumerator(RecoEta);
  for (int icut=0;icut<(int)EgammaEleID.size();++icut){
    ElectronEffEta.AddNumerator(EleEta[icut]);
  }

  ElectronEffEta.SetDenominator(RecoEta);
  ElectronEffEta.ComputeEfficiencies();
  ElectronEffEta.SetTitle("fake rate vs eta");
  ElectronEffEta.SetXaxisTitle("electron #eta");
  ElectronEffEta.SetYaxisTitle("Fake rate");
  ElectronEffEta.SetYaxisMin(0.0);
  ElectronEffEta.Write();

  // === as a function of pt ===
  sprintf(filename,"%s-EleMisidPtBarrel.root",outname_.c_str());
  EfficiencyEvaluator ElectronEffPtBarrel(filename);
  ElectronEffPtBarrel.AddNumerator(RecoPtBarrel);
  for (int icut=0;icut<(int)EgammaEleID.size();++icut){
    ElectronEffPtBarrel.AddNumerator(ElePtBarrel[icut]);
  }

  ElectronEffPtBarrel.SetDenominator(RecoPtBarrel);
  ElectronEffPtBarrel.ComputeEfficiencies();
  ElectronEffPtBarrel.SetTitle("fake rate vs pT");
  ElectronEffPtBarrel.SetXaxisTitle("electron pT");
  ElectronEffPtBarrel.SetYaxisTitle("Fake rate");
  ElectronEffPtBarrel.SetYaxisMin(0.0);
  ElectronEffPtBarrel.Write();

  sprintf(filename,"%s-EleMisidPtEndcap.root",outname_.c_str());
  EfficiencyEvaluator ElectronEffPtEndcap(filename);
  ElectronEffPtEndcap.AddNumerator(RecoPtEndcap);
  for (int icut=0;icut<(int)EgammaEleID.size();++icut){
    ElectronEffPtEndcap.AddNumerator(ElePtEndcap[icut]);
  }

  ElectronEffPtEndcap.SetDenominator(RecoPtEndcap);
  ElectronEffPtEndcap.ComputeEfficiencies();
  ElectronEffPtEndcap.SetTitle("fake rate vs pT");
  ElectronEffPtEndcap.SetXaxisTitle("electron pT");
  ElectronEffPtEndcap.SetYaxisTitle("Fake rate");
  ElectronEffPtEndcap.SetYaxisMin(0.0);
  ElectronEffPtEndcap.Write();

  // === as a function of PU ===
  sprintf(filename,"%s-EleMisidPUBarrel.root",outname_.c_str());
  EfficiencyEvaluator ElectronEffPUBarrel(filename);
  ElectronEffPUBarrel.AddNumerator(RecoPUBarrel);
  for (int icut=0;icut<(int)EgammaEleID.size();++icut){
    ElectronEffPUBarrel.AddNumerator(ElePUBarrel[icut]);
  }

  ElectronEffPUBarrel.SetDenominator(RecoPUBarrel);
  ElectronEffPUBarrel.ComputeEfficiencies();
  ElectronEffPUBarrel.SetTitle("fake rate vs vertices");
  ElectronEffPUBarrel.SetXaxisTitle("# vertices");
  ElectronEffPUBarrel.SetYaxisTitle("Fake rate");
  ElectronEffPUBarrel.SetYaxisMin(0.0);
  ElectronEffPUBarrel.Write();

  sprintf(filename,"%s-EleMisidPUEndcap.root",outname_.c_str());
  EfficiencyEvaluator ElectronEffPUEndcap(filename);
  ElectronEffPUEndcap.AddNumerator(RecoPUEndcap);
  for (int icut=0;icut<(int)EgammaEleID.size();++icut){
    ElectronEffPUEndcap.AddNumerator(ElePUEndcap[icut]);
  }

  ElectronEffPUEndcap.SetDenominator(RecoPUEndcap);
  ElectronEffPUEndcap.ComputeEfficiencies();
  ElectronEffPUEndcap.SetTitle("fake rate vs vertices");
  ElectronEffPUEndcap.SetXaxisTitle("# vertices");
  ElectronEffPUEndcap.SetYaxisTitle("Fake rate");
  ElectronEffPUEndcap.SetYaxisMin(0.0);
  ElectronEffPUEndcap.Write();

}


