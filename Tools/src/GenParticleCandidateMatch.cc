#include "Tools/include/GenParticleCandidateMatch.hh"

using namespace vecbos;
using namespace std;

GenParticleCandidateMatch::GenParticleCandidateMatch(GenParticleCollectionPtr genparticles)
  : genParticles_(genparticles), deltaR_(0.1), checkCharge_(false), status_(-1) { }

GenParticle* GenParticleCandidateMatch::overlap(CompositeCandidate *cand, int genParticlePdgId) {
  
  GenParticle *particle = 0;
  for(genparticleptr_iterator it = genParticles_.begin(); it != genParticles_.end(); ++it) {
    int chargePart = (genParticlePdgId>0) ? 1 : -1;
    int chargeCand = (cand->charge()) ? 1 : -1;
    if(checkCharge_ && ( chargeCand != chargePart )) continue;
    if(status_ > -1 && (*it)->status()!=status_ ) continue;
    if( (*it)->pdgId()==genParticlePdgId && cand->angularMatch(*(*it),deltaR_)) {
      particle = *it;
      break;
    }
  }
  return particle;
}
