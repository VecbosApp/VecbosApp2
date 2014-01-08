#ifndef VECBOS_CANDIDATE_FILTER_HH
#define VECBOS_CANDIDATE_FILTER_HH

#include <utility>
#include "DataFormats/include/Candidate.hh"
#include "Tools/include/CollectionSelector.hh"

namespace vecbos {

  class CandidateKinematicFilter : public CollectionSelector<Candidate*> {

    /// a cut on a range (min, max)
    typedef std::pair<float,float> range;
    /// candidate collection
    typedef std::vector<Candidate*> collection;

  public:
    /// default constructor
    CandidateKinematicFilter();
    /// constructor from an input collection
    CandidateKinematicFilter(collection input);
    /// destructor
    ~CandidateKinematicFilter() { };

    /// set the pt range
    void setPtRange(float min, float max) { ptrange_.first = min;  ptrange_.second = max; }
    /// set the eta range
    void setEtaRange(float min, float max) { etarange_.first = min;  etarange_.second = max; }
    /// set the mass range
    void setMassRange(float min, float max) { massrange_.first = min;  massrange_.second = max; }
    /// set the transverse mass range
    void setMtRange(float min, float max) { mtrange_.first = min;  mtrange_.second = max; }

    /// selected objects
    collection output();
    
  private:
    range ptrange_;
    range etarange_;
    range massrange_;
    range mtrange_;

  };

}

#endif
