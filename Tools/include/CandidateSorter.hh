#ifndef VECBOS_CANDIDATE_SORTER_HH
#define VECBOS_CANDIDATE_SORTER_HH

#include <string>
#include <vector>

#include "DataFormats/include/CompositeCandidateFwd.hh"

namespace vecbos {

  class CandidateSorter {
  public:
    /// constructor
    CandidateSorter(CompositeCandidateCollection candidates, std::string sortBy) :
      input_(candidates), sortBy_(sortBy), polemass_(0) { max_ = std::numeric_limits<float>::infinity(); sortAscending_=false; }
    /// destructor
    ~CandidateSorter() { }
    
    /// if set, return the first n
    void setMax(int n) { max_ = n; }
    /// set the pole mass of the candidates to be sorted
    void setPoleMass(float mass) { polemass_ = mass; }
    /// if set, sort the collection in ascending value of the sortBy 
    void sortAscending() { sortAscending_=true; }
    
    /// get the sorted collection
    CompositeCandidateCollection output();


  private:

    CompositeCandidateCollection input_;
    std::string sortBy_;
    int max_;
    bool sortAscending_;
    float polemass_;

  };

}

#endif

