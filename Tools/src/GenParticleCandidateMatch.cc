#include "Tools/include/GenParticleCandidateMatch.hh"

using namespace vecbos;
using namespace std;

GenParticleCandidateMatch::GenParticleCandidateMatch(CandidateCollectionPtr genparticles)
  : genParticles_(genparticles), deltaR_(0.1), checkCharge_(false), status_(-1) { }

GenParticle* GenParticleCandidateMatch::overlap(Candidate *cand, int genParticlePdgId) {
  
  GenParticle *particle = 0;
  for(candidateptr_iterator it = genParticles_.begin(); it != genParticles_.end(); ++it) {
    int chargePart = (genParticlePdgId>0) ? 1 : -1;
    int chargeCand = (cand->charge()) ? 1 : -1;
    if(checkCharge_ && ( chargeCand != chargePart )) continue;
    if(status_ > -1 && (*it)->status()!=status_ ) continue;
    if(fabs((*it)->pdgId())==fabs(genParticlePdgId) && cand->angularMatch(*(*it),deltaR_)) {
      particle = dynamic_cast<GenParticle*>(*it);
      break;
    }
  }
  return particle;
}
