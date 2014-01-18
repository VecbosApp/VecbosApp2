#ifndef VECBOS_PFJET_FWD_HH
#define VECBOS_PFJET_FWD_HH

#include "DataFormats/include/PFJet.hh"

namespace vecbos {

  typedef std::vector<vecbos::PFJet> PFJetCollection;
  typedef std::vector<vecbos::PFJet>::iterator pfjet_iterator;

  typedef std::vector<vecbos::PFJet*> PFJetCollectionPtr;
  typedef std::vector<vecbos::PFJet*>::iterator pfjetptr_iterator;

}

#endif
