#include "DataFormats/include/RecoCandidate.hh"
#include "DataFormats/include/Track.hh"

using namespace vecbos;

RecoCandidate::~RecoCandidate() { }

Track RecoCandidate::track() const {
  return Track();
}

Track RecoCandidate::gsfTrack() const {
   return Track();
}

Track RecoCandidate::standAloneMuon() const {
  return Track();
}

Track RecoCandidate::combinedMuon() const {
  return Track();
}

SuperCluster RecoCandidate::superCluster() const {
  return SuperCluster();
}

