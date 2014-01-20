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
      name_(name), checkCharge_(false), massMin_(-1.), massMax_(14000) { inputs_.clear(); };
    /// destructor
    ~CandidateCombiner() { };
    
    /// add one input collection to be combined
    void addDaughterCollection(CandidateCollectionPtr daus) { inputs_.push_back(daus); }

    /// set the accepted mass range for the outupt (default is take everything)
    void setMassRange(float min, float max) { massMin_ = min; massMax_ = max; }

    /// get the output (if the mass range is set, get only the candidates fullfilling the mass range
    CompositeCandidateCollection output();
  
  private:
    /// the list of candidate collections to be combined
    std::vector<CandidateCollectionPtr> inputs_;
    /// check the opposite charge between the 2 daughters
    bool checkCharge_;
    /// the name of the composite candidate formed
    std::string name_;

    /// combine two leaf candidate collections
    CompositeCandidateCollection combineTwoCollections(CandidateCollectionPtr first, CandidateCollectionPtr second);
    /// combine one composite and one leaf candidate collections
    CompositeCandidateCollection combineCompositeCollectionAndLeafCollection(CompositeCandidateCollection first, CandidateCollectionPtr second);

    /// mass range accepted
    float massMin_, massMax_;

  };

}

#endif

