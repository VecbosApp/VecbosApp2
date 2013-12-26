#include "Math/include/Constants.h"
#include "DataFormats/include/Muon.hh"

using namespace vecbos;

Muon::Muon( int charge, const LorentzVector & p4, const Point & vertexPosition,
	    Track innerTrack, Track staTrack, Track gblTrack, Track combTrack) :
  innerTrack_(innerTrack), standaloneTrack_(staTrack), globalMuonTrack_(gblTrack), combinedTrack_(combTrack) {

  // candidate generic variables
  charge_ = charge;
  pt_  = p4.Pt();
  eta_ = p4.Eta();
  phi_ = p4.Phi();
  math::PhysConstants constants;
  mass_ = constants.muon_mass;
  vertex_ = vertexPosition;
  pdgId_ = (charge<0) ? 13 : -13;
  status_ = 0;
  cacheCartesianFixed_ = false;


}

Muon::LorentzVector Muon::calibratedP4() {
  Vector direction=momentum().Unit();
  Vector scaledMomentum = direction*calibratedMomentum_;
  
  LorentzVector calibratedP4;
  math::PhysConstants constants;
  calibratedP4.SetVectM(scaledMomentum,constants.muon_mass);

  return calibratedP4;
}
