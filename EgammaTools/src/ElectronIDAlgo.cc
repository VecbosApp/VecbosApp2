#include <iostream>
#include "EgammaTools/include/ElectronEffectiveArea.hh"

#include "EgammaTools/include/ElectronIDAlgo.hh"

using namespace vecbos;
using namespace std;

bool ElectronIDAlgo::pass_hlt() {
  /// cuts[EB,EE]
  float cuts_ecaliso[2] = {0.2, 0.2};
  float cuts_hcaliso[2] = {0.2, 0.2};
  float cuts_trkiso[2] = {0.2, 0.2};
  float cuts_hoe[2] = {0.12, 0.10};
  float cuts_see[2] = {0.01, 0.03};
  float cuts_deta[2] = {0.007, 0.009};
  float cuts_dphi[2] = {0.15, 0.10};
  float cuts_dxy[2] = {0.02, 0.02};
  float cuts_dz[2] = {0.10, 0.10};

  int i = electron_.isEB() ? 0 : 1;

  bool pass = true;

  /// acceptance
  if(electron_.pt()<10. || fabs(electron_.eta())>2.5) pass=false;
  /// isolation
  float ecaliso = (electron_.isEB()) ? fmax(0.0,electron_.dr03EcalRecHitSumEt()-1.0) : electron_.dr03EcalRecHitSumEt();
  if(ecaliso/electron_.pt() > cuts_ecaliso[i]) pass=false;
  if(electron_.dr03HcalTowerSumEt()/electron_.pt() > cuts_hcaliso[i]) pass=false;
  if(electron_.dr03TkSumPt()/electron_.pt() > cuts_trkiso[i]) pass=false;
  /// H/E
  if(electron_.hcalOverEcal() > cuts_hoe[i]) pass=false;
  /// pure electron ID
  if(electron_.sigmaIetaIeta() > cuts_see[i]) pass=false;
  if(fabs(electron_.deltaEtaSuperClusterTrackAtVtx()) > cuts_deta[i]) pass=false;
  if(fabs(electron_.deltaPhiSuperClusterTrackAtVtx()) > cuts_dphi[i]) pass=false;
  /// conv rej
  if(electron_.gsfTrack().trackerExpectedHitsInner() > 0 || 
     electron_.hasMatchedConv()) pass=false;
  /// impact parameter
  if(fabs(electron_.gsfTrack().dxy()) > cuts_dxy[i]) pass=false;
  if(fabs(electron_.gsfTrack().dz()) > cuts_dz[i]) pass=false;
  
  return pass;

}

int ElectronIDAlgo::getIntCutsWP(std::string wp) {
  if(wp.compare("veto")==0) return 0;
  else if(wp.compare("loose")==0) return 1;
  else if(wp.compare("medium")==0) return 2;
  else if(wp.compare("tight")==0) return 3;
  else return -1;
}

bool ElectronIDAlgo::pass_cuts_id(std::string wp) {
  /// cuts[eta:EB,EE][wp:veto,loose,medium,tight]
  float cuts_deta[2][4] = { {0.007, 0.007, 0.004, 0.004}, {0.010, 0.009, 0.007, 0.005} };
  float cuts_dphi[2][4] = { {0.800, 0.150, 0.060, 0.030}, {0.700, 0.100, 0.030, 0.020} };
  float cuts_see [2][4] = { {0.010, 0.010, 0.010, 0.010}, {0.030, 0.030, 0.030, 0.030} };
  float cuts_hoe [2][4] = { {0.150, 0.120, 0.120, 0.120}, {1000., 0.100, 0.100, 0.100} };
  float cuts_1e1p[2][4] = { {1000., 0.050, 0.050, 0.050}, {1000., 0.050, 0.050, 0.050} };

  int intwp=getIntCutsWP(wp);
  int i = electron_.isEB() ? 0 : 1;

  bool pass=true;
  /// H/E
  if(electron_.hcalOverEcal() > cuts_hoe[i][intwp]) pass=false;
  /// pure electron ID
  if(electron_.sigmaIetaIeta() > cuts_see[i][intwp]) pass=false;
  if(fabs(electron_.deltaEtaSuperClusterTrackAtVtx()) > cuts_deta[i][intwp]) pass=false;
  if(fabs(electron_.deltaPhiSuperClusterTrackAtVtx()) > cuts_dphi[i][intwp]) pass=false;
  if(fabs(1./electron_.superCluster().energy()-1./electron_.gsfTrack().p()) > cuts_1e1p[i][intwp]) pass=false;
  
  return pass;
}

bool ElectronIDAlgo::pass_cuts_iso(std::string wp) {
  /// cuts[eta:EB,EE][wp:veto,loose,medium,tight]
  float cuts_pfiso[2][4] = { {0.15, 0.15, 0.15, 0.10}, {0.15, 0.15, 0.15, 0.10} };

  int intwp=getIntCutsWP(wp);
  int i = electron_.isEB() ? 0 : 1;

  bool pass=true;
  ElectronEffectiveArea ea;
  if(ea.getRhoCorrectedIsolation03(electron_,rho_)/electron_.pt() > cuts_pfiso[i][intwp]) pass=false;
  
  return pass;
}

