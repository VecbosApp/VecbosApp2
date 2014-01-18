#ifndef VECBOS_PFJET_FWD_HH
#define VECBOS_PFJET_FWD_HH

#include "DataFormats/include/PFJet.hh"

namespace vecbos {

  typedef std::vector<vecbos::PFJet> PFJetCollection;
  typedef std::vector<vecbos::PFJet>::iterator jet_iterator;

  typedef std::vector<vecbos::PFJet*> PFJetCollectionPtr;
  typedef std::vector<vecbos::PFJet*>::iterator jetptr_iterator;

}

#endif
