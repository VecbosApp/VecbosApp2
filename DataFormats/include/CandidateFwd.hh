#ifndef VECBOS_CANDIDATE_FWD_H
#define VECBOS_CANDIDATE_FWD_H

#include "DataFormats/include/Candidate.hh"

namespace vecbos {

  typedef std::vector<vecbos::Candidate> CandidateCollection;
  typedef std::vector<vecbos::Candidate*> CandidateCollectionPtr;
  typedef std::vector<vecbos::Candidate*>::iterator candidateptr_iterator;

}

#endif

