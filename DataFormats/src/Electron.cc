#include "Math/include/Constants.h"
#include "DataFormats/include/Electron.hh"

using namespace vecbos;
using namespace std;

Electron::Electron( int charge, SuperCluster supercluster, SuperCluster pfsupercluster, Track gsfTrack, Track ctfTrack, Point vertexPosition) :
  charge_(charge), superCluster_(supercluster), pfSuperCluster_(supercluster), gsfTrack_(gsfTrack), ctfTrack_(ctfTrack) {
  
  trackExtrapolations_.positionAtVtx = vertexPosition;
  trackExtrapolations_.momentumAtVtx = gsfTrack_.momentum();

  showerShape_.sigmaIetaIeta = superCluster_.sigmaIetaIeta();
  showerShape_.sigmaIphiIphi = superCluster_.sigmaIphiIphi();
  showerShape_.e1x5 = superCluster_.e1x5();
  showerShape_.e2x5Max = superCluster_.e2x5Max();
  showerShape_.e5x5 = superCluster_.e5x5();
  showerShape_.r9 = superCluster_.e3x3() / superCluster_.energy();

 }

void Electron::setScPixCharge(int charge) {

  chargeInfo_.scPixCharge = charge;
  chargeInfo_.isGsfCtfScPixConsistent = (gsfTrack_.charge()==ctfTrack_.charge() &&
					 gsfTrack_.charge()==chargeInfo_.scPixCharge  );
  chargeInfo_.isGsfScPixConsistent =  (gsfTrack_.charge()==chargeInfo_.scPixCharge);
  chargeInfo_.isGsfCtfConsistent = ( gsfTrack_.charge()==ctfTrack_.charge() );
  
}

float Electron::superClusterFbrem() const {
  float pout = trackPAtOuter();
  float electronClusterEnergy = trackClusterMatching_.eEleClusterOverPout * pout;
  return (superCluster_.energy() - electronClusterEnergy)/superCluster_.energy() ;
}

float Electron::pfSuperClusterFbrem() const {
  float pout = trackPAtOuter() ;
  float electronClusterEnergy = trackClusterMatching_.eEleClusterOverPout * pout;
  return (pfSuperCluster_.energy() - electronClusterEnergy)/pfSuperCluster_.energy() ;
}

Electron::LorentzVector Electron::calibratedMomentum() {
  Vector direction=momentum().Unit();
  Vector scaledMomentum = direction*calibratedEnergy_;
  
  LorentzVector calibratedP4;
  math::PhysConstants constants;
  calibratedP4.SetVectM(scaledMomentum,constants.electron_mass);

  return calibratedP4;
}