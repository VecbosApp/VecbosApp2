#include <algorithm>
#include <iostream>

#include "Tools/include/CandidateSorter.hh"
#include "DataFormats/include/Candidate.hh"
#include "DataFormats/include/CompositeCandidate.hh"

using namespace vecbos;
using namespace std;

CompositeCandidateCollection CandidateSorter::output() {
  
  typedef std::pair<double, CompositeCandidate *> ValCand;
  std::vector<ValCand> pairs;

  for (CompositeCandidateCollection::iterator it = input_.begin(); it < input_.end(); ++it) {    
    double val = 0;
    if(sortBy_.compare("pt")==0) val = it->pt();
    else if(sortBy_.compare("massval")==0) val = abs(it->mass()-polemass_);
    else if(sortBy_.compare("sumpt")==0) {
      for(int dau=0; dau < it->numberOfDaughters(); ++dau) val += it->daughter(dau).pt();
    }
    
    if (!sortAscending_) val = -val;
    pairs.push_back(std::make_pair(val,&(*it)));
  }
  
  std::sort(pairs.begin(), pairs.end());
  
  int n = fmin(pairs.size(), max_);
  
  CompositeCandidateCollection out;
  out.reserve(n);
  for(int i = 0; i < n; ++i) {
    out.push_back(*(pairs[i].second));
  } 
  
  return out;
  
}