bool ElectronIDAlgo::pass_cuts_convrej(std::string wp) {
  int cuts_misshits[2][4] = { {1000, 1, 1, 0}, {1000, 1, 1, 0} };

  int intwp=getIntCutsWP(wp);
  int i = electron_.isEB() ? 0 : 1;

  bool pass=true;
  /// conversion rejection
  if(electron_.gsfTrack().trackerExpectedHitsInner() > cuts_misshits[i][intwp] || 
     electron_.hasMatchedConv()) pass=false;

  return pass;
}

bool ElectronIDAlgo::pass_cuts_ip(std::string wp) {
  /// cuts[eta:EB,EE][wp:veto,loose,medium,tight]
  float cuts_d0  [2][4] = { {0.040, 0.020, 0.020, 0.020}, {0.040, 0.020, 0.020, 0.020} };
  float cuts_dz  [2][4] = { {0.200, 0.200, 0.100, 0.100}, {0.200, 0.200, 0.100, 0.100} };

  int intwp=getIntCutsWP(wp);
  int i = electron_.isEB() ? 0 : 1;
  
  bool pass=true;
  /// impact parameter
  if(fabs(electron_.gsfTrack().d0()) > cuts_d0[i][intwp]) pass=false;
  if(fabs(electron_.gsfTrack().dz()) > cuts_dz[i][intwp]) pass=false;

  return pass;
}



bool ElectronIDAlgo::pass_mva_id(std::string algo, std::string wp) {
  bool pass=false;
  if(algo.compare("mva")==0 && wp.compare("tight")==0) {
    if(!pass_hlt()) return false;
    float pt = electron_.pt();
    float abseta = fabs(electron_.superCluster().eta());
    float bdt = electron_.mvaTriggering();
    if ( ( pt <= 20 && abseta >= 0.000 && abseta < 0.800 && bdt > 0.00 ) ||
	 ( pt <= 20 && abseta >= 0.800 && abseta < 1.479 && bdt > 0.10 ) ||
	 ( pt <= 20 && abseta >= 1.479 && abseta < 2.500 && bdt > 0.62 ) ||
	 ( pt >  20 && abseta >= 0.000 && abseta < 0.800 && bdt > 0.94 ) ||
	 ( pt >  20 && abseta >= 0.800 && abseta < 1.479 && bdt > 0.85 ) ||
	 ( pt >  20 && abseta >= 1.479 && abseta < 2.500 && bdt > 0.92 ) )
      pass=true;
  } else if(algo.compare("mva")==0 && wp.compare("loose")==0) {
    float pt = electron_.pt();
    float abseta = fabs(electron_.eta());
    float bdt = electron_.mvaNonTriggering();
    if ( ( pt <= 10 && abseta >= 0.000 && abseta < 0.800 && bdt >  0.470 ) ||
	 ( pt <= 10 && abseta >= 0.800 && abseta < 1.479 && bdt >  0.004 ) ||
	 ( pt <= 10 && abseta >= 1.479 && abseta < 2.500 && bdt >  0.295 ) ||
	 ( pt >  10 && abseta >= 0.000 && abseta < 0.800 && bdt > -0.340 ) ||
	 ( pt >  10 && abseta >= 0.800 && abseta < 1.479 && bdt > -0.650 ) ||
	 ( pt >  10 && abseta >= 1.479 && abseta < 2.500 && bdt >  0.600 ) )
      pass=true;
  }
  return pass;
}

bool ElectronIDAlgo::pass_mva_iso(std::string wp) {
  bool pass=true;
  ElectronEffectiveArea ea;
  if ( (wp.compare("tight")==0 ) &&
       ea.getRhoCorrectedIsolation03(electron_,rho_)/electron_.pt() > 0.15) pass=false;
  else if ( (wp.compare("loose")==0 ) &&
       ea.getRhoCorrectedIsolation03(electron_,rho_)/electron_.pt() > 0.40) pass=false;

  return pass;
}

bool ElectronIDAlgo::pass_mva_convrej(std::string wp) {
  int max_misshits = 100;
  if( wp.compare("tight")==0 ) max_misshits = 0;
  if( wp.compare("loose")==0 ) max_misshits = 1;

  bool pass=true;
  /// conversion rejection
  if(electron_.gsfTrack().trackerExpectedHitsInner() > max_misshits || 
     electron_.hasMatchedConv()) pass=false;

  return pass;
}

bool  ElectronIDAlgo::pass_mva_ip(std::string wp) {
  bool pass=true;
  if( wp.compare("tight")==0 ) {
    /// impact parameter
    if(fabs(electron_.gsfTrack().dxy()) > 0.020) pass=false;
    if(fabs(electron_.gsfTrack().dz()) > 0.100) pass=false;
  }
  if( wp.compare("loose")==0 ) {
    /// impact parameter
    if(electron_.gsfTrack().sip3D() >= 4.0) pass=false;
  }
  
  return pass;
}

