// C++ includes
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>

// ROOT includes
#include <TROOT.h>
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TCanvas.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TVector3.h>

// vecbos includes
#include "Math/src/HistogramTools.cc"

namespace macros {

  class PlotMomentumResolution {
  public:
    /// default constructor
    PlotMomentumResolution() : iecal_(-1) { };
    /// destructor 
    ~PlotMomentumResolution() { };
    
    /// set iecal
    enum iecal {eb=0, ee};
    void setBarrel() { iecal_=eb; }
    void setEndcap() { iecal_=ee; }

    /// compare resolutions
    void compareResolutions();

    enum est {combcorr=0, comb, ecal, track};
    /// get the momentum resolution as estimated from different components of the electron
    TGraphErrors* trackResolution() { return resolution(track); }
    TGraphErrors* ecalResolution()  { return resolution(ecal); }
    TGraphErrors* combResolution()  { return resolution(comb); }
    TGraphErrors* combCorrResolution()  { return resolution(combcorr); }

  private:
    /// return the graph of the resolution, given the momentum estimate
    TGraphErrors* resolution(int iestimate);
    /// get the effective RMS
    float effRMS(TH1F* histo);
    /// the ECAL sector
    int iecal_;

  };
}

using namespace std;
using namespace macros;
using namespace math;

TGraphErrors* PlotMomentumResolution::resolution(int iestimate) {
  
  TFile *file = TFile::Open("data/electronidmc.root");
  TTree *tree = (TTree*)file->Get("electrons/T1");

  Float_t         etrue;
  Float_t         thetatrue;
  Float_t         phitrue;
  Float_t         pt[4];
  Float_t         eta[4];
  Float_t         phi[4];
  tree->SetBranchAddress("etrue", &etrue);
  tree->SetBranchAddress("thetatrue", &thetatrue);
  tree->SetBranchAddress("phitrue", &phitrue);
  tree->SetBranchAddress("pt", pt);
  tree->SetBranchAddress("eta", eta);
  tree->SetBranchAddress("phi", phi);

  TH1F* resolution = new TH1F("resolution","",50,-0.5,0.5);

  HistogramTools htools(resolution);

  // stringstream absresol;
  // cout << "iestimate = " << iestimate << endl;
  // absresol << "(pt[" << iestimate <<"]/abs(sin(2*atan(exp(-eta["<< iestimate << "]))))-etrue)";

  TGraphErrors *result = new TGraphErrors();
  result->SetName("resograph");

  float step = 4.0;
  int point=0;
  for(float p=7.0; p<100.0; p+=step) {
    if(p>40) step = 8;

    /// fill the histogram
    resolution->Reset();
    for(int i=0;i<tree->GetEntries();++i) {
      tree->GetEntry(i);
      TVector3 preco;
      preco.SetPtEtaPhi(pt[iestimate],eta[3],phi[3]);
      TVector3 ptrue;
      ptrue.SetMagThetaPhi(etrue,thetatrue,phitrue);
      float resol = (preco.Mag()-ptrue.Mag())/ptrue.Mag();
      if(iecal_ == 0 && fabs(ptrue.Eta())>1.479) continue;
      if(iecal_ == 1 && fabs(ptrue.Eta())<1.479) continue;
      if(preco.Mag()>p && preco.Mag()<p+step) resolution->Fill(resol);
    }

    cout << "p = " << p << "\tnumetries = " << resolution->GetEntries() << endl;
    result->SetPoint( point,p,htools.effectiveSigma() );
    point++;
  }
  return result;
}

void PlotMomentumResolution::compareResolutions() {

  TGraphErrors *trk_reso = trackResolution();
  TGraphErrors *ecal_reso = ecalResolution();
  TGraphErrors *comb_reso = combResolution();
  TGraphErrors *combCorr_reso = combCorrResolution();

  /// points
  trk_reso->SetMarkerStyle(kOpenSquare);
  ecal_reso->SetMarkerStyle(kOpenCircle);
  comb_reso->SetMarkerStyle(kFullCircle);
  combCorr_reso->SetMarkerStyle(kFullCircle);
  combCorr_reso->SetMarkerColor(kRed);

  TCanvas *can = new TCanvas("can","",600,600);
  trk_reso->Draw("AP");
  ecal_reso->Draw("P");
  comb_reso->Draw("P");
  combCorr_reso->Draw("P");

  can->SaveAs("electron_resolution.pdf");

}
