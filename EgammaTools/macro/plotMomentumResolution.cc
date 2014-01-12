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
#include "TObjArray.h"
#include "TObjString.h"

// vecbos includes
#include "Math/src/HistogramTools.cc"

namespace macros {

  class PlotMomentumResolution {
  public:
    /// default constructor
    PlotMomentumResolution() { };

    /// destructor 
    ~PlotMomentumResolution() { };

    /// get the track momentum resolution
    enum est {combcorr=0, comb, ecal, track};

    TGraphErrors* trackResolution(std::string cut) { return resolution(track,cut); }
    TGraphErrors* ecalResolution(std::string cut)  { return resolution(ecal,cut); }
    TGraphErrors* combResolution(std::string cut)  { return resolution(comb,cut); }
    TGraphErrors* combCorrResolution(std::string cut)  { return resolution(combcorr,cut); }

  private:
    /// return the graph of the resolution, given the momentum estimate
    TGraphErrors* resolution(int iestimate, std::string cut);
    /// get the effective RMS
    float effRMS(TH1F* histo);
   
  };
}

using namespace std;
using namespace macros;
using namespace math;

TGraphErrors* PlotMomentumResolution::resolution(int iestimate, std::string cut) {
  
  TFile *file = TFile::Open("data/electronidmc.root");
  TTree *tree = (TTree*)file->Get("electrons/T1");

  TH1F* resolution = new TH1F("resolution","",500,-0.5,0.5);

  HistogramTools htools(resolution);

  stringstream variable;
  variable << "(pt[" << iestimate <<"]/abs(sin(2*atan(exp(-eta["<< iestimate << "]))))-etrue)/"
	   << "pt[" << iestimate <<"]/abs(sin(2*atan(exp(-eta["<< iestimate << "]))))";

  TGraphErrors *result = new TGraphErrors();
  result->SetName("resograph");

  float step = 2.0;
  int point=0;
  for(float p=7.0; p<100.0; p+=step) {
    stringstream pcut;
    pcut << "&& (etrue > " << p << " && etrue < " << step << ")";
    string fullcut = cut + pcut.str();
    tree->Project("resolution",variable.str().c_str(),fullcut.c_str());
    cout << "p = " << p << "\tfullcut = " << fullcut << endl;
    
    result->SetPoint( point,p,htools.effectiveSigma() );
    point++;
  }
  return result;
}

