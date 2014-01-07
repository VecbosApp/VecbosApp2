#ifndef VECBOS_GENPARTICLE_CANDIDATE_MATCH_HH
#define VECBOS_GENPARTICLE_CANDIDATE_MATCH_HH

#include "DataFormats/include/CompositeCandidate.hh"
#include "DataFormats/include/GenParticle.hh"
#include "DataFormats/include/GenParticleFwd.hh"

namespace vecbos {
  
  class GenParticleCandidateMatch {

  public:
    /// default constructor
    GenParticleCandidateMatch(GenParticleCollectionPtr genparticles);
    /// destructor 
    ~GenParticleCandidateMatch() { };

    /// set the delta R for the angular match
    void setDeltaR(float deltaR) { deltaR_ = deltaR; }

    /// if set, check charge of the matched candidate and the gen particle (assume +/-1 for pdgId >/< 0)
    void checkCharge(bool check) { checkCharge_ = check; }

    /// if set, matches only with the required status of the genParticle
    /// default is no check on the status
    /// PYTHIA status 1=final state as given by the generator, 3=immediate decays of resonances
    /// typically matching to status=1 particles
    void checkStatus(int status) { status_ = status; }

    /// overlap between the candidate and a genparticle of given pdgId
    GenParticle* overlap(CompositeCandidate *cand, int genParticlePdgId);

  private:
    /// the list of GenParticles available in the event
    GenParticleCollectionPtr genParticles_;
    /// the deltaR for angular match
    float deltaR_;
    /// bool check charge of the matched candidate and the gen particle
    bool checkCharge_;
    /// required status of the genParticle
    int status_;

  };
}

#endif
