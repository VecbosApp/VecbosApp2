#ifndef VECBOS_GENPARTICLE_H
#define VECBOS_GENPARTICLE_H

#include "DataFormats/include/Candidate.hh"
#include "DataFormats/include/CandidateFwd.hh"

namespace vecbos {

  class GenParticle : public Candidate {
    
  public:
    /// default constructor
    GenParticle() : Candidate() { }
    /// constructor from values
    GenParticle(int charge, const LorentzVector & p4, const Point & vtx = Point( 0, 0, 0 ),
		int pdgId = 0, int status = 0, int mother = -1);
    /// destructor
    ~GenParticle() { };

    /// set the mother index in the genparticle list
    void setMother(int index);
    /// get the mother 
    Candidate* mother(CandidateCollectionPtr genParticles);
    /// tell if the particle has a mother in the list of the saved particles
    bool hasMother(CandidateCollectionPtr genParticles) { return mother_ > 0 && mother_ < genParticles.size(); }


  private:
    int mother_;

  };

}

#endif

