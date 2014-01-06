#ifndef VECBOS_CANDIDATE_COMBINER_HH
#define VECBOS_CANDIDATE_COMBINER_HH

#include <string>
#include <vector>

#include "DataFormats/include/CandidateFwd.hh"
#include "DataFormats/include/CompositeCandidateFwd.hh"

namespace vecbos {

  class CandidateCombiner {
  public:

    /// constructor
    CandidateCombiner(std::string name="") :
      name_(name), checkCharge_(false) { inputs_.clear(); };
    /// destructor
    ~CandidateCombiner() { };
    
    /// add one input collection to be combined
    void addDaughterCollection(CandidateCollection daus) { inputs_.push_back(daus); }

    /// get the output
    CompositeCandidateCollection output();
  
  private:
    /// the list of candidate collections to be combined
    std::vector<CandidateCollection> inputs_;
    /// check the opposite charge between the 2 daughters
    bool checkCharge_;
    /// the name of the composite candidate formed
    std::string name_;

    /// combine two leaf candidate collections
    CompositeCandidateCollection combineTwoCollections(CandidateCollection first, CandidateCollection second);
    /// combine one composite and one leaf candidate collections
    CompositeCandidateCollection combineCompositeCollectionAndLeafCollection(CompositeCandidateCollection first, CandidateCollection second);

  };

}

#endif

