#include "DataFormats/include/GenParticle.hh"

using namespace vecbos;

GenParticle::GenParticle(int charge, const LorentzVector & p4, const Point & vtx,
			 int pdgId, int status, int mother) 
  : Candidate(charge, p4, vtx, pdgId, status), mother_(mother) {
  
  cacheCartesianFixed_ = false;
}

Candidate* GenParticle::mother(CandidateCollectionPtr genParticles) {
  if(mother_ > 0 && mother_ < genParticles.size() ) return genParticles[mother_];
  else return 0;
}
