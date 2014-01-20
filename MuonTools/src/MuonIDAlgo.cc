#include <iostream>
#include "Tools/include/VertexSelector.hh"

#include "MuonTools/include/MuonIDAlgo.hh"

using namespace vecbos;
using namespace std;

MuonIDAlgo::MuonIDAlgo(float rho, vecbos::VertexCollection vertices) :
  rho_(rho)
{
  muon_ = Muon();
  /// filter and take the first one
  VertexSelector stdselector(vertices);
  bestVertex_ = stdselector.goodCollection().front();
}

int MuonIDAlgo::getIntCutsWP(std::string wp) {
  if(wp.compare("loose")==0) return 0; // hzz4l
  else if(wp.compare("tight")==0) return 1; // hww2l2nu
  else return -1;
}

bool MuonIDAlgo::pass_cuts_id(std::string wp) {
  /// independent on the wp so far
  bool pass=true;

  /// preselection
  bool good_global_mu = muon_.isGlobalMuon() || muon_.numberOfMatches() > 0;
  if(fabs(muon_.eta()) > 2.4 || (!good_global_mu) ) pass=false;
  /// PF muon ID
  if(!muon_.isPFId()) pass=false;

  return pass;
}

bool MuonIDAlgo::pass_cuts_iso(std::string wp) {
  /// cuts[wp:loose,tight]
  float cuts_pfiso[2] = { 0.4, 0.2 };

  int intwp=getIntCutsWP(wp);

  bool pass=true;
  if(muon_.dr04pfIsolationDeltaBetaCorr()/muon_.pt() > cuts_pfiso[intwp]) pass=false;
  
  return pass;
}

bool MuonIDAlgo::pass_cuts_ip(std::string wp) {
  /// cuts: only the HZZ is implemented now
  /// independent on the wp so far
  int intwp=getIntCutsWP(wp);
  
  bool pass=true;
  /// impact parameter
  if(muon_.innerTrack().sip3D() >= 4.0) pass=false;
  return pass;
}

