#ifndef VECBOS_COMPOSITE_CANDIDATE_FWD_H
#define VECBOS_COMPOSITE_CANDIDATE_FWD_H

#include "DataFormats/include/CompositeCandidate.hh"

namespace vecbos {

  /// collection of Candidate objects
  typedef std::vector<CompositeCandidate> CompositeCandidateCollection;

  /// collection of pointers of Candidate objects
  typedef std::vector<CompositeCandidate*> CompositeCandidateCollectionPtr;

}

#endif
