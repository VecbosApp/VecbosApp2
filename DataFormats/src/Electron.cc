#include "Math/include/Constants.h"
#include "DataFormats/include/Electron.hh"

using namespace vecbos;
using namespace std;

Electron::Electron( int charge,  const LorentzVector & p4, const Point & vertexPosition,
		    SuperCluster supercluster, SuperCluster pfsupercluster, 
		    Track gsfTrack, Track ctfTrack) :
  superCluster_(supercluster), pfSuperCluster_(supercluster), gsfTrack_(gsfTrack), ctfTrack_(ctfTrack) {
  
  // candidate generic variables
  charge_ = charge;
  pt_  = p4.Pt();
  eta_ = p4.Eta();
  phi_ = p4.Phi();
  math::PhysConstants constants;
  mass_ = constants.electron_mass;
  vertex_ = vertexPosition;
  pdgId_ = (charge<0) ? 11 : -11;
  status_ = 0;
  cacheCartesianFixed_ = false;
  
  // electron specific variables
  trackExtrapolations_.positionAtVtx = vertexPosition;
  trackExtrapolations_.momentumAtVtx = gsfTrack_.momentum();

  showerShape_.sigmaIetaIeta = superCluster_.sigmaIetaIeta();
  showerShape_.sigmaIphiIphi = superCluster_.sigmaIphiIphi();
  showerShape_.e1x5 = superCluster_.e1x5();
  showerShape_.e2x5Max = superCluster_.e2x5Max();
  showerShape_.e5x5 = superCluster_.e5x5();
  showerShape_.r9 = superCluster_.e3x3() / superCluster_.energy();

  // reconstruction and ECAL fiducial flags
  recoflags_ = -1;
  fiducialflags_ = -1;

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

Electron::LorentzVector Electron::calibratedP4() {
  Vector direction=momentum().Unit();
  Vector scaledMomentum = direction*calibratedEnergy_;
  
  LorentzVector calibratedP4;
  math::PhysConstants constants;
  calibratedP4.SetVectM(scaledMomentum,constants.electron_mass);

  return calibratedP4;
}

bool Electron::ecalDrivenSeed() const {
  return (recoflags_ >> vecbosflags::isEcalDriven)%2;
}

bool Electron::trackerDrivenSeed() const {
  return (recoflags_ >> vecbosflags::isTrackerDriven)%2;
}

void Electron::setFiducialFlags(int flags) { 
  fiducialflags_ = flags; 
  fiducialFlags_.isEB = (fiducialflags_ >> vecbosflags::isEB)%2;
  fiducialFlags_.isEE = (fiducialflags_ >> vecbosflags::isEE)%2;
  fiducialFlags_.isEBEEGap = (fiducialflags_ >> vecbosflags::isEBEEGap)%2;
  fiducialFlags_.isEBEtaGap = (fiducialflags_ >> vecbosflags::isEBEtaGap)%2;
  fiducialFlags_.isEBPhiGap = (fiducialflags_ >> vecbosflags::isEBPhiGap)%2;
  fiducialFlags_.isEEDeeGap = (fiducialflags_ >> vecbosflags::isEEDeeGap)%2;
  fiducialFlags_.isEERingGap = (fiducialflags_ >> vecbosflags::isEERingGap)%2;
}

bool Electron::isInFiducialRegion() {
  return ( fabs(eta()) < 1.4442 || // EB
           (fabs(eta()) > 1.560 && fabs(eta()) < 2.5 ) // EE
           );
}

