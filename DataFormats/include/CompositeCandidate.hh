#ifndef VECBOS_COMPOSITE_CANDIDATE_H
#define VECBOS_COMPOSITE_CANDIDATE_H

#include "DataFormats/include/Candidate.hh"
#include "DataFormats/include/CandidateFwd.hh"

#include "TLorentzVector.h"
#include "TVector3.h"
#include <string>
#include <vector> 

namespace vecbos {
  
  class CompositeCandidate : public Candidate {
  public:

    /// electric charge type
    typedef int Charge;
    /// Lorentz vector
    typedef TLorentzVector LorentzVector;
    /// point in the space
    typedef TVector3 Point;
    /// point in the space
    typedef TVector3 Vector;
    /// collection of daughters
    typedef CandidateCollection daughters;

    /// default constructor
    CompositeCandidate(std::string name="") : Candidate(), name_(name) { }

    /// constructor from values
    CompositeCandidate( int charge, const LorentzVector & p4, const Point & vtx = Point( 0, 0, 0 ),
                        int pdgId = 0, int status = 0, std::string name="") :
      Candidate( charge, p4, vtx, pdgId, status ), name_(name) { }
    /// destructor
    virtual ~CompositeCandidate();
    /// get the name of the candidate
    std::string name() const { return name_;}
    /// set the name of the candidate
    void        setName(std::string name) { name_ = name;}
    /// number of daughters
    virtual int numberOfDaughters();
    /// return daughter at a given position, i = 0, ... numberOfDaughters() - 1
    virtual Candidate daughter( int );
    /// add a clone of the passed candidate as daughter 
    void addDaughter( Candidate & cand);
    /// clear daughters
    void clearDaughters() { dau.clear(); }

  private:
    /// collection of daughters
    daughters dau;
    /// candidate name
    std::string name_;
  };

}

#endif
